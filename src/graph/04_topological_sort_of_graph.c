#include "graph.h"
#include "queue.h"

static void topology_sort(Graph *g) {
    Queue *q = QUEUE_CREATE(Node*);

    /* 第一步： 所有入度为0的点入队 */
    // 遍历node_map里面的所有节点
    HashMapIter *iter = hashmap_iter_create(g->node_map);
    while (hashmap_iter_next(iter)) {
        Node *cur = (Node *) hashmap_iter_value(iter);
        if (cur->in == 0) {
            ENQUEUE(q, cur);
        }
    }

    // 第二部BFS
    int result_count = 0; // 统计处理了多少个节点
    int total = hashmap_size(g->node_map);
    while (!QUEUE_EMPTY(q)) {
        Node *cur = (Node *) DEQUEUE_M(q, Node*);
        printf("%d", cur->val);
        result_count++;

        for (Edge *e = cur->edges; e; e = e->next) {
            // 出度为0的节点的入度 - 1
            e->to->in--;
            if (e->to->in == 0) {
                ENQUEUE(q, e->to);
            }
        }
    }

    /* 第三步：检测环 */
    if (result_count < total)
        printf("\n图中有环\n");

    QUEUE_DESTROY(q);
}
