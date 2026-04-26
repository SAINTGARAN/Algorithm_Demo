#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linux_list.h"
#include "stack.h"
#include "stack_oop.h"
// 题目：在二叉树中找到一个节点的后继节点

typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
} TreeNode;

// 方法一：经典二叉树结构，中序遍历放到数组或链表里面

typedef struct BinaryTreeNode {
    int data;
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
} BinaryTreeNode;

typedef struct list {
    BinaryTreeNode *node;
    struct list *next;
} list;

// 中序遍历
static void inorder(TreeNode *root) {
    if (!root) { return; }
    Stack *s = stackCreate();
    while (!s->empty(s) || root != NULL) {
        // 链表里面还有没处理节点 或者 还有节点未进栈,两个条件缺一不可
        if (root != NULL) {
            s->push(s, root); // 当前节点进栈
            root = root->left; // 一直往左走
        } else {
            // 走到最左边，无节点
            s->pop(s);
            printf("%d ", root->data);
            root = root->right;
        }
    }
    printf("\n");
    stackDestroy(s);
}

static list *inorder_insert_list(BinaryTreeNode *root) {
    if (!root) { return NULL; }
    Stack *s = stackCreate();
    list *head = NULL;
    list *tail = NULL;
    while (!s->empty(s) || root != NULL) {
        if (root != NULL) {
            s->push(s, root);
            root = root->left;
        } else {
            root = s->pop(s);
            // 创建新的链表节点
            list *new_node = (list *) malloc(sizeof(list));
            new_node->node = root;
            new_node->next = NULL;

            if (!head) {
                head = tail = new_node;
            } else {
                tail->next = new_node;
                tail = tail->next;
            }

            root = root->right;
        }
    }
    s->destroy(s);
    return head;
}

/*
 * 有右子树 --> 后继节点在右子树，取右子树最左边的节点
 * 无右子树 --> 一直往上走， 知道我是父节点的右孩子为止
 */

static TreeNode *getLeftMost(TreeNode *node) {
    if (node == NULL) { return NULL; }

    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

static TreeNode *inorder_successor(TreeNode *node) {
    if (!node) { return NULL; }

    if (node->right != NULL) {
        return getLeftMost(node->right);
    } else {
        // 无右树
        TreeNode *parent = node->parent;
        while (parent != NULL && parent->left != node) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }
}
