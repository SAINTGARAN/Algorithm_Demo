#include <stdio.h>
#include <stdlib.h>
#include "stack_oop.h"

typedef struct BinaryTreeNode {
    struct BinaryTreeNode *left;
    struct BinaryTreeNode *right;
    int val;
} BinaryTreeNode;

static BinaryTreeNode *newNode(int val) {
    BinaryTreeNode *n = calloc(1, sizeof(BinaryTreeNode));
    n->val = val;
    return n;
}


// 先序遍历递归实现
void prevorder_recursion(BinaryTreeNode *root) {
    if (root == NULL) { return; }
    printf("%d ", root->val);
    prevorder_recursion(root->left);
    prevorder_recursion(root->right);
}

// 先序遍历非递归实现  利用栈
void prevorder(BinaryTreeNode *root) {
    if (root != NULL) {
        Stack *s = stackCreate();
        s->push(s, root);
        while (!s->empty(s)) {
            root = s->pop(s);
            printf("%d ", root->val);
            if (root->right != NULL) {
                s->push(s, root->right);
            }
            if (root->left != NULL) {
                s->push(s, root->left);
            }
        }
        s->destroy(s);
    }
}

// morrisPreorder
void morrisPreorder(BinaryTreeNode *root) {
    if (root == NULL) { return; }
    BinaryTreeNode *cur;
    while (cur) {
        if (cur->left == NULL) {
            // 没有左子树：直接打印。走右边
            printf("%d ", cur->val);
            cur = cur->right;
        } else {
            // 找出左子树的最右节点
            BinaryTreeNode *pred = cur->left;
            while (pred->right && pred->right != cur) {
                pred = pred->right;
            }

            if (pred->right == NULL) {
                // 第一次到达：建立线索， 打印，走左边
                pred->right = cur;
                printf("%d ", cur->val);
                cur = cur->left;
            } else {
                // 第二次到达，说明左子树走完了，断线索，走右边
                pred->right = NULL;
                cur = cur->right;
            }
        }
    }
}

// 中序遍历
static void inOrderUnRecursion(BinaryTreeNode *root) {
    if (root == NULL) { return; }
    Stack *s = stackCreate();
    while (!s->empty(s) || root != NULL) {
        if (root != NULL) {
            s->push(s, root);
            root = root->left; // 左边界进栈
        } else {
            root = s->pop(s);
            printf("%d ", root->val);
            root = root->right;
        }
    }
    printf("\n");
    s->destroy(s);
}

// 中序遍历递归写法
static void inOrderRecursion(BinaryTreeNode *root) {
    if (!root) { return; }
    inOrderRecursion(root->left); // 先走左子树
    printf("%d ", root->val); // 在处理当前节点
    inOrderRecursion(root->right); // 最后走右子树
}

// 后续遍历  栈 + 辅助栈
/**
 * 实现思路：
 * 先序是根左右 将它改成根右左， 在整体反转就是左右根
 */
static void posOrderUnRecursion(BinaryTreeNode *root) {
    BinaryTreeNode *cur;
    if (root != NULL) {
        Stack *s1 = stackCreate();
        Stack *s2 = stackCreate();
        s1->push(s1, root);
        while (!s1->empty(s1)) {
            cur = (BinaryTreeNode *) s1->pop(s1);
            s2->push(s2, cur);
            if (cur->left != NULL) {
                s1->push(s1, cur->left);
            }
            if (cur->right != NULL) {
                s1->push(s1, cur->right);
            }
        }
        while (!s2->empty(s2)) {
            printf("%d ", ((BinaryTreeNode *) s2->pop(s2))->val);
        }
        s1->destroy(s1);
        s2->destroy(s2);
    }
}

// 后序遍历
void posOrder(BinaryTreeNode *root) {
    if (root == NULL) { return; }
    BinaryTreeNode **node1;
    BinaryTreeNode **node2;
    int top1 = -1, top2 = -1;
    node1[++top1] = root;
    while (top1 >= 0) {
        BinaryTreeNode *cur = node1[top1--];
        node2[++top2] = cur;
        // 先压左，再压右
        if (cur->left != NULL) {
            node1[++top1] = cur->left;
        }
        if (cur->right != NULL) {
            node1[++top1] = cur->right;
        }
    }
    while (top2 >= 0) {
        printf("%d ", node2[top2--]->val);
    }
    printf("\n");
}

// 判断一颗树是否是搜索二叉树
/*
 * 搜索二叉树
 * 对于一个子树来说，左节点比根节点小，右节点比根节点大
 * 解法：
 * 中序遍历 左根右 如果每次遍历的时候，节点总是升序的，那就是一颗二叉搜索树
 */
static int prevValue = INT_MIN;
bool isBST(BinaryTreeNode *root) {
    if (root == NULL) { return true; }
    bool leftIsBST = isBST(root->left);
    if (!leftIsBST) {
        return false;
    }
    if (root->val <= prevValue) {
        return false;
    } else {
        prevValue = root->val;
    }
    return isBST(root->right);
}

// 非递归方式实现如何判断是一个树是否是搜索二叉树
bool checkBST(BinaryTreeNode *root) {
    if (root == NULL) { return true; }
    int prev_value = INT_MIN;
    BinaryTreeNode *queue[1000];
    int front = 0, rear = 0;
    while (front < rear) {
        if (root->left != NULL) {
        }
    }
}

static void test_posOrder() {
    /*
     *         1
     *        / \
     *       2   3
     *      / \   \
     *     4   5   6
     *
     *  后序: 4 5 2 6 3 1
     */
    BinaryTreeNode *n1 = newNode(1);
    BinaryTreeNode *n2 = newNode(2);
    BinaryTreeNode *n3 = newNode(3);
    BinaryTreeNode *n4 = newNode(4);
    BinaryTreeNode *n5 = newNode(5);
    BinaryTreeNode *n6 = newNode(6);

    n1->left = n2;
    n1->right = n3;
    n2->left = n4;
    n2->right = n5;
    n3->right = n6;

    printf("Test 1 - 普通树: ");
    posOrderUnRecursion(n1); // 期望: 4 5 2 6 3 1
    printf("\n");

    /*
     *   1 → 2 → 3 (全部右斜)
     *   后序: 3 2 1
     */
    BinaryTreeNode *r1 = newNode(1);
    BinaryTreeNode *r2 = newNode(2);
    BinaryTreeNode *r3 = newNode(3);
    r1->right = r2;
    r2->right = r3;

    printf("Test 2 - 右斜树: ");
    posOrderUnRecursion(r1); // 期望: 3 2 1
    printf("\n");

    /*
     *       1
     *      /
     *     2
     *    /
     *   3          (全部左斜)
     *   后序: 3 2 1
     */
    BinaryTreeNode *l1 = newNode(1);
    BinaryTreeNode *l2 = newNode(2);
    BinaryTreeNode *l3 = newNode(3);
    l1->left = l2;
    l2->left = l3;

    printf("Test 3 - 左斜树: ");
    posOrderUnRecursion(l1); // 期望: 3 2 1
    printf("\n");

    // 单节点
    BinaryTreeNode *single = newNode(42);
    printf("Test 4 - 单节点: ");
    posOrderUnRecursion(single); // 期望: 42
    printf("\n");

    // 空树
    printf("Test 5 - 空树:   ");
    posOrderUnRecursion(NULL); // 期望: 无输出
    printf("\n");
}

int main(void) {
    test_posOrder();
    return 0;
}
