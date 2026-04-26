#pragma once

#include <stdbool.h>

typedef struct Stack Stack;
typedef struct StackPrivate StackPrivate;

struct Stack {
    // 私有数据
    StackPrivate *_priv;

    // 公有方法
    void (*destroy)(Stack *self);

    void (*push)(Stack *self, void *data);

    void *(*pop)(Stack *self);

    void *(*peek)(Stack *self);

    bool (*empty)(Stack *self);

    int (*size)(Stack *self);

    void (*print)(Stack *self, void (*printFn)(void *));
};

// 唯一的构造入口
Stack *stackCreate(void);
