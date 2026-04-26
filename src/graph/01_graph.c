#include "graph.h"

// 创建图
#include "graph.h"

/* ── 创建图 ── */
Graph *graph_create(void) {
    Graph *g = malloc(sizeof(Graph));
    g->node_map = hashmap_create(16, hash_int, equal_int);
    g->edges = malloc(sizeof(Edge *) * 16);
    g->edge_count = 0;
    g->edge_cap = 16;
    return g;
}

/* ── 添加节点 ── */
Node *graph_add_node(Graph *g, int val) {
    /* 已存在直接返回 */
    Node **exist = MAP_GET(g->node_map, val, Node *);
    if (exist && *exist) return *exist;

    Node *node = malloc(sizeof(Node));
    node->val = val;
    node->in = 0;
    node->out = 0;
    node->edges = NULL;
    node->edge_count = 0;

    MAP_PUT(g->node_map, val, node);
    return node;
}

/* ── 查找节点 ── */
Node *graph_get_node(Graph *g, int val) {
    Node **res = MAP_GET(g->node_map, val, Node *);
    return (res ? *res : NULL);
}

/* ── 内部：创建一条边 ── */
static Edge *make_edge(Node *from, Node *to, int weight) {
    Edge *e = malloc(sizeof(Edge));
    e->weight = weight;
    e->from = from;
    e->to = to;
    e->next = NULL;
    return e;
}

/* ── 内部：把边追加到图的 edges 数组 ── */
static void push_edge(Graph *g, Edge *e) {
    if (g->edge_count >= g->edge_cap) {
        g->edge_cap *= 2;
        g->edges = realloc(g->edges, sizeof(Edge *) * g->edge_cap);
    }
    g->edges[g->edge_count++] = e;
}

/* ── 内部：把边头插到节点的邻接链表 ── */
static void attach_edge(Node *from, Edge *e) {
    e->next = from->edges;
    from->edges = e;
    from->out++;
    e->to->in++;
    from->edge_count++;
}

/* ── 添加有向边 A → B ── */
Edge *graph_add_directed_edge(Graph *g, int from_val, int to_val, int weight) {
    Node *from = graph_get_node(g, from_val);
    Node *to = graph_get_node(g, to_val);
    if (!from || !to) return NULL;

    Edge *e = make_edge(from, to, weight);
    attach_edge(from, e);
    push_edge(g, e);
    return e;
}

/* ── 添加无向边 A ↔ B（两条有向边）── */
void graph_add_edge(Graph *g, int from_val, int to_val, int weight) {
    Node *from = graph_get_node(g, from_val);
    Node *to = graph_get_node(g, to_val);
    if (!from || !to) return;

    Edge *e = make_edge(from, to, weight);
    Edge *re = make_edge(to, from, weight);

    attach_edge(from, e);
    attach_edge(to, re);
    push_edge(g, e);
    push_edge(g, re);
}

/* ── 释放图 ── */
void graph_free(Graph *g) {
    if (!g) return;

    /* 释放所有边 */
    for (int i = 0; i < g->edge_count; i++)
        free(g->edges[i]);
    free(g->edges);

    /* 释放所有节点 */
    HashMapIter *iter = hashmap_iter_create(g->node_map);
    while (hashmap_iter_next(iter)) {
        Node *node = *(Node **) hashmap_iter_value(iter);
        free(node);
    }
    hashmap_iter_free(iter);

    hashmap_free(g->node_map);
    free(g);
}

/* ── 打印图（调试用）── */
void graph_print(Graph *g) {
    HashMapIter *iter = hashmap_iter_create(g->node_map);
    while (hashmap_iter_next(iter)) {
        Node *node = *(Node **) hashmap_iter_value(iter);
        printf("节点%d (in=%d out=%d): ", node->val, node->in, node->out);
        for (Edge *e = node->edges; e != NULL; e = e->next)
            printf("--%d-->%d ", e->weight, e->to->val);
        printf("\n");
    }
    hashmap_iter_free(iter);
}

