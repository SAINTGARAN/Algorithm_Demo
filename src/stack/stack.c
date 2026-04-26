#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY 8

/* 结构体定义只在 .c 里，外部完全看不到 */
struct Stack {
    int *data;
    int top;
    int capacity;
};

Stack *stackCreate(void) {
    Stack *s = (Stack *)malloc(sizeof(Stack));
    s->data = (int *)malloc(sizeof(int) * INIT_CAPACITY);
    s->top = 0;
    s->capacity = INIT_CAPACITY;
    return s;
}

void stackDestroy(Stack **s) {
    if (!s || !*s) return;
    free((*s)->data);
    free(*s);
    *s = NULL;  /* 调用方的指针也被置空，杜绝野指针 */
}

void stackPush(Stack *s, int val) {
    if (s->top == s->capacity) {
        s->capacity *= 2;
        s->data = (int *)realloc(s->data, sizeof(int) * s->capacity);
    }
    s->data[s->top++] = val;
}

int stackPop(Stack *s) {
    if (stackEmpty(s)) {
        fprintf(stderr, "Error: pop from empty stack\n");
        exit(EXIT_FAILURE);
    }
    return s->data[--s->top];
}

int stackPeek(Stack *s) {
    if (stackEmpty(s)) {
        fprintf(stderr, "Error: peek at empty stack\n");
        exit(EXIT_FAILURE);
    }
    return s->data[s->top - 1];
}

bool stackEmpty(Stack *s) {
    return s->top == 0;
}

int stackSize(Stack *s) {
    return s->top;
}

void stackPrint(Stack *s) {
    printf("Stack (bottom -> top): [");
    for (int i = 0; i < s->top; i++) {
        printf("%d", s->data[i]);
        if (i < s->top - 1) printf(", ");
    }
    printf("]\n");
}
