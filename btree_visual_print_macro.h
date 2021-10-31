/**
 * @file btree_visual_print.h
 * @brief 可视化动态打印二叉树，纯宏版本
 * @version 1.0
 * @date 2021-10-32
 *
 * @copyright Copyright (c) 2021
 *
 */

/*  std=C99 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 需要类似下方的结构体，类似即可，不需要一模一样*/
/*
typedef struct node
{
    char data;
    struct node *lchild, *rchild;
} BTNode, *BTree;
*/
/* data也可以是 int data, char *data, 等等 */
/*但是如果用int *, char*, char**, struct A *, void* 这种再用指针另指元素, 下面函数有两行要修改, 已用TODO标出 */

/**
 * @brief
 * @param BTREE_TYPE 二叉树类型名，也即二叉树结点指针类型名，比如BTree，BTNode*
 * @param ROOT_IDENT 根节点变量名称
 * @param LEFT_IDENT 二叉树结点中左孩子指针域的定义名称，比如lchild，比如left
 * @param RIGHT_IDENT 二叉树结点中右孩子指针域的定义名称，比如rchild，比如right
 * @param DATA_IDENT 二叉树结点中数据域的定义名称，比如data
 * @param ELEM_FMT_STR 元素打印时的格式字符, 比如char用"%c", int用"%d", 字符串用"%s"等等
 * @param FILE_IDENT 要输出的地方, 比如stdout, 又或者某个文件指针
 * @example
 *      BTree t1;
 *      BTREE_VISUAL_PRINT(BTree, t1, lchild, rchild, data, "%d", stdout);
 *
 *      FILE * fp = fopen("out.txt", "w");
 *      BTREE_VISUAL_PRINT(BTree, t1, lchild, rchild, data, "%c", fp);
 *      fclose(fp);
 *
 *  _(5)___
 *  |      |
 * (2)  _(10)__________
 *      |              |
 *     (5)       ____(20)
 *               |
 *            _(17)_
 *            |     |
 *          (12)  (19)
 *
 * 下面这颗树可能有点宽，为了更好的显示效果，你最好关闭自动换行（auto wrap）
 *
 *                    _______________________________(Amy)_____________________
 *                    |                                                        |
 *               ___(Bob)____________________                            ___(Carl)___________________
 *               |                           |                           |                           |
 *    ________(David)            ________(Edison)__                ___(Frank)             _______(George)__
 *    |                          |                 |               |                      |                |
 * (Henry)__               ___(Iris)__          (Jack)       ___(Kevin)             ___(Laura)_         (Mark)_
 *          |              |          |                      |                      |          |               |
 *       (Neil)         (Oscar)    (Peter)               (Queena)                (Robin)     (Sam)           (Tom)
 */
#define BTREE_VISUAL_PRINT(BTREE_TYPE, ROOT_IDENT, LEFT_IDENT, RIGHT_IDENT, DATA_IDENT, ELEM_FMT_STR, FILE_IDENT)                    \
    do                                                                                                                               \
    {                                                                                                                                \
        if (ROOT_IDENT == NULL)                                                                                                      \
            break;                                                                                                                   \
        const int _MAX_STR_LEN = 128;        /* 单个元素打印成str最大长度 */                                              \
        const int _MAX_NODE_NUM = 1024;      /* 结点最大个数 */                                                                \
        const char horiz_conj_char = '_';    /* 连接横线 */                                                                      \
        const char vert_conj_char = '|';     /* 竖线 */                                                                            \
        const char left_bracket_char = '(';  /* 每个元素左边的小括号, 你也可以换成你喜欢的 */                    \
        const char right_bracket_char = ')'; /* 元素右边的小括号 */                                                          \
                                                                                                                                     \
        /* 思路: 层序遍历算深度, 中序遍历算横坐标, 用结构体数组把这些信息存好再统一打印 */        \
        typedef struct _node_print_info                                                                                              \
        {                                                                                                                            \
            BTREE_TYPE address; /* 二次遍历时的唯一区分标志，即key //用hash表将地址映射会更省时间空间 */ \
            int str_len;                                                                                                             \
            int depth;                                                                                                               \
            int left_margin; /*  horizontal coordintate */                                                                           \
        } _Info;                                                                                                                     \
                                                                                                                                     \
        BTREE_TYPE tree_deque[_MAX_NODE_NUM];                                                                                        \
        int depth_queue[_MAX_NODE_NUM]; /* 记录深度的队列 */                                                                  \
        _Info *info_p_arr[_MAX_NODE_NUM];                                                                                            \
        int node_count = 0;                                                                                                          \
        int front = -1, rear = -1;                                                                                                   \
        BTREE_TYPE p;                                                                                                                \
        _Info *info_p;                                                                                                               \
        char temp_str[_MAX_STR_LEN];                                                                                                 \
                                                                                                                                     \
        /* 用队列统计结点的深度信息, 顺便统计每个结点数据打印时的长度 */                                 \
        tree_deque[++rear] = ROOT_IDENT;                                                                                             \
        depth_queue[rear] = 1;                                                                                                       \
        while (front < rear)                                                                                                         \
        {                                                                                                                            \
            p = tree_deque[++front];                                                                                                 \
            info_p = (_Info *)calloc(1, sizeof(_Info));                                                                              \
            info_p->address = p; /* 记录地址 */                                                                                  \
            memset(temp_str, 0, _MAX_STR_LEN);                                                                                       \
            sprintf(temp_str, ELEM_FMT_STR, p->DATA_IDENT); /*  TODO: 注意 */                                                      \
            info_p->str_len = strlen(temp_str) + 2;         /* 计算打印后的元素长度 */                                     \
            info_p->depth = depth_queue[front];                                                                                      \
            info_p_arr[node_count++] = info_p;                                                                                       \
                                                                                                                                     \
            if (p->LEFT_IDENT != NULL)                                                                                               \
            {                                                                                                                        \
                tree_deque[++rear] = p->LEFT_IDENT;                                                                                  \
                depth_queue[rear] = depth_queue[front] + 1;                                                                          \
            }                                                                                                                        \
            if (p->RIGHT_IDENT != NULL)                                                                                              \
            {                                                                                                                        \
                tree_deque[++rear] = p->RIGHT_IDENT;                                                                                 \
                depth_queue[rear] = depth_queue[front] + 1;                                                                          \
            }                                                                                                                        \
        }                                                                                                                            \
                                                                                                                                     \
        /* 接下来统计横坐标, 用中序遍历 */                                                                              \
        p = ROOT_IDENT;                                                                                                              \
        int top = -1, i;                                                                                                             \
        int horizontal_accumu_cache = 0; /* 横坐标累计长度 */                                                                 \
        do                                                                                                                           \
        {                                                                                                                            \
            while (p != NULL)                                                                                                        \
            {                                                                                                                        \
                tree_deque[++top] = p;                                                                                               \
                p = p->LEFT_IDENT;                                                                                                   \
            }                                                                                                                        \
            p = tree_deque[top--];                                                                                                   \
            for (i = 0; i < node_count; ++i)                                                                                         \
            { /* 这里用hash表的优势就体现出来了 */                                                                      \
                if (info_p_arr[i]->address == p)                                                                                     \
                { /* 计算横坐标 */                                                                                              \
                    info_p_arr[i]->left_margin = horizontal_accumu_cache;                                                            \
                    horizontal_accumu_cache += info_p_arr[i]->str_len - 1; /* 减一可以重叠一个括号,更紧凑一点点 */   \
                }                                                                                                                    \
            }                                                                                                                        \
            p = p->RIGHT_IDENT;                                                                                                      \
        } while (!(p == NULL && top == -1));                                                                                         \
                                                                                                                                     \
        /* 接下来开始打印 */                                                                                                  \
        int horiz_left_start, horiz_right_end, cursor, j, k, cur_depth = 1, end_flag = 0;                                            \
        int vert_index_arr[_MAX_NODE_NUM]; /* 偶数行的竖线存储数组 */                                                      \
        i = 0;                                                                                                                       \
        while (i < node_count)                                                                                                       \
        {                                                                                                                            \
            k = -1;                                                                                                                  \
            cursor = 0;                                                                                                              \
            while (info_p_arr[i]->depth == cur_depth)                                                                                \
            {                                                                                                                        \
                /* 打印左边 */                                                                                                   \
                p = info_p_arr[i]->address;                                                                                          \
                if (p->LEFT_IDENT != NULL)                                                                                           \
                { /* 有左孩子说明有横线要打印 */                                                                         \
                    for (j = 0; j < node_count; ++j)                                                                                 \
                    { /* 暴力查找 */                                                                                             \
                        if (info_p_arr[j]->address == p->LEFT_IDENT)                                                                 \
                        {                                                                                                            \
                            horiz_left_start = info_p_arr[j]->left_margin + info_p_arr[j]->str_len / 2;                              \
                            vert_index_arr[++k] = horiz_left_start;                                                                  \
                            break;                                                                                                   \
                        }                                                                                                            \
                    }                                                                                                                \
                    for (; cursor < horiz_left_start; ++cursor)                                                                      \
                        fprintf(FILE_IDENT, " ");                                                                                    \
                    for (; cursor < info_p_arr[i]->left_margin; ++cursor)                                                            \
                        fprintf(FILE_IDENT, "%c", horiz_conj_char);                                                                  \
                }                                                                                                                    \
                else                                                                                                                 \
                { /* 没有左孩子全打印空格即可 */                                                                         \
                    for (; cursor < info_p_arr[i]->left_margin; ++cursor)                                                            \
                        fprintf(FILE_IDENT, " ");                                                                                    \
                }                                                                                                                    \
                                                                                                                                     \
                /* 打印元素 */                                                                                                   \
                fprintf(FILE_IDENT, "%c", left_bracket_char);                                                                        \
                fprintf(FILE_IDENT, ELEM_FMT_STR, p->DATA_IDENT); /* 这里要注意TODO: */                                         \
                fprintf(FILE_IDENT, "%c", right_bracket_char);                                                                       \
                cursor += info_p_arr[i]->str_len;                                                                                    \
                                                                                                                                     \
                /* 打印右边 */                                                                                                   \
                if (p->RIGHT_IDENT != NULL)                                                                                          \
                {                                                                                                                    \
                    for (j = 0; j < node_count; ++j)                                                                                 \
                    {                                                                                                                \
                        if (info_p_arr[j]->address == p->RIGHT_IDENT)                                                                \
                        {                                                                                                            \
                            horiz_right_end = info_p_arr[j]->left_margin + info_p_arr[j]->str_len / 2;                               \
                            vert_index_arr[++k] = horiz_right_end;                                                                   \
                            break;                                                                                                   \
                        }                                                                                                            \
                    }                                                                                                                \
                    for (; cursor < horiz_right_end; ++cursor)                                                                       \
                        fprintf(FILE_IDENT, "%c", horiz_conj_char);                                                                  \
                } /* 右边没有else ,因为只考虑横线即可, 空白算到同层下一个元素左边 */                       \
                                                                                                                                     \
                if (++i >= node_count)                                                                                               \
                {                                                                                                                    \
                    end_flag = 1; /* 打印完最后一个元素, 下一行不需要竖线了,提前结束 */                        \
                    break;                                                                                                           \
                }                                                                                                                    \
            }                                                                                                                        \
            fprintf(FILE_IDENT, "\n");                                                                                               \
                                                                                                                                     \
            /* 打印竖线 */                                                                                                       \
            if (!end_flag)                                                                                                           \
            {                                                                                                                        \
                cursor = 0;                                                                                                          \
                for (j = 0; j <= k; ++j)                                                                                             \
                {                                                                                                                    \
                    for (; cursor < vert_index_arr[j]; ++cursor)                                                                     \
                        fprintf(FILE_IDENT, " ");                                                                                    \
                    fprintf(FILE_IDENT, "%c", vert_conj_char);                                                                       \
                    cursor++;                                                                                                        \
                }                                                                                                                    \
                fprintf(FILE_IDENT, "\n");                                                                                           \
            }                                                                                                                        \
            cur_depth++;                                                                                                             \
        }                                                                                                                            \
        for (i = 0; i < node_count; ++i)                                                                                             \
            free(info_p_arr[i]); /* 释放空间 */                                                                                  \
    } while (0)
/****************************************************************
为了打印水平线很多想法：
暴力法, 暴力查找
如果非要记录下每个结点的孩子在info_arr中的索引, 不管是用虚完全二叉树记录索引法， 还是放在中序遍历中记录父亲节点, 都需要反查其在info_arr中的索引
如果一开始就用hash, 会省很多事
如果在打印时, 多看一行, 利用横坐标行吗? 不行,  不知道结点之间的父子关系,
但是, 如果把多看一行和记录虚索引结合起来可以吗?需要向后搜索, 效率有所提高, 但是不会太高
如果用二维数组虚索引+记录下标呢, 然后根据二维数组地址连续性, 用偏移量得到反查结果,不行, 但是用静态链表法可以, 但是花销会很大
最终妥协了, 暴力吧.反正是辅助函数
****************************************************************/
