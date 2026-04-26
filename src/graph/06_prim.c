#include "graph.h"

/*
 * Prim 算法
 * 从任意节点出发，每次找集合边界权重最小的边
 * 把对应节点加入集合，直到所有节点加入
 */


/*
 * Prim 算法
 * 从任意节点出发，每次找集合边界权重最小的边
 * 把对应节点加入集合，直到所有节点加入
 */

static void prim(Graph *g, int start_val) {
    int node_count = hashmap_size(g->node_map);

    /* visited：记录节点是否已加入集合，key=Node *，value=int */
    HashMap *visited = hashmap_create(node_count, hash_ptr, equal_ptr);

    /* 起始节点加入集合 */
    Node *start = graph_get_node(g, start_val);
    if (!start) return;

    int zero = 0;
    MAP_PUT(visited, start, zero);

    int total_weight = 0;
    int selected = 0;
    Edge **mst = malloc(sizeof(Edge *) * (node_count - 1));

    while (selected < node_count - 1) {
        Edge *min_edge = NULL;

        /* 遍历所有已加入集合的节点，找边界最小边 */
        HashMapIter *iter = hashmap_iter_create(visited);
        while (hashmap_iter_next(iter)) {
            Node *cur = (Node *) hashmap_iter_key(iter);

            /* 遍历当前节点的所有邻接边 */
            for (Edge *e = cur->edges; e != NULL; e = e->next) {
                /* 邻居不在集合内才考虑 */
                int *in = MAP_GET(visited, e->to, int);
                if (in) continue; /* 已在集合内，跳过 */

                /* 找权重最小的边 */
                if (!min_edge || e->weight < min_edge->weight)
                    min_edge = e;
            }
        }
        hashmap_iter_free(iter);

        /* 找不到边，图不连通 */
        if (!min_edge) {
            printf("图不连通，无法构成生成树\n");
            free(mst);
            hashmap_free(visited);
            return;
        }

        /* 把最小边的目标节点加入集合 */
        MAP_PUT(visited, min_edge->to, zero);
        mst[selected++] = min_edge;
        total_weight += min_edge->weight;
    }

    /* 输出结果 */
    printf("最小生成树总权重：%d\n", total_weight);
    for (int i = 0; i < selected; i++)
        printf("  %d --%d--> %d\n",
               mst[i]->from->val,
               mst[i]->weight,
               mst[i]->to->val);

    free(mst);
    hashmap_free(visited);
}

// 处理森林
