#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define abs(x) ((x)<0 ? -(x) : (x))

typedef struct node {
    int data;
    struct node *left;
    struct node *right;
} node;

// 平衡二叉树：每一颗子树的左右高度差小于1
int getHeight(node *root) {
    if (root == NULL) return 0;
    int lh = getHeight(root->left);
    if (lh == -1) return -1;

    int rh = getHeight(root->right);
    if (rh == -1) return -1;

    if (abs(lh - rh) > 1) return -1;

    return (lh > rh ? lh : rh) + 1;
}

bool isBalanced(node *root) {
    return getHeight(root) != -1;
}

/*
* 执行过程
        1
       / \
      2   3
     /
    4
process(4) → {true,  1}
process(2) → left={true,1}, right={true,0}, 差=1 → {true,  2}
process(3) → {true,  1}
process(1) → left={true,2}, right={true,1}, 差=1 → {true,  3}  ✅
 */

typedef struct {
    bool isBalanced;
    int height;
} ReturnType;

ReturnType process(node *root) {
    if (root == NULL) {
        ReturnType base = {true, 0};
        return base;
    }
    ReturnType left = process(root->left);
    ReturnType right = process(root->right);

    // 计算当前节点的高度
    int height = ((left.height > right.height) ? left.height : right.height) + 1;

    // 当前是否平衡：左右都平衡，且高度差不超过1
    bool isBalanced = left.isBalanced && right.isBalanced && abs(left.height - right.height) <= 1;
    ReturnType ret = {isBalanced, height};
    return ret;
}

static bool _isBalanced(node *root) {
    return process(root)->isBalance();
}
