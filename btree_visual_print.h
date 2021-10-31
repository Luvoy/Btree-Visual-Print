/**
 * @file btree_visual_print.h
 * @brief 可视化动态打印二叉树
 * @version 1.0
 * @date 2021-09-27
 *
 * @copyright Copyright (c) 2021
 *
 */

// std=C99
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 需要类似下方的结构体
 *
 */

#define NODE_EXAMPLE

#ifdef NODE_EXAMPLE
typedef struct node
{
    // something else
    char data;                    //也可以是 int data, char *data, 等等
                                  //但是如果用int *, char*, char**, struct A *, void* 这种再用指针另指元素, 下面函数有两行要修改, 已用TODO标出
    struct node *lchild, *rchild; //如果用其他的名字, 下面函数中替换即可
} BTNode, *BTree;                 //如果用其他的名字, 下面函数中替换即可
#endif

/**
 * @brief
 *
 * @param root 根节点地址
 * @param elem_fmt 元素打印时的格式字符, 比如char用%c, int用%d, 字符串用%s等等
 * @param fp 要输出的地方, 比如stdout, 又或者某个文件指针
 * @example
 *      btree_visual_print(t, "%c", stdout);
 *
 *      FILE * fp = fopen("out.txt", "w");
 *      btree_visual_print(t, "%c", fp);
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
void btree_visual_print(const BTree root, const char *elem_fmt, FILE *fp)
{
    if (root == NULL)
        return;
    const int _MAX_STR_LEN = 128;        //单个元素打印成str最大长度
    const int _MAX_NODE_NUM = 1024;      //结点最大个数
    const char horiz_conj_char = '_';    //连接横线
    const char vert_conj_char = '|';     //竖线
    const char left_bracket_char = '(';  //每个元素左边的小括号, 你也可以换成你喜欢的
    const char right_bracket_char = ')'; //元素右边的小括号

    //思路: 层序遍历算深度, 中序遍历算横坐标, 用结构体数组把这些信息存好再统一打印
    typedef struct _node_print_info
    {
        BTree address; //二次遍历时的唯一区分标志，即key //用hash表将地址映射会更省时间空间
        int str_len;
        int depth;
        int left_margin; // horizontal coordintate
    } _Info;

    BTree tree_deque[_MAX_NODE_NUM];
    int depth_queue[_MAX_NODE_NUM]; //记录深度的队列
    _Info *info_p_arr[_MAX_NODE_NUM];
    int node_count = 0;
    int front = -1, rear = -1;
    BTree p;
    _Info *info_p;
    char temp_str[_MAX_STR_LEN];

    //用队列统计结点的深度信息, 顺便统计每个结点数据打印时的长度
    tree_deque[++rear] = root;
    depth_queue[rear] = 1;
    while (front < rear)
    {
        p = tree_deque[++front];
        info_p = (_Info *)calloc(1, sizeof(_Info));
        info_p->address = p; //记录地址
        memset(temp_str, 0, _MAX_STR_LEN);
        sprintf(temp_str, elem_fmt, p->data);   // TODO: 注意
        info_p->str_len = strlen(temp_str) + 2; //计算打印后的元素长度
        info_p->depth = depth_queue[front];
        info_p_arr[node_count++] = info_p;

        if (p->lchild != NULL)
        {
            tree_deque[++rear] = p->lchild;
            depth_queue[rear] = depth_queue[front] + 1;
        }
        if (p->rchild != NULL)
        {
            tree_deque[++rear] = p->rchild;
            depth_queue[rear] = depth_queue[front] + 1;
        }
    }

    //接下来统计横坐标, 用中序遍历
    p = root;
    int top = -1, i;
    int horizontal_accumu_cache = 0; //横坐标累计长度
    do
    {
        while (p != NULL)
        {
            tree_deque[++top] = p;
            p = p->lchild;
        }
        p = tree_deque[top--];
        for (i = 0; i < node_count; ++i)
        { //这里用hash表的优势就体现出来了
            if (info_p_arr[i]->address == p)
            { //计算横坐标
                info_p_arr[i]->left_margin = horizontal_accumu_cache;
                horizontal_accumu_cache += info_p_arr[i]->str_len - 1; //减一可以重叠一个括号,更紧凑一点点
            }
        }
        p = p->rchild;
    } while (!(p == NULL && top == -1));

    //接下来开始打印
    int horiz_left_start, horiz_right_end, cursor, j, k, cur_depth = 1, end_flag = 0;
    int vert_index_arr[_MAX_NODE_NUM]; //偶数行的竖线存储数组
    i = 0;
    while (i < node_count)
    {
        k = -1;
        cursor = 0;
        while (info_p_arr[i]->depth == cur_depth)
        {
            //打印左边
            p = info_p_arr[i]->address;
            if (p->lchild != NULL)
            { //有左孩子说明有横线要打印
                for (j = 0; j < node_count; ++j)
                { //暴力查找
                    if (info_p_arr[j]->address == p->lchild)
                    {
                        horiz_left_start = info_p_arr[j]->left_margin + info_p_arr[j]->str_len / 2;
                        vert_index_arr[++k] = horiz_left_start;
                        break;
                    }
                }
                for (; cursor < horiz_left_start; ++cursor)
                    fprintf(fp, " ");
                for (; cursor < info_p_arr[i]->left_margin; ++cursor)
                    fprintf(fp, "%c", horiz_conj_char);
            }
            else
            { //没有左孩子全打印空格即可
                for (; cursor < info_p_arr[i]->left_margin; ++cursor)
                    fprintf(fp, " ");
            }

            //打印元素
            fprintf(fp, "%c", left_bracket_char);
            fprintf(fp, elem_fmt, p->data); //这里要注意TODO:
            fprintf(fp, "%c", right_bracket_char);
            cursor += info_p_arr[i]->str_len;

            //打印右边
            if (p->rchild != NULL)
            {
                for (j = 0; j < node_count; ++j)
                {
                    if (info_p_arr[j]->address == p->rchild)
                    {
                        horiz_right_end = info_p_arr[j]->left_margin + info_p_arr[j]->str_len / 2;
                        vert_index_arr[++k] = horiz_right_end;
                        break;
                    }
                }
                for (; cursor < horiz_right_end; ++cursor)
                    fprintf(fp, "%c", horiz_conj_char);
            } //右边没有else ,因为只考虑横线即可, 空白算到同层下一个元素左边

            if (++i >= node_count)
            {
                end_flag = 1; //打印完最后一个元素, 下一行不需要竖线了,提前结束
                break;
            }
        }
        fprintf(fp, "\n");

        //打印竖线
        if (!end_flag)
        {
            cursor = 0;
            for (j = 0; j <= k; ++j)
            {
                for (; cursor < vert_index_arr[j]; ++cursor)
                    fprintf(fp, " ");
                fprintf(fp, "%c", vert_conj_char);
                cursor++;
            }
            fprintf(fp, "\n");
        }
        cur_depth++;
    }
    for (i = 0; i < node_count; ++i)
        free(info_p_arr[i]); //释放空间
    return;
}
