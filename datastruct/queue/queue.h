#pragma once

#include <stddef.h>

typedef struct Queue Queue;

Queue *_queue_create(size_t data_size, void (*free_fn)(void *));

void _queue_destroy(Queue *q);

int _enqueue(Queue *q, const void *data);

int _dequeue(Queue *q, void *out);

void *_queue_peek(Queue *q);

int _queue_size(Queue *q);

int _queue_empty(Queue *q);

/* ─────────────────────────────────────────
   对外接口宏
───────────────────────────────────────── */
#define QUEUE_CREATE(type) \
_queue_create(sizeof(type), NULL)

#define QUEUE_CREATE_EX(type, free_fn) \
_queue_create(sizeof(type), (free_fn))

//__typeof__ 是 GCC 的扩展关键字，作用是在编译期获取一个表达式的类型
#define ENQUEUE(q, val) \
({ __typeof__(val) _tmp = (val); _enqueue((q), &_tmp); })

// DEQUEUE(q, int, val) 展开后：
// int _out; _dequeue(q, &_out); val = _out;
//                        ▲
//                   这就是 out，指向 _out 的地址
#define DEQUEUE(q, type, out) \
({ type _out; _dequeue((q), &_out); (out) = _out; })

// 改造后（一行声明+赋值）
#define DEQUEUE_M(q, type) \
({ type _out; _dequeue((q), &_out); _out; })
//                                      ↑ 语句表达式的值就是最后一个表达式

// 只看队头不取出的操作
#define QUEUE_PEEK(q, type) \
((type *)_queue_peek(q))

#define QUEUE_EMPTY(q)   _queue_empty(q)
#define QUEUE_SIZE(q)    _queue_size(q)
#define QUEUE_DESTROY(q) _queue_destroy(q)
