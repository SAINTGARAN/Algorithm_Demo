#include <stdio.h>
#include <stdlib.h>

/*每个节点的左孩子永远是下
每个节点的右孩子永远是上
按中序遍历输出就是折痕序列*/

// level：当前来到的层数， n:一共的层数
void fold(int level, int n, int is_down) {
    if (level > n) return;

    // 左子树 永远是下
    fold(level + 1, n, 1);

    printf("%s", is_down ? "下" : "上");

    fold(level + 1, n, 0);
}
