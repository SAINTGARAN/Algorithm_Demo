#include <stdio.h>
#include <stdlib.h>

/*
 * 用一个HashMap把每个节点的层号存起来。出队是查表得知当前节点属于那层
 * 和currentLevel比较来判断是否换层了
 */

typedef struct Node {
    int val;
    struct Node *left, *right;
} Node;

/* -------- HashMap：节点指针 -> 层号 -------- */
#define MAP_SIZE 1000

/*
* 先理解 HashMap 的结构
    table[0] --> NULL
    table[1] --> [NodeA, level=2] --> [NodeC, level=3] --> NULL
    table[2] --> [NodeB, level=1] --> NULL
...
多个 key 可能 hash 到同一个桶（下标），所以每个桶是一条链表，这叫链地址法解决哈希冲突。
 */
typedef struct MapEntry {
    Node *key;
    int level;
    struct MapEntry *next;
} MapEntry;

MapEntry *table[MAP_SIZE];

void map_init() {
    for (int i = 0; i < MAP_SIZE; i++) {
        table[i] = NULL;
    }
}

int hash(Node *p) {
    return (int) ((unsigned long long) p % MAP_SIZE);
}

void map_put(Node *key, int level) {
    int h = hash(key);
    /*
     * 链地址法解决hash冲突
     * 遍历该桶的链表
     * 找到了一个已经存在的同一个节点
     * 直接更新层号，不重复插入
     */
    for (MapEntry *e = table[h]; e; e = e->next) {
        if (e->key == key) {
            e->level = level;
            return;
        }
    }
    // 没找到 --> 循环结束后在链表表头插入新节点
    MapEntry *e = malloc(sizeof(MapEntry));
    e->key = key;
    e->level = level;
    e->next = table[h]; // // 新节点的 next 存入"第一个节点的地址"，接上原链表
    table[h] = e; // // 把指针变量里的地址换成新节点的地址，新节点成为新的头
}

int map_get(Node *key) {
    int h = hash(key);
    for (MapEntry *e = table[h]; e; e = e->next) {
        if (e->key == key) {
            return e->level;
        }
    }
    return -1;
}

void map_free() {
    for (int i = 0; i < MAP_SIZE; i++) {
        MapEntry *e = table[i];
        while (e) {
            MapEntry *next = e->next;
            free(e);
            e = next;
        }
        table[i] = NULL;
    }
}

/* -------- BFS + HashMap 求最大宽度 -------- */
int maxWidth(Node *root) {
    if (!root) return 0;
    map_init();

    Node *queue[1024];
    int front = 0, rear = 0;

    queue[rear++] = root;
    map_put(root, 1); // head 对应第 1 层

    int curLevel = 1;
    int curLevelNodes = 0;
    int max = 0; // 对应 Integer.MIN_VALUE，这里直接用 0

    while (front < rear) {
        Node *cur = queue[front++];
        int curNodeLevel = map_get(cur);

        if (curNodeLevel == curLevel) {
            curLevelNodes++; // 还在同一层，计数
        } else {
            /* 换层：结算上一层宽度 */
            if (curLevelNodes > max) max = curLevelNodes;
            curLevel++;
            curLevelNodes = 1; // 新层第一个节点
        }

        if (cur->left) {
            map_put(cur->left, curNodeLevel + 1);
            queue[rear++] = cur->left;
        }
        if (cur->right) {
            map_put(cur->right, curNodeLevel + 1);
            queue[rear++] = cur->right;
        }
    }

    /* ⚠ 最后一层在 while 内没有机会触发 else，必须补一次 */
    if (curLevelNodes > max) max = curLevelNodes;

    map_free();
    return max;
}
