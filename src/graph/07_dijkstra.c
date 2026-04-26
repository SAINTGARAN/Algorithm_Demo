#include "graph.h"
#include "limits.h"
/*
 * Dijkstra:单源最短路径算法，从一个点出发，找到所有其它节点的最短路径
 * 思想：
 * 维护一个距离表 dist,记录起点到每个节点的最短距离
 * 初始:起始距离为0，其它节点距离 = 无穷大
 *
 * 每次从未处理的节点中找距离最小的节点cur
 * 用cur去更新它所有邻居的距离
 *
 * 重复直到所有节点处理完
 */

static void dijkstra(Graph *g, int start_val) {
    int node_count = hashmap_size(g->node_map);

    /* key = node* val = int(距离)*/
    HashMap *dist = hashmap_create(node_count, hash_ptr, equal_ptr);

    /* processed: key = node*, val = int */
    HashMap *processed = hashmap_create(node_count, hash_ptr, equal_ptr);

    /* 初始化所有节点 */
    int inf = INT_MAX;
    int zero = 0;

    /* 初始化所有节点距离为INT_MAX */
    HashMapIter *it = hashmap_iter_create(g->node_map);
    while (hashmap_iter_next(it)) {
        Node *node = *(Node **) hashmap_iter_value(it); // val = node*
        MAP_PUT(dist, node, inf);
    }
    hashmap_iter_free(it);

    /* 起点距离为0 */
    Node *start = graph_get_node((g), start_val);
    if (!start) {
        return;
    }
    MAP_PUT(dist, start, zero);

    for (int i = 0; i < node_count; i++) {
        // 处理未处理节点距离最小的
        Node *cur = NULL;
        int min_dist = INT_MAX;

        HashMapIter *it = hashmap_iter_create(dist);
        while (hashmap_iter_next(it)) {
            Node *node = *(Node **) hashmap_iter_key(it);
            int d = *(int *) hashmap_iter_value(it);
            int *is_processed = MAP_GET(processed, node, int); // 一旦我们找到了当前距离最短的节点并用它更新了邻居，这个节点的最短距离就固定下来了，以后不需要也不应该再处理它

            if (!is_processed && d < min_dist) {
                min_dist = d;
                cur = node;
            }
        }
        hashmap_iter_free(it);

        /* 找不到，剩余节点不可达 */
        if (!cur) break;
        /* 标记已处理 */
        MAP_PUT(processed, cur, zero);

        /* 更新邻居距离 */
        for (Edge *e = cur->edges; e != NULL; e = e->next) {
            int *d = MAP_GET(dist, e->to, int);
            int new_dist = min_dist + e->weight;

            if (d && new_dist < *d)
                MAP_PUT(dist, e->to, new_dist);
        }
    }

    /* 输出结果 */
    printf("从节点 %d 出发的最短路径：\n", start_val);
    HashMapIter *res = hashmap_iter_create(dist);
    while (hashmap_iter_next(res)) {
        Node *node = *(Node **) hashmap_iter_key(res);
        int d = *(int *) hashmap_iter_value(res);
        if (d == INT_MAX)
            printf("  节点%d：不可达\n", node->val);
        else
            printf("  节点%d：%d\n", node->val, d);
    }
    hashmap_iter_free(res);

    hashmap_free(dist);
    hashmap_free(processed);
}
