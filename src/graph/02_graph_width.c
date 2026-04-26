#include <stdio.h>
#include <stdlib.h>
#include "../HashMap/hashmap.h"
#include "../../datastruct/queue/queue.h"
/*
 * 图的宽度优先遍历
 * 1、利用队列实现
 * 2、从源节点开始依次按照宽度进队列，然后弹出
 * 3、每弹出一个点，就把该点所有没进过队列的邻接点放入队列
 * 4、直到队列为空
 */

typedef struct Node {
    int val;
    int in; // 入度
    int out; // 出度
    struct Edge *edges; // 邻接边链表（通过 edge->to 找到所有相邻节点）
    int edge_count;
} Node;

typedef struct Edge {
    int weight;
    Node *from;
    Node *to;
    struct Edge *next; // 下一条边
} Edge;

typedef struct Graph {
    HashMap *node_map; // val → Node*，快速查找节点
    Edge **edges; // 所有边的数组，方便遍历和释放内存
    int edge_count;
    int edge_cap;
} Graph;

static void bfs(Node *node) {
    if (node == NULL) return;

    Queue *queue = QUEUE_CREATE(Node *);
    HashMap *visited = hashmap_create(16, hash_ptr, equal_ptr);

    /* 入队，标记已访问 */
    ENQUEUE(queue, node);
    MAP_PUT(visited, node, node);

    while (QUEUE_SIZE(queue) > 0) {
        Node *cur;
        DEQUEUE(queue, Node *, cur);

        /* 处理当前节点 */
        printf("访问节点: %d\n", cur->val);

        /* 遍历所有邻接边 */
        Edge *e = cur->edges;
        while (e != NULL) {
            Node *next = e->to;

            /* 没访问过才入队 */
            if (!MAP_HAS(visited, next)) {
                ENQUEUE(queue, next);
                MAP_PUT(visited, next, next);
            }
            e = e->next;
        }
    }

    hashmap_free(visited);
    QUEUE_DESTROY(queue);
}

static void bfs_recursive(Queue *q, HashMap *visited) {
    if (QUEUE_EMPTY(q)) return;

    Node *cur = DEQUEUE_M(q, Node *);
    printf("%d ", cur->val);

    for (Edge *e = cur->edges; e != NULL; e = e->next) {
        if (!MAP_HAS(visited, e->to)) {
            // 用指针地址判断
            MAP_PUT(visited, e->to, e->to); // 用指针地址标记
            ENQUEUE(q, e->to);
        }
    }
    bfs_recursive(q, visited);
}

void bfs_recursive_handler(Node *start) {
    Queue *q = QUEUE_CREATE(Node *);
    HashMap *visited = hashmap_create(16, hash_ptr, equal_ptr);

    MAP_PUT(visited, start, start);
    ENQUEUE(q, start);

    bfs_recursive(q, visited);

    hashmap_free(visited);
    QUEUE_DESTROY(q);
}

/* ─── 辅助函数 ─── */

Node *create_node(int val) {
    Node *node = malloc(sizeof(Node));
    node->val = val;
    node->in = 0;
    node->out = 0;
    node->edges = NULL;
    node->edge_count = 0;
    return node;
}

static void add_edge(Graph *g, Node *from, Node *to, int weight) {
    Edge *e = malloc(sizeof(Edge));
    e->weight = weight;
    e->from = from;
    e->to = to;
    e->next = from->edges;
    from->edges = e;
    from->out++;
    to->in++;
    from->edge_count++;

    if (g->edge_count == g->edge_cap) {
        g->edge_cap *= 2;
        g->edges = realloc(g->edges, g->edge_cap * sizeof(Edge *));
    }
    g->edges[g->edge_count++] = e;
}

static Graph *graph_create() {
    Graph *g = malloc(sizeof(Graph));
    g->node_map = hashmap_create(16, hash_ptr, equal_ptr);
    g->edges = malloc(16 * sizeof(Edge *));
    g->edge_count = 0;
    g->edge_cap = 16;
    return g;
}

static void graph_free(Graph *g) {
    if (g == NULL) return;

    /* 释放所有边 */
    for (int i = 0; i < g->edge_count; i++)
        free(g->edges[i]);
    free(g->edges);

    /* 释放所有节点 + HashMap */
    hashmap_free_with(g->node_map, free); /* free 直接释放 Node* */

    free(g);
}

/* ─── 测试用例 ─── */

int main() {
    /*
     * 测试图结构：
     *
     *     1
     *    / \
     *   2   3
     *  / \   \
     * 4   5   6
     *
     * BFS 期望顺序：1 → 2 → 3 → 4 → 5 → 6
     */
    printf("=== 测试1：普通树形图 ===\n");
    {
        Graph *g = graph_create();
        Node *n1 = create_node(1);
        Node *n2 = create_node(2);
        Node *n3 = create_node(3);
        Node *n4 = create_node(4);
        Node *n5 = create_node(5);
        Node *n6 = create_node(6);

        add_edge(g, n1, n2, 1);
        add_edge(g, n1, n3, 1);
        add_edge(g, n2, n4, 1);
        add_edge(g, n2, n5, 1);
        add_edge(g, n3, n6, 1);

        bfs(n1);
        graph_free(g);
    }

    /*
     * 测试图结构（有环）：
     *
     *  1 → 2 → 3
     *  ↑       |
     *  └───────┘
     *
     * BFS 期望顺序：1 → 2 → 3
     * 不能死循环
     */
    printf("\n=== 测试2：有环图 ===\n");
    {
        Graph *g = graph_create();
        Node *n1 = create_node(1);
        Node *n2 = create_node(2);
        Node *n3 = create_node(3);

        add_edge(g, n1, n2, 1);
        add_edge(g, n2, n3, 1);
        add_edge(g, n3, n1, 1); /* 环：3 → 1 */

        bfs(n1);
        graph_free(g);
    }

    /*
     * 测试图结构（非连通图，只遍历起点能到达的部分）：
     *
     *  1 → 2    3 → 4
     *            （孤立）
     *
     * BFS 从 1 出发，期望顺序：1 → 2
     * 3 和 4 不可达，不应出现
     */
    printf("\n=== 测试3：非连通图 ===\n");
    {
        Graph *g = graph_create();
        Node *n1 = create_node(1);
        Node *n2 = create_node(2);
        Node *n3 = create_node(3);
        Node *n4 = create_node(4);

        add_edge(g, n1, n2, 1);
        add_edge(g, n3, n4, 1); /* 孤立的连通分量 */

        bfs(n1);
        graph_free(g);
    }

    /*
     * 测试图结构（单节点）：
     *
     *  1
     *
     * BFS 期望输出：1
     */
    printf("\n=== 测试4：单节点 ===\n");
    {
        Graph *g = graph_create();
        Node *n1 = create_node(1);

        bfs(n1);
        graph_free(g);
    }

    /*
     * 测试 NULL 节点
     */
    printf("\n=== 测试5：NULL 节点 ===\n");
    {
        bfs(NULL);
        printf("NULL 节点，正常返回\n");
    }

    return 0;
}
