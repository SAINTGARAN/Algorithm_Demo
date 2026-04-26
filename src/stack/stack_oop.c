#include "stack_oop.h"
#include <stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY 16

struct StackPrivate {
    void **data; // 指针数组
    int top;
    int capacity;
};

// ========== 方法实现 ==========

static bool _empty(Stack *self) {
    return self->_priv->top == 0;
}

static int _size(Stack *self) {
    return self->_priv->top;
}

static void _push(Stack *self, void *data) {
    StackPrivate *p = self->_priv;
    if (p->top >= p->capacity) {
        p->capacity *= 2;
        void **tmp = realloc(p->data, sizeof(void *) * p->capacity);
        if (!tmp) {
            fprintf(stderr, "[%s:%d] realloc failed\n", __FILE__, __LINE__);
            exit(EXIT_FAILURE);
        }
        p->data = tmp;
    }
    p->data[p->top++] = data;
}

static void *_pop(Stack *self) {
    if (_empty(self)) {
        fprintf(stderr, "Error: pop from empty stack\n");
        exit(EXIT_FAILURE);
    }
    return self->_priv->data[--self->_priv->top];
}

static void *_peek(Stack *self) {
    if (_empty(self)) {
        fprintf(stderr, "Error: peek from empty stack\n");
        exit(EXIT_FAILURE);
    }
    return self->_priv->data[self->_priv->top - 1];
}

static void _print(Stack *self, void (*printFn)(void *)) {
    StackPrivate *p = self->_priv;
    if (_empty(self)) {
        printf("[empty stack]\n");
        return;
    }
    printf("bottom -> ");
    for (int i = 0; i < p->top; i++) {
        printFn(p->data[i]);
        if (i < p->top - 1) printf(", ");
    }
    printf(" <- top\n");
}

static void _destroy(Stack *self) {
    free(self->_priv->data);
    free(self->_priv);
    free(self);
}

// ========== 构造函数 ==========

Stack *stackCreate(void) {
    Stack *s = malloc(sizeof(Stack));
    if (!s) {
        fprintf(stderr, "[%s:%d] malloc failed\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    s->_priv = malloc(sizeof(StackPrivate));
    if (!s->_priv) {
        free(s);
        fprintf(stderr, "[%s:%d] malloc failed\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    s->_priv->data = malloc(sizeof(void *) * INIT_CAPACITY);
    if (!s->_priv->data) {
        free(s->_priv);
        free(s);
        fprintf(stderr, "[%s:%d] malloc failed\n", __FILE__, __LINE__);
        exit(EXIT_FAILURE);
    }

    s->_priv->top = 0;
    s->_priv->capacity = INIT_CAPACITY;

    s->destroy = _destroy;
    s->push = _push;
    s->pop = _pop;
    s->peek = _peek;
    s->empty = _empty;
    s->size = _size;
    s->print = _print;

    return s;
}
