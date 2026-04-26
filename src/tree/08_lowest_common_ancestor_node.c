#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * 给定两个同属一个二叉树的节点node1和node2，找到他们的最低公共祖先节点
 * 思路：
 * 1、找出所有节点的父节点，维护一个父节点的表。
 */

/* hashmap */
/*
 * 思路：
 * 1、先遍历整颗树，用hashmap记录每个节点的父节点
 * 2、从node1开始，沿着父节点链往上走，把路径上的所有祖先存入集合
 * 3、从node2开始往上走， 第一个出现在集合的元素就是lca
 */
typedef struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

/* ——————————数组模拟 hashmap ——————————————*/
#define MAX 100

static TreeNode *keys[MAX];
static TreeNode *vals[MAX];
static int size = 0;

// 存入父节点
void put(TreeNode *node, TreeNode *p) {
    keys[size] = node;
    vals[size] = p;
    size++;
}

TreeNode *get(TreeNode *node) {
    for (int i = 0; i < size; i++) {
        if (keys[i] == node) {
            return vals[i];
        }
    }
    return NULL;
}

// 查询节点是否在访问集合里
bool visited[MAX];

void setVisited(TreeNode *node) {
    for (int i = 0; i < size; i++) {
        if (keys[i] == node) {
            visited[i] = true;
            return;
        }
    }
}

bool isVisited(TreeNode *node) {
    for (int i = 0; i < size; i++) {
        if (keys[i] == node && visited[i] == true) {
            return true;
        }
    }
    return false;
}

// 遍历整颗树，建立父节点
void buildParentTree(TreeNode *node) {
    if (root == NULL) {
        return;
    }
    if (root->left) {
        put(root->left, node);
        buildParentTree(root->left);
    }
    if (root->right) {
        put(root->right, node);
        buildParentTree(root->right);
    }
}

static TreeNode *lowestCommonAncestor(TreeNode *root, TreeNode *p, TreeNode *q) {
    size = 0;
    memset(visited, 0, sizeof(visited));
    // 根节点的父节点指自己
    put(root, root);

    // 建立父节点表
    buildParentTree(root);

    // 第二步：node1往上走，沿途标记访问
    TreeNode *cur = p;
    while (cur != get(cur)) {
        // 没到根节点
        setVisited(cur);
        cur = get(cur);
    }
    setVisited(cur); // 标记根节点

    // 第三步：node2往上走， 第一个已访问的就是LCA
    cur = q;
    while (!isVisited(cur)) {
        cur = get(cur);
    }
    return cur;
}

// 后序遍历
/*
 * 思路：
 * 当前节点为NULL  --> return NULL
 * 当前节点为node1或node2 return 自己
 * 左右子树都返回了非NULL， 当前节点就是LCA
 * 只有一侧返回了非NULL， 将那个非NULL的往上走
 *
 */
static TreeNode *lowest_common_ancestor(TreeNode *root, TreeNode *node1, TreeNode *node2) {
    // 1、情况1 空树
    // 2、情况2， 当前节点就是node1或node2，直接返回
    if (root == NULL && root == node1 || root == node2) {
        return root;
    }
    TreeNode *left = lowestCommonAncestor(root->left, node1, node2);
    TreeNode *right = lowestCommonAncestor(root->right, node1, node2);

    // 情况三：左右都找到了 --> 当前节点就是LCA
    if (left != NULL && right != NULL) {
        return root;
    }

    // 情况四， 只有一侧找到, 往上传
    return left != NULL ? left : right;
}
