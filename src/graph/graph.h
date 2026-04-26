#pragma once
#include "hashmap.h"

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
    // 所有边的数组，方便遍历和释放内存 如果用 Edge *edges，那数组每个元素就是 Edge（结构体本身），移动和排序时要拷贝整个结构体，代价很大。用 Edge ** 只存指针，排序时只移动指针（8字节），快很多。
    Edge **edges;
    int edge_count;
    int edge_cap;
} Graph;

Graph *graph_create(void);

Node *graph_add_node(Graph *g, int val);

Node *graph_get_node(Graph *g, int val);

Edge *graph_add_directed_edge(Graph *g, int from_val, int to_val, int weight);

void graph_add_edge(Graph *g, int from_val, int to_val, int weight);

void graph_free(Graph *g);

void graph_print(Graph *g);
