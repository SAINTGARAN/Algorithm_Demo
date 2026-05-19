#include <stdio.h>
#define Max(a, b) ((a) > (b) ? (a) : (b))
/*
 * 从二叉树有的节点A出发，可以往上走，也可以往下走，但沿途的节点只能经过一次，到达节点B时路径上的节点个数叫做A到B的距离
 */

/*
 * 分析思路：
 * 1、头节点不参与
 *      最大距离来自左子树
 *      最大距离来自右子树
 * 2、头节点参数
 *      左树的高 + 1 + 右数的高
 */

typedef struct {
    int maxDistance;
    int height;
} Info;

typedef struct TreeNode {
    struct TreeNode *left;
    struct TreeNode *right;
    int val;
} TreeNode;

// 返回以X为头的整棵树，两个信息
static Info process(TreeNode *node) {
    if (node == NULL) {
        return (Info){0, 0};
    }
    Info leftInfo = process(node->left);
    Info rightInfo = process(node->right);

    int p1 = leftInfo.maxDistance;
    int p2 = rightInfo.maxDistance;
    int p3 = leftInfo.height + 1 + rightInfo.height;
    int maxDistance = Max(p3, Max(p1, p2));
    int height = Max(leftInfo.height, rightInfo.height) + 1;
}
