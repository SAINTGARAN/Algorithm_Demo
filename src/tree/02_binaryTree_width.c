#include <stdio.h>
#include <stdlib.h>

typedef struct BinaryTree {
    struct BinaryTree *left;
    struct BinaryTree *right;
    int val;
} BinaryTree;

#define MAX_QUEUE 100

// 宽度遍历用队列 头 左 右
static void bfs(BinaryTree *root) {
    if (root == NULL) {
        return;
    }
    BinaryTree *queue[MAX_QUEUE]; // 数组模拟队列
    int front = 0;
    int rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        BinaryTree *cur = queue[front++];

        // 处理当前节点
        printf("%d ", cur->val);
        // 把左孩子先入队（保证左边先处理）
        if (cur->left != NULL) {
            queue[rear++] = cur->left;
        }

        // 再把右孩子入队列
        if (cur->right != NULL) {
            queue[rear++] = cur->right;
        }
    }
    printf("\n");
}

BinaryTree *newNode(int val) {
    BinaryTree *node = (BinaryTree *) malloc(sizeof(BinaryTree));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 求一个树的最大宽度
static int maxWidth(BinaryTree *root) {
    if (root == NULL) {
        return 0;
    }
    BinaryTree *queue[MAX_QUEUE];
    int front = 0;
    int rear = 0; // rear指向下一个可以入队的位置
    queue[rear++] = root;

    int max = 0;
    while (front < rear) {
        int levelSize = rear - front;

        if (levelSize > max) {
            max = levelSize;
        }
        // 只处理这层的levelSize个节点
        // 处理的同时把下一层的节点全部入队
        for (int i = 0; i < levelSize; i++) {
            BinaryTree *cur = queue[front++];
            printf("%d ", cur->val);
            if (cur->left != NULL) {
                queue[rear++] = cur->left;
            }
            if (cur->right != NULL) {
                queue[rear++] = cur->right;
            }
        }
    }
    return max;
}

int main() {
    /*
     * 构造这棵树：
     *         1
     *        / \
     *       2   3
     *      / \ / \
     *     4  5 6  7
     *
     * 每层宽度：1, 2, 4
     * 最大宽度应该是 4
     */
    BinaryTree *root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);
    root->right->left = newNode(6);
    root->right->right = newNode(7);

    printf("测试1 - 满二叉树，最大宽度应为4: %d\n", maxWidth(root));

    /*
     * 构造这棵树：
     *         1
     *        /
     *       2
     *      /
     *     3
     *
     * 每层宽度：1, 1, 1
     * 最大宽度应该是 1
     */
    BinaryTree *root2 = newNode(1);
    root2->left = newNode(2);
    root2->left->left = newNode(3);

    printf("测试2 - 链状树，最大宽度应为1: %d\n", maxWidth(root2));

    /*
     * 构造这棵树：
     *         1
     *        / \
     *       2   3
     *        \
     *         4
     *          \
     *           5
     *
     * 每层宽度：1, 2, 1, 1
     * 最大宽度应该是 2
     */
    BinaryTree *root3 = newNode(1);
    root3->left = newNode(2);
    root3->right = newNode(3);
    root3->left->right = newNode(4);
    root3->left->right->right = newNode(5);

    printf("测试3 - 不规则树，最大宽度应为2: %d\n", maxWidth(root3));

    // 空树
    printf("测试4 - 空树，最大宽度应为0: %d\n", maxWidth(NULL));

    // 只有根节点
    BinaryTree *root5 = newNode(1);
    printf("测试5 - 只有根节点，最大宽度应为1: %d\n", maxWidth(root5));

    return 0;
}
