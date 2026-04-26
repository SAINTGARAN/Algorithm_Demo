#pragma once

#include <stdbool.h>

/* 不透明类型，外部只知道有这个东西，看不到里面有什么 */
typedef struct Stack Stack;

Stack *stackCreate(void);

void stackDestroy(Stack **s);

void stackPush(Stack *s, int val);

int stackPop(Stack *s);

int stackPeek(Stack *s);

bool stackEmpty(Stack *s);

int stackSize(Stack *s);

void stackPrint(Stack *s);
