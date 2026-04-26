#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * 前缀树
 */
#define ALPHABET_SIZE 26

typedef struct TireNode {
    int pass; // 通过次数
    int end; // 作为终点次数
    struct TireNode *next[ALPHABET_SIZE]; // 26个子节点
} TrieNode;

static void insert(TrieNode *root, char *word) {
    if (!word) {
        return;
    }

    TrieNode *node = root; // 从根节点出发
    node->pass++; // 根节点pass + 1,表示又有一个字母经过根
    int index = 0;
    for (int i = 0; i < strlen(word); i++) {
        index = word[i] - 'a';
        if (!node->next[index]) {
            // 如果字母对应的子节点不存在，才创建
            // 将新建节点的地址存到父节点的next[index]的槽里面
            node->next[index] = (TrieNode *) calloc(1, sizeof(TrieNode));
        }
        node = node->next[index]; // 走到子节点
        node->pass++;
    }
    node->end++; // 走到子节点后，当前节点就是单次末尾， end + 1
}

/*
* // 调用
char *words[] = {"abc", "cde", "abcd", NULL};
insertAll(root, words, 0);
 */
static void insertAll(TrieNode *root, char **words, int index) {
    if (!words || !words[index]) return; // 遇到 NULL 停止

    // 插入当前单词（内层循环）
    TrieNode *node = root;
    node->pass++;
    for (int i = 0; i < strlen(words[index]); i++) {
        int idx = words[index][i] - 'a';
        if (!node->next[idx]) {
            node->next[idx] = (TrieNode *) calloc(1, sizeof(TrieNode));
        }
        node = node->next[idx];
        node->pass++;
    }
    node->end++;

    insertAll(root, words, index + 1); // 递归处理下一个单词
}

// 查询一个字符串加入了几次
static int search(TrieNode *root, char *word) {
    if (!word) return 0;
    TrieNode *node = root;
    for (int i = 0; i < strlen(word); i++) {
        int idx = word[i] - 'a';
        if (!node->next[idx]) {
            // 说明没加入过这个word
            return 0;
        }
        // 沿着节点往下走
        node = node->next[idx];
    }
    // 整个字符走完，节点的end值就是加入了几次
    return node->end;
}

// 所有加入的字符串中，有几个是以pre这个字符串作为前缀的
static int prefixNumber(TrieNode *root, char *word) {
    if (!word) return 0;
    TrieNode *node = root;
    for (int i = 0; i < strlen(word); i++) {
        int idx = word[i] - 'a';
        if (!node->next[idx]) {
            return 0;
        }
        node = node->next[idx];
    }
    return node->end;
}

static void freeSubTree(TrieNode *node) {
    if (!node) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freeSubTree(node->next[i]); // 递归释放所有子节点
    }
    free(node);
}


// 删除
static void delete(TrieNode *root, char *word) {
    if (!search(root, word)) return; // 单词不存在，直接返回

    TrieNode *node = root;
    node->pass--;

    for (int i = 0; i < strlen(word); i++) {
        int idx = word[i] - 'a';
        node->next[idx]->pass--;

        if (node->next[idx]->pass == 0) {
            // 没有任何单词经过这个节点了
            // 后面的子树全部释放，直接结束
            freeSubTree(node->next[idx]);
            node->next[idx] = NULL; // ⚠️ 必须置NULL，否则是野指针
            return;
        }

        node = node->next[idx];
    }
    node->end--; // 走到末尾，end - 1
}
