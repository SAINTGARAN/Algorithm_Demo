#include <stdio.h>
#include <stdlib.h>

/*
 * 算法思想
 * 设当前节点为cur
 * 1、如果cur没有左孩子：
 *      说明左边已经处理完了，或者根本没有左边
 *      访问cur，然后将cur移动到右孩子：cur = cur->right
 * 2、如果cur有左孩子
 *      寻找cur在中序遍历下的前驱节点pre(即左子树中最右侧的节点)
 *          情况A:pre->right为空 （说明这是第一次访问cur）
 *                将pre->right指向cur
 *                继续向左走：cur = cur->left
 *          情况B：pre->right指向cur（说明是第二次访问cur，左子树已遍历完）
 *                 将pre->right置为NULL
 *                 访问cur
 *                 转向右子树：cur = cur->right
 * 3、cur为空，停止遍历
 *
 * 先序：
 * 1、只访问一次的节点直接打印。访问两次的节点，只打印第一次
 * 中序：
 * 1、只访问一次的节点直接打印。访问两次的节点，只打印第二次的
 */

typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

// 创建新节点的辅助函数
struct TreeNode *newNode(int data) {
    struct TreeNode *node = (struct TreeNode *) malloc(sizeof(struct TreeNode));
    node->val = data;
    node->left = node->right = NULL;
    return node;
}

// 中序遍历
static void morrisInorder(TreeNode *root) {
    TreeNode *curr = root, *pre;

    while (curr != NULL) {
        if (curr->left == NULL) {
            // 情况1：没有左孩子，访问当前节点并跳到右边
            printf("%d ", curr->val);
            curr = curr->right;
        } else {
            // 情况二：有做孩子，寻找左孩子的中序前驱节点
            pre = curr->left;
            while (pre->right != NULL && pre->right != curr) {
                pre = pre->right;
            }

            if (pre->right == NULL) {
                // 第一次来到cur中序前驱节点
                // 第一次访问，建立线索，然后进入左子树
                pre->right = curr;
                curr = curr->left;
            } else {
                // 第二次访问：说明左子树已处理完，恢复树结构，访问当前节点，进入右子树
                pre->right = NULL;
                printf("%d ", curr->val);
                curr = curr->right;
            }
        }
    }
}

void morrisPreorder(struct TreeNode *root) {
    struct TreeNode *curr = root, *pre;

    while (curr != NULL) {
        if (curr->left == NULL) {
            printf("%d ", curr->val);
            curr = curr->right;
        } else {
            pre = curr->left;
            while (pre->right != NULL && pre->right != curr) {
                pre = pre->right;
            }

            if (pre->right == NULL) {
                // 第一次访问：建立线索前就打印（根节点先打印）
                printf("%d ", curr->val);
                pre->right = curr;
                curr = curr->left;
            } else {
                // 第二次访问：只需拆除线索
                pre->right = NULL;
                curr = curr->right;
            }
        }
    }
}

// 后序遍历 逆序打印左树左边界，逆序打印整棵树右边界
/*
*
后序遍历的顺序是 左 -> 右 -> 根。
在 Morris 遍历中，当我们从左子树返回根节点（即第二次访问某个节点 cur）时，左子树的右边界上的节点，如果倒序输出，正好符合后序遍历中“右 -> 根”的局部顺序。
具体步骤：
建立一个临时节点 dummy，让 dummy->left = root。将 cur 置为 dummy。
如果 cur 有左孩子：
找到前驱节点 pre（左子树最右节点）。
如果 pre->right == NULL：建立线索，pre->right = cur，cur = cur->left。
如果 pre->right == cur：关键步骤。
拆除线索：pre->right = NULL。
倒序输出从 cur->left 到 pre 这条路径上的所有节点。
cur = cur->right。
如果 cur 没有左孩子：cur = cur->right
 */

struct Node {
    int data;
    struct Node *left, *right;
};

// 辅助函数：反转从 from 到 to 的右孩子路径
void reversePath(struct Node *from, struct Node *to) {
    if (from == to) return;
    struct Node *x = from, *y = from->right, *z;
    while (x != to) {
        z = y->right;
        y->right = x;
        x = y;
        y = z;
    }
}

// 辅助函数：倒序打印并恢复路径
void printPathReverse(struct Node *from, struct Node *to) {
    reversePath(from, to);
    struct Node *p = to;
    while (1) {
        printf("%d ", p->data);
        if (p == from) break;
        p = p->right;
    }
    reversePath(to, from); // 恢复原样
}

void morrisPostorder(struct Node *root) {
    if (root == NULL) return;

    // 创建虚拟根节点 dummy，指向真正的 root
    struct Node *dummy = newNode(0);
    dummy->left = root;

    struct Node *curr = dummy, *pre;

    while (curr != NULL) {
        if (curr->left == NULL) {
            curr = curr->right;
        } else {
            pre = curr->left;
            while (pre->right != NULL && pre->right != curr) {
                pre = pre->right;
            }

            if (pre->right == NULL) {
                // 第一次访问，建立线索
                pre->right = curr;
                curr = curr->left;
            } else {
                // 第二次访问，左子树已完，倒序打印左子树的右边界
                pre->right = NULL;
                printPathReverse(curr->left, pre);
                curr = curr->right;
            }
        }
    }
    free(dummy);
}

int main() {
    /* 构造如下二叉树:
            1
          /   \
         2     3
        / \
       4   5
    */
    struct TreeNode *root = newNode(1);
    root->left = newNode(2);
    root->right = newNode(3);
    root->left->left = newNode(4);
    root->left->right = newNode(5);

    printf("Morris 中序遍历: ");
    morrisInorder(root);
    printf("\n");

    printf("Morris 前序遍历: ");
    morrisPreorder(root);
    printf("\n");

    return 0;
}
