#pragma once
#include <stdio.h>
/*
 * linux内核的链表实现核心思想是吧链表节点嵌入到数据结构里， 而不是让链表节点包含数据
 */

struct list_head {
    struct list_head *prev;
    struct list_head *next;
};

// 初始化
#define LIST_HEAD_INIT {&(name), &(name)}   // 让一个空链表的prev和next都指向自己
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name)

// 通过成员指针反推结构体指针 结构体放在地址 0，成员的地址就等于它的偏移量
#define my_offsetof(type, member) \
        ((unsigned long)&((type *)0)->member)

#define container_of(ptr, type, member) \
        ((type *)((char *)(ptr) - my_offsetof(type, member)))

// 专用：从链表节点拿到数据结构
#define list_entry(ptr, type, member) \
        container_of(ptr, type, member)

// 遍历
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/*
 * 结构体成员在内存中的位置是固定的，知道成员地址和偏移量，就能算出结构体起始地址
 * 用 pos 遍历 head 链表，每个节点是 type 类型，链表挂在 member 成员上
 * struct student *s;
 * list_for_each_entry(s, &student_list, struct student, list) {
 * printf("%s\n", s->name);
}
 */
#define list_for_each_entry(pos, head, type, member)                    \
    for (pos = list_entry((head)->next, type, member);                  \
        &pos->member != (head);                                        \
        pos = list_entry(pos->member.next, type, member))

// 在两个节点之间插入
static inline void __list_add(struct list_head *new,
                              struct list_head *prev,
                              struct list_head *next) {
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

// 头插法
static inline void list_add(struct list_head *new, struct list_head *head) {
    __list_add(new, head, head->next);
}

// 尾插法
static inline void list_add_tail(struct list_head *new, struct list_head *head) {
    __list_add(new, head, head->prev);
}

// 删除节点
static inline void list_del(struct list_head *entry) {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
}
