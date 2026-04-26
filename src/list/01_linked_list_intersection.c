#include "comparator.h"

/*
 * 【题目】给定两个可能有环也可能无环的单链表，头节点head1和head2。请实
现一个函数，如果两个链表相交，请返回相交的第一个节点。如果不相交，返回null

【要求】如果两个链表长度之和为N，时间复杂度请达到O(N)，额外空间复杂度
请达到0（1）。
 */
struct snode {
    int val;
    struct snode *next;
};

// 检查链表有环无环 哈希表 需要额外空间

// 检查链表有环无环 快慢指针
int has_cycle(struct snode *head) {
    struct snode *slow = head;
    struct snode *fast = head;
    // 因为fast每次走两步，所以必须保证fast和fast->next都不是NULL，才能安全的走两步
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            return 1;
        }
    }
    return 0;
}

// 找到环的入口节点
struct snode *find_cycle_entry(struct snode *head) {
    struct snode *slow = head;
    struct snode *fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        // 第二步：一个从头走，一个从相遇点走，再次相遇就是入口
        if (slow == fast) {
            slow = head;
            while (slow != fast) {
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
    }
    return NULL;
}

// 两个单链表（有环或无环相交问题）
struct snode *linked_list_intersection(struct snode *head1, struct snode *head2) {
    if (head1 == NULL || head2 == NULL) {
        return NULL;
    }
    int len1 = 1;
    int len2 = 1;
    struct snode *end1 = head1;
    struct snode *end2 = head2;
    while (end1->next != NULL) {
        end1 = end1->next;
        len1++;
    }
    while (end2->next != NULL) {
        end2 = end2->next;
        len2++;
    }
    if (end1 != end2) {
        return NULL;
    }

    struct snode *longer = len1 > len2 ? head1 : head2;
    struct snode *shorter = len1 > len2 ? head2 : head1;

    int diff = len1 > len2 ? len1 - len2 : len2 - len1;
    /*
     * int n = len1 - len2
     * n >> 31 取符号位，正数得 0，负数得 -1（全1），异或再减就翻转了符号。
     * int diff = (n ^ (n >> 31) - (n >> 31));
     */

    while (diff-- > 0) {
        longer = longer->next;
    }
    while (longer != shorter) {
        longer = longer->next;
        shorter = shorter->next;
    }
    return longer;
}

// 双指针法
/*
 * a 的路径: 1→2→3→6→7 → 4→5→6
 * b 的路径: 4→5→6→7 → 1→2→3→6
 * a 走了: len(A) + len(B) 的前一部分
 * b 走了: len(B) + len(A) 的前一部分
 */
struct snode *linked_list_intersection_by_2ptr(struct snode *head1, struct snode *head2) {
    if (head1 == NULL || head2 == NULL) {
        return NULL;
    }
    struct snode *a = head1;
    struct snode *b = head2;

    while (a != b) {
        a = (a != NULL) ? a->next : head2;
        b = (b != NULL) ? b->next : head1;
    }
    return a;
}

// 两个有环链表， 返回第一个相交节点， 如果不相交返回NULL
struct snode *linked_list_both_loop(struct snode *head1, struct snode *head2) {
    struct snode *loop1 = find_cycle_entry(head1);
    struct snode *loop2 = find_cycle_entry(head2);
    // 如果环节点相等，不代表前面没有相交节点
    if (loop1 == loop2) {
        struct snode *cur1 = head1;
        struct snode *cur2 = head2;

        while (cur1 != cur2) {
            cur1 = (cur1 != loop1) ? cur1->next : head2;
            cur2 = (cur2 != loop2) ? cur2->next : head1;
        }
        return cur1;
    } else {
        /*
         * 如果环节点不相等， 说明在环节点前面没有相交
         */
        struct snode *cur = loop1->next;
        while (cur != loop1) {
            if (cur == loop2) {
                return loop1; // 在同一个环上，loop1 或 loop2 都算交点
            }
            cur = cur->next;
        }
        return NULL;
    }
}