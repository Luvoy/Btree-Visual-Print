# Btree Visual Print

## Introduction
二叉树动态可视化打印，纯C实现。这么基础的东西网上竟然没有，于是我造了一个，这对于学习数据结构和算法来说还是有用的。

Print binary tree visually, using pure c. Since I haven't find similar programs on the Internet, I build one, which is quite convenient for learning data structures and algorithms.

## Preview
```c
  _(5)___
  |      |
 (2)  _(10)__________
      |              |
     (5)       ____(20)
               |
            _(17)_
            |     |
          (12)  (19)

// 下面这颗树可能有点宽 :-)
// below tree is a little wider :-)


                    _______________________________(Amy)_____________________
                    |                                                        |
               ___(Bob)____________________                            ___(Carl)___________________
               |                           |                           |                           |
    ________(David)            ________(Edison)__                ___(Frank)             _______(George)__
    |                          |                 |               |                      |                |
 (Henry)__               ___(Iris)__          (Jack)       ___(Kevin)             ___(Laura)_         (Mark)_
          |              |          |                      |                      |          |               |
       (Neil)         (Oscar)    (Peter)               (Queena)                (Robin)     (Sam)           (Tom)
```


## Usage

### For macro:
你的二叉树结点必须看起来像下面的定义，但不必完全一致。

Your binary tree's node should look like this, but it needn't be the very same.

```
typedef struct node
{
    char data;
    struct node *lchild, *rchild;
} BTNode, *BTree;
```
然后 then
```
#include "btree_visual_print_macro.h"

BTree t1;
/* add some contents to t1 */
BTREE_VISUAL_PRINT(BTree, t1, lchild, rchild, data, "%c", stdout);
/* where %c is the format str of then element in each node */
```

### For function:
你的二叉树结点必须使用下面的定义，或在进入```.h```文件，在函数中替换成你的定义。

Your binary tree's node should be exact this, or replace the definition in ```.h``` file with yours.

```
typedef struct node
{
    char data;
    struct node *lchild, *rchild;
} BTNode, *BTree;
```
然后 then
```
#include "btree_visual_print.h"

BTree t1;
/* add some contents to t1 */
btree_visual_print(t, "%d", stdout);
/* where %d is the format str of then element in each node */

```
### Other things
- 支持```char```、```int```、```char*```等基本类型 Supported are: ```char```、```int```、```char*```, among other basic types
- 其他类型（如指针、结构体）可能要修改两行代码，已用TODO标出 For other types such as ptr or struct, two lines of code should be altered, which are marked as TODO in the code

## Theory
- 中序遍历计算横坐标 In-order to calculate the horizontal coordinate
- 层序遍历计算纵坐标 Layer-order to calculate the vertical coordinate
- ```sprintf```预先求字符串长度，而不是```itoa``` Use ```sprintf``` instead of ```itoa``` to pre-calculate str length
- 剩下的自己看吧，懒得写了╮(╯-╰)╭ Read the remnant by yourself, as I'm so lazy ╮(╯-╰)╭
