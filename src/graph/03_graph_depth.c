#include <stdio.h>
#include "graph.h"
#include "stack_oop.h"

static void dfs(Node *node) {
    if (!node) {
        return;
    }
    Stack *s = stackCreate();
    HashMap *m = hashmap_create(16, hash_ptr, equal_ptr);
    s->push(s, node);
    MAP_PUT(m, node, node);

    printf("%d", node->val);

    while (!s->empty(s)) {
        Node *cur = (Node *) s->pop(s);
        Edge *e = cur->edges;
        while (e) {
            Node *next = e->to;
            if (!hashmap_contains(m, next)) {
                s->push(s, cur);
                s->push(s, next);
                MAP_PUT(m, next, next);
                printf("%d", next->val);
                break;
            }
            e = e->next;
        }
    }
}

static void dfs_recursive(Node *node, HashMap *visited) {
    if (!node || MAP_HAS(visited, node)) return;

    // 访问当前节点
    printf("%d", node->val);
    MAP_PUT(visited, node, node);

    // 递归遍历所有邻居
    Edge *e = node->edges;
    while (e) {
        dfs_recursive(e->to, visited);
        e = e->next;
    }
}

// 入口
static void dfs_recursive_handler(Node *node) {
    HashMap *m = hashmap_create(16, hash_ptr, equal_ptr);
    dfs_recursive(node, m);
    hashmap_free(m);
}
