#include <stdio.h>
#include "hashmap.h"
#define MAX(a, b) ((a) > (b) ? (a) : (b))
/*
 * 思路：
 * 1、x参与
 *      x的快乐值 + x的子节点的子节点不来的整颗数的最大快乐值相加
 * 2、x不参与
 *      0 + Max(a来整棵树最大快乐之， a不来整棵树的最大快乐值)
 */

/*
 * 实现一对多的树状结构， C语言通常有两种主流写法： 1、动态数组和链表
 *
 */
// 定义结构体
typedef struct Employee {
    int happy;
    struct Employee **nexts; // 指向 Employee 指针数组的指针（相当于 List）
    int nexts_count; // 当前有多少个下属
    int nexts_capacity; // 当前分配了多少个下属的空间
} Employee;

// 创建一个新员工
Employee *createEmployee(int happy) {
    Employee *e = (Employee *) malloc(sizeof(Employee));
    e->happy = happy;
    e->nexts = NULL;
    e->nexts_count = 0;
    e->nexts_capacity = 0;
    return e;
}

// 向list中添加下属
void add_subordinate(Employee *manager, Employee *sub) {
    if (manager->nexts_count >= manager->nexts_capacity) {
        int new_capacity = manager->nexts_capacity == 0 ? 2 : manager->nexts_capacity * 2;
        manager->nexts = (Employee **) realloc(manager->nexts, sizeof(Employee *) * new_capacity);
        manager->nexts_capacity = new_capacity;
    }
    manager->nexts[manager->nexts_count++] = sub;
}

// 释放内存
void freeEmployee(Employee *e) {
    if (e == NULL) return;
    for (int i = 0; i < e->nexts_count; i++) {
        freeEmployee(e->nexts[i]);
    }
    free(e->nexts);
    free(e);
}

typedef struct Info {
    int laiMaxHappy;
    int buMaxHappy;
} Info;

static Info process(Employee *e) {
    // 基础情况：如果是空指针
    if (e == NULL) {
        return (Info){0, 0};
    }

    // 基础情况：叶子节点（没有下属）
    if (e->nexts_count == 0) {
        return (Info){e->happy, 0};
    }

    int lai = e->happy; // x来的情况下的最大收益
    int bu = 0; // x不来的情况下，整颗数的最大收益
    for (int i = 0; i < e->nexts_count; i++) {
        Info nextInfo = process(e->nexts[i]);
        // 【修正点 1】：如果 e 来，下属只能选“不来”的状态
        lai += nextInfo.buMaxHappy;

        // 【修正点 2】：如果 e 不来，下属可以来也可以不来，选大的
        bu += MAX(nextInfo.laiMaxHappy, nextInfo.buMaxHappy);
    }
    return (Info){lai, bu};
}
