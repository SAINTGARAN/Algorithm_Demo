#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
/*
 * 递归写法
 * 递归思路：
 * 1、左树是搜索二叉树
 * 2、右数是搜索二叉树
 * 3、左数的最大值 小于x ; 右树的最小值 大于 x
 */
typedef struct BinaryTreeNode {
    int data;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
} BinaryTreeNode;

typedef struct {
    bool isBST;
    int max;
    int min;
} ReturnType;

BinaryTreeNode *newNode(int data) {
    BinaryTreeNode *n = malloc(sizeof(BinaryTreeNode));
    n->data = data;
    n->left = NULL;
    n->right = NULL;
    return n;
}

ReturnType process(BinaryTreeNode *root) {
    if (root == NULL) {
        // 空树 max/min 用边界值占位
        ReturnType base = {true, INT_MIN, INT_MAX};
        return base;
    }

    ReturnType left = process(root->left);
    ReturnType right = process(root->right);

    int max = root->data;
    if (left.max > max) max = left.max;
    if (right.max > max) max = right.max;

    int min = root->data;
    if (left.min < min) min = left.min;
    if (right.min < min) min = right.min;
    bool isBST = left.isBST && right.isBST
                 && left.max < root->data // 左树所有节点 < 根
                 && right.min > root->data; // 右树所有节点 > 根

    ReturnType result = {isBST, max, min};
    return result;
}

bool checkBST(BinaryTreeNode *root) {
    return process(root).isBST;
}

int main() {
    BinaryTreeNode *root;

    // ✅ 用例1：标准BST
    //        5
    //       / \
    //      3   7
    //     / \ / \
    //    2  4 6  8
    root = newNode(5);
    root->left = newNode(3);
    root->right = newNode(7);
    root->left->left = newNode(2);
    root->left->right = newNode(4);
    root->right->left = newNode(6);
    root->right->right = newNode(8);
    printf("用例1（标准BST）:    %s\n", checkBST(root) ? "✅ true" : "❌ false");

    // ❌ 用例2：漏网之鱼
    //        10
    //       /  \
    //      5    15
    //     / \
    //    3   12    ← 12 在左子树但 > 10
    root = newNode(10);
    root->left = newNode(5);
    root->right = newNode(15);
    root->left->left = newNode(3);
    root->left->right = newNode(12);
    printf("用例2（漏网之鱼）:   %s\n", checkBST(root) ? "✅ true" : "❌ false");

    // ✅ 用例3：单节点
    root = newNode(1);
    printf("用例3（单节点）:     %s\n", checkBST(root) ? "✅ true" : "❌ false");

    // ✅ 用例4：空树
    printf("用例4（空树）:       %s\n", checkBST(NULL) ? "✅ true" : "❌ false");

    // ❌ 用例5：右子树含小于根的值
    //      10
    //     /  \
    //    5    20
    //         /
    //         8    ← 8 在右子树但 < 10
    root = newNode(10);
    root->left = newNode(5);
    root->right = newNode(20);
    root->right->left = newNode(8);
    printf("用例5（右树含小值）: %s\n", checkBST(root) ? "✅ true" : "❌ false");

    return 0;
}
