#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * 二叉树的序列化及反序列化
 * 就是将内存里的一颗树如何变成字符串的形式， 又如何从字符串形式变成内存里的一颗树
 */

typedef struct TreeNode {
    struct TreeNode *left;
    struct TreeNode *right;
    int val;
} TreeNode;

TreeNode *createTreeNode(int val) {
    TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// 序列化
// 序列化：先序遍历这棵树，把每个节点的值写成字符串，NULL 节点写成 #，节点之间用空格分隔。
void serializeTree(TreeNode *root, char *buf, int *pos) {
    if (root == NULL) {
        *pos += sprintf(buf + *pos, "# ");
        return;
    }
    *pos += sprintf(buf + *pos, "%d", root->val);
    serializeTree(root->left, buf, pos);
    serializeTree(root->right, buf, pos);
}

// 反序列化
// 反序列化：按同样的先序顺序读回来，读到 # 就返回 NULL，读到数字就建一个节点，然后递归建左子树、右子树。
TreeNode *deserialize_helper(char **ptr) {
    /* 跳过空格 */
    while (**ptr == ' ') (*ptr)++;

    if (**ptr == '\0') return NULL;

    /* 读到#, 空节点 */
    if (**ptr == '#') {
        (*ptr)++;
        return NULL;
    }

    /* 读整数 （含负数）*/
    int sign = 1;
    if (**ptr == '-') {
        sign = -1;
        (*ptr)++;
    }

    int val = 0;
    while (**ptr >= '0' && **ptr <= '9') {
        val = (val * 10) + (**ptr - '0');
        (*ptr)++;
    }
    val *= sign;
    TreeNode *root = createTreeNode(val);
    root->left = deserialize_helper(ptr);
    root->right = deserialize_helper(ptr);
    return root;
}

TreeNode *deserialize(char *buf) {
    char *ptr = buf;
    return deserialize_helper(&ptr);
}
