#include "graph.h"
/*
 * 要求无向图
 * 最小生成树
 * 核心是贪心：每次选权重最小的边，只要不形成环就加入
 */
//
// 用数组标记连通分量代替并查集，思路是给每个节点一个组号，合并时把一组的所有节点改成同一个组号

/*
 * 用hashmap存组好， key是node* value是组号， 完全不改变Node结构体
 */

static int cmp_edge(const void *a, const void *b) {
    Edge *edgeA = *(Edge **) a;
    Edge *edgeB = *(Edge **) b;
    return edgeA->weight - edgeB->weight;
}

static void krushal_hashmap(Graph *g) {
    int node_count = hashmap_size(g->node_map);
    /* 用 HashMap 存组号，key=Node *，value=组号（用指针强转存int）*/
    HashMap *group = hashmap_create(node_count, hash_ptr, equal_ptr);

    /* 初始化：每个节点组号等于自己的地址转换成的序号 */
    int id = 0;
    HashMapIter *iter = hashmap_iter_create(g->node_map);
    while (hashmap_iter_next(iter)) {
        Node *node = hashmap_iter_value(iter); //g->hashmap 的value是组号
        MAP_PUT(group, node, id);
    }
    hashmap_iter_free(iter);

    /* 边按权重排序 */
    /* qsort本身传的就是数组的地址， 数组元素本身就是Edge* qsort传递的是元素的地址*/
    qsort(g->edges, g->edge_count, sizeof(Edge *), cmp_edge);

    /* 选边 */
    int selected = 0;
    int total_weight = 0;
    Edge **mst = malloc(sizeof(Edge *) * (node_count - 1)); // n个节点的生成树恰好有n - 1条边

    // 遍历
    for (int i = 0; i < g->edge_count && selected < node_count - 1; i++) {
        Edge *edge = g->edges[i];
        int gu = *MAP_GET(group, edge->from, int);
        int gv = *MAP_GET(group, edge->to, int);

        if (gu == gv) continue; // 同组，会成环，跳过
        /* 选中这条边，把 gv 组全部并入 gu */
        mst[selected++] = edge;
        total_weight += edge->weight;

        /* 遍历所有节点，把 gv 组改成 gu */
        HashMapIter *it = hashmap_iter_create(group);
        while (hashmap_iter_next(it)) {
            Node *node = hashmap_iter_key(it);
            int g_id = *(int *) hashmap_iter_value(it);
            if (g_id == gv)
                MAP_PUT(group, node, gu);
        }
        hashmap_iter_free(it);
    }

    /* 输出 */
    if (selected < node_count - 1)
        printf("图不连通，无法构成生成树\n");
    else {
        printf("最小生成树总权重：%d\n", total_weight);
        for (int i = 0; i < selected; i++)
            printf("  %d --%d--> %d\n",
                   mst[i]->from->val,
                   mst[i]->weight,
                   mst[i]->to->val);
    }

    hashmap_free(group);
    free(mst);
}
