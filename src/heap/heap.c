#include "heap.h"
#include <stdlib.h>

// 小根堆：值越小越优先
int cmp_min(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
    // a < b → 返回负数 → a 更优先 → a 在上面
}

// 大根堆：值越大越优先
int cmp_max(const void *a, const void *b) {
    return *(int *) b - *(int *) a; // a、b 调换位置
    // a > b → b-a 为负 → a 更优先 → a 在上面
}

Heap *heap_create(int capacity, HeapComparator comp) {
    Heap *h = (Heap *) malloc(sizeof(Heap));
    if (!h) return NULL;
    h->nodes = (void **) malloc(sizeof(void *) * capacity);
    h->size = 0;
    h->capacity = capacity;
    h->comp = comp;
    return h;
}

void heap_free(Heap *h) {
    if (h) {
        free(h->nodes);
        free(h);
    }
}

static void _swap(void **a, void **b) {
    void *temp = *a;
    *a = *b;
    *b = temp;
}

static void _heapify_up(Heap *h, int index) {
    while (index > 0) {
        // 如果已经爬到根节点（index 0），就没法再爬了
        int parent = (index - 1) / 2; // 找到我爸爸是谁
        // 调用比较器：对比“我”和“我爸爸”
        // 如果 comp 返回 < 0，说明“我”的优先级比“我爸爸”高
        if (h->comp(h->nodes[index], h->nodes[parent]) < 0) {
            // 交换“我”和“我爸爸”的位置
            _swap(&h->nodes[index], &h->nodes[parent]);
            // 我现在跳到了爸爸的位置，继续往上看，重复这个过程
            index = parent;
        } else {
            // 如果我不比我爸爸优秀，说明位置对了，停止“向上爬”
            break;
        }
    }
}

static void _heapify_down(Heap *h, int index) {
    int best = index; // 假设当前这个位置（父节点）是最好的
    int left = 2 * index + 1; // 我的左孩子
    int right = 2 * index + 2; // 我的右孩子
    // 1. 看看左孩子是否比我更好？
    // left < h->size 是为了保证左孩子确实存在（没越界）
    if (left < h->size && h->comp(h->nodes[left], h->nodes[best]) < 0)
        best = left;
    // 2. 看看右孩子是否比“目前最好的”还要好？
    if (right < h->size && h->comp(h->nodes[right], h->nodes[best]) < 0)
        best = right;
    // 3. 检查：最好的位置还是我吗？
    if (best != index) {
        // 如果最好的不是我，说明孩子里有更优秀的
        // 那就和最优秀的那个孩子交换位置
        _swap(&h->nodes[index], &h->nodes[best]);
        // 交换之后，我到了孩子的位置，继续往下看，递归处理
        _heapify_down(h, best);
    }
}

void heap_push(Heap *h, void *data) {
    if (h->size == h->capacity) {
        h->capacity *= 2;
        h->nodes = (void **) realloc(h->nodes, sizeof(void *) * h->capacity);
    }
    h->nodes[h->size] = data;
    _heapify_up(h, h->size);
    h->size++;
}

void *heap_pop(Heap *h) {
    if (h->size == 0) return NULL;
    void *res = h->nodes[0];
    h->nodes[0] = h->nodes[h->size - 1];
    h->size--;
    _heapify_down(h, 0);
    return res;
}

void *heap_peek(Heap *h) {
    return (h->size > 0) ? h->nodes[0] : NULL;
}

int heap_size(Heap *h) {
    return h->size;
}

int heap_is_empty(Heap *h) {
    return h->size == 0;
}
