#include <stdio.h>
#include <stdlib.h>

// 贪心算法——安排最大会议数量

/*贪心策略核心思想
问题定义：给定 n 个会议，每个会议有开始时间和结束时间，同一时间只能举办一个会议，目标是安排尽可能多的会议。
*/

typedef struct {
    int id; /* 会议编号（显示用）     */
    char name[16]; /* 会议名称（可选标签）   */
    int start; /* 开始时间               */
    int end; /* 结束时间（不含，左闭右开）*/
} Meeting;

/*
 * 小于0， a在b前面
 * 大于0， b在a前面
 */
static int cmp_by_end(const void *a, const void *b) {
    const Meeting *meetingA = *(Meeting **) a;
    const Meeting *meetingB = *(Meeting **) b;
    if (meetingA->end != meetingB->end) {
        return meetingA->end - meetingB->end; // 如果结束时间不相等，结束时间小的在前面
    }
    return meetingA->start - meetingB->start; // 结束时间相等，按起始时间早的先排序
}

int greedy_max_meetings(Meeting **meetings, int max_meetings, int selected[]) {
    if (max_meetings <= 0) {
        return 0;
    }

    // 按结束时间排序
    qsort(meetings, max_meetings, sizeof(Meeting), cmp_by_end);

    int last_end = -1;
    int count = 0;
    // 线性遍历 -- 贪心算法
    // 如何判断两个会议时间有没有相交
    for (int i = 0; i < max_meetings; i++) {
        /*
         * 关键判断：当前会议开始时间 >= 上次选中的结束时间
         * 即：不重叠（允许首尾相接）
         */
        if (meetings[i]->start > last_end) {
            selected[count++] = i; // 记录会议下标
            last_end = meetings[i]->end;
        }
    }
    return count;
}
