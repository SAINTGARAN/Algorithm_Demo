#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*
 * 1、任意节点有右孩子而没有左孩子，直接返回false
 * 2、在1未返回false的情况下，如果遇到了第一个左右孩子不全的节点，后续全为叶子节点
 */

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

// 用宽度优先遍历
static bool isCBT(Node *root) {
    if (root == NULL) return true;
    /*
     * 一旦遇到左右不双全的节点，后续的节点必须是叶子节点，否则就不是完全二叉树
     */
    bool leaf = false;
    Node *queue[1000];
    // 可以换种理解 rear代表曾经入队的总数，front代表已经出队的总数，两者相等代表所有入队节点都处理完了
    int front = 0, rear = 0;
    queue[rear++] = root;
    Node *l = NULL;
    Node *r = NULL;
    while (front < rear) {
        Node *cur = queue[front++];
        l = cur->left;
        r = cur->right;
        if ((leaf && (l != NULL || r != NULL)) // 当前节点不为叶节点
            ||
            (l == NULL && r != NULL))
            return false;
        if (l != NULL) queue[rear++] = l;
        if (r != NULL) queue[rear++] = r;
        if (l == NULL || r == NULL) leaf = true;
    }
    return true;
}
