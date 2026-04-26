#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"

typedef struct BinaryTreeNode {
    int data;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
} BinaryTreeNode;

typedef struct {
    int num;
    int height;
} ReturnType;

/*
 * 判断是否是满二叉树
 * 公式：节点总数 = 2^height - 1;
 * 思路：
 * 1、左子树是满二叉树
 * 2、右子树是满二叉树
 */

static ReturnType process(BinaryTreeNode *root) {
    if (root == NULL) {
        ReturnType base = {0, 0};
        return base;
    }

    ReturnType left = process(root->left);
    ReturnType right = process(root->right);

    // 收集信息 子树节点个数和高度
    int height = ((left.height > right.height) ? left.height : right.height) + 1;
    int num = left.num + right.num + 1;

    return (ReturnType){num, height};
}

static bool isFull(BinaryTreeNode *root) {
    if (root == NULL) {
        return true;
    }

    ReturnType result = process(root);

    return result.num == (1 << result.height) - 1;
}

// 判断是否是满二叉树 非递归写法
// 满二叉树：每个节点要么是0个节点，要么是2两个节点
int is_full_binary_tree(BinaryTreeNode *root) {
    if (root == NULL) {
        return 1;
    }
    Queue *q = QUEUE_CREATE(BinaryTreeNode*);
    ENQUEUE(q, root);

    while (!QUEUE_EMPTY(q)) {
        BinaryTreeNode *node = DEQUEUE(q, BinaryTreeNode*, node);

        int has_left = (node->left != NULL);
        int has_right = (node->right != NULL);

        if (has_left != has_right) {
            QUEUE_DESTROY(q);
            return 0;
        }
        // 能走到这里，has_left 和 has_right 一定相等
        // 要么都是 1，要么都是 0，判断一个就够了
        if (has_left) {
            ENQUEUE(q, node->left);
        }
        if (has_right) {
            ENQUEUE(q, node->right);
        }
    }
    QUEUE_DESTROY(q);
    return 1;
}

/*
 * 完美二叉树：所有叶子节点都在同一层，每层节点数达到最大
 */
// 层序遍历，一层处理完看是否出现了叶子节点
int is_perfect_binary_tree(BinaryTreeNode *root) {
    if (root == NULL) {
        return 1;
    }
    Queue *q = QUEUE_CREATE(BinaryTreeNode*);
    ENQUEUE(q, root);

    while (!QUEUE_EMPTY(q)) {
        int level_size = QUEUE_SIZE(q); // 当前层的节点数
        int has_leaf = 0;

        for (int i = 0; i < level_size; i++) {
            BinaryTreeNode *node = DEQUEUE(q, BinaryTreeNode*, node);
            int has_left = node->left != NULL;
            int has_right = node->right != NULL;

            if (has_left != has_right) {
                QUEUE_DESTROY(q);
                return 0;
            }
            // 走到这里， 左右都是对称的
            if (!has_leaf) {
                has_leaf = 1;
            } else {
                if (has_leaf) {
                    ENQUEUE(q, node->left);
                    return 0;
                }
                ENQUEUE(q, node->right);
                ENQUEUE(q, node->left);
            }
        }

        // 这一层出现了叶子，说明是最后一层， 队列应该为空
        if (has_leaf && !QUEUE_EMPTY(q)) {
            QUEUE_DESTROY(q);
            return 0;
        }
    }
    QUEUE_DESTROY(q);
    return 1;
}
