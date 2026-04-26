#include <stdlib.h>
#include <string.h>
#include "queue.h"

typedef struct _QNode {
    void *data;
    struct _QNode *next;
} _QNode;

struct Queue {
    _QNode *front; // 指向队列第一个节点
    _QNode *rear; // 指向队列最后一个节点
    int size;
    size_t data_size;

    void (*free_fn)(void *);
};

Queue *_queue_create(size_t data_size, void (*free_fn)(void *)) {
    Queue *q = (Queue *) malloc(sizeof(Queue));
    if (!q) return (NULL);
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    q->data_size = data_size;
    q->free_fn = free_fn;
    return (q);
}

int _enqueue(Queue *q, const void *data) {
    _QNode *new_node = (_QNode *) malloc(sizeof(_QNode));
    if (!new_node) return (-1);

    new_node->data = malloc(q->data_size); // ✅ 用 q->data_size
    if (!new_node->data) {
        free(new_node);
        return (-1);
    }
    memcpy(new_node->data, data, q->data_size); // ✅
    new_node->next = NULL;

    if (q->rear) {
        q->rear->next = new_node;
    } else {
        q->front = new_node;
    }
    q->rear = new_node;
    q->size++;
    return (0);
}

int _dequeue(Queue *q, void *out) {
    if (!q || !q->front) return (-1); // ✅ 同时检查 q 和 q->front

    _QNode *old_node = q->front;
    if (out) memcpy(out, old_node->data, q->data_size); // ✅

    q->front = old_node->next; // ✅ front 前移
    if (!q->front) q->rear = NULL;

    if (q->free_fn) q->free_fn(old_node->data);
    free(old_node->data);
    free(old_node);
    q->size--;
    return (0);
}

void *_queue_peek(Queue *q) { return q->front ? q->front->data : NULL; }
int _queue_size(Queue *q) { return q->size; }
int _queue_empty(Queue *q) { return q->size == 0; }

void _queue_destroy(Queue *q) {
    if (!q) return;
    while (q->size) _dequeue(q, NULL);
    free(q); /* 堆上分配，这里统一释放 */
}
