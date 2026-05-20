#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

/*
1. 什么是搜索二叉树 (BST)?
二叉搜索树（Binary Search Tree，简称 BST），也叫二叉排序树。它是一种特殊的二叉树，必须满足以下三个核心性质：
左子树上所有节点的值都小于它的根节点的值。
右子树上所有节点的值都大于它的根节点的值。
左右子树也分别为二叉搜索树。
 */

/*
* 2. 怎么判断一棵树是不是搜索二叉树？
判断 BST 有两种最主流的方法，一种利用中序遍历，一种利用递归范围约束。
方法 A：中序遍历（最直观的方法）
核心思想：
二叉搜索树有一个特性——它的中序遍历（左-根-右）结果一定是一个严格递增的序列。
步骤：
对树进行中序遍历。
在遍历过程中，记录上一个访问到的节点的值。
如果当前节点的值 <= 上一个节点的值，说明不是 BST。
优点：结合你之前问的 Morris 遍历，这种方法可以用O(1)的空间复杂度完成判断。
方法 B：递归判断（带范围限制）
这是一个容易踩坑的地方！
错误逻辑：只检查 root->left < root 且 root->right > root。
错误原因：即使每个节点都比它的直接孩子大/小，整棵树仍可能不是 BST（比如右子树里藏了一个比根节点还小的远房孙子）。
正确逻辑：每个节点都必须在一个确定的“上下界”之间。
根节点可以是在 (负无穷, 正无穷) 之间。
当你向左走时，更新上界：左孩子必须在 (当前下界, 当前节点值) 之间。

 *
 */


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
