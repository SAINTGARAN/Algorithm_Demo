#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>

/**
 * 比较函数指针类型
 * 返回值意义：
 * < 0 : a 的优先级高于 b (a 应该在堆顶)
 * > 0 : b 的优先级高于 a
 * = 0 : 优先级相等
 */
typedef int (*HeapComparator)(const void *a, const void *b);

int cmp_min(const void *a, const void *b);

int cmp_max(const void *a, const void *b);

// 堆结构体定义
typedef struct {
    void **nodes; // 存储指针的动态数组
    int size; // 当前元素数量
    int capacity; // 当前容量
    HeapComparator comp; // 比较器函数指针
} Heap;

/**
 * 创建一个堆
 * @param capacity 初始容量
 * @param comp 比较器函数
 */
Heap *heap_create(int capacity, HeapComparator comp);

/**
 * 销毁堆
 * 注意：此函数仅释放堆本身的内存，不释放 nodes 中存储的元素指向的内存
 */
void heap_free(Heap *h);

/**
 * 向堆中插入一个元素
 */
void heap_push(Heap *h, void *data);

/**
 * 弹出堆顶元素（优先级最高的元素）
 * @return 堆顶元素的指针，如果堆为空则返回 NULL
 */
void *heap_pop(Heap *h);

/**
 * 查看堆顶元素但不弹出
 */
void *heap_peek(Heap *h);

/**
 * 返回当前堆的大小
 */
int heap_size(Heap *h);

/**
 * 判断堆是否为空
 */
int heap_is_empty(Heap *h);

#endif // HEAP_H
