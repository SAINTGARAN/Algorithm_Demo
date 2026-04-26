#include <stdio.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

// 将一个链表插入到另一个链表里面去
static void insertListToList(Node *aHead, Node *bHead, int index) {
    if (aHead == NULL || bHead == NULL) { return; }

    // 找到插入点上一个节点
    Node *curA = aHead;
    for (int i = 0; i < index - 1 && curA != NULL; i++) {
        curA = curA->next;
    }

    // 如果index太大，超出了A的范围 找到了Index位置的节点以及Index超出了A的范围
    if (curA == NULL) {
        return;
    }
 Node *realB = bHead->next;
    if (realB == NULL) {
        return;
    }
    // 找到b的尾节点
    Node *curB = bHead;
    while (curB->next != NULL) {
        curB = curB->next;
    }

    // 交换指针 先接尾
    // curB->next = curA->next;
    // curA->next = bHead;

    // 先接头，后接尾
    Node *tempA = curA->next;
    curA->next = realB;
    curB->next = tempA;

}
