#include <stdbool.h>
#include <stdio.h>
#define max(a, b) (((a) > (b)) ? (a) : (b))

/*输入：
正数数组costs
正数数组profits
正数k
正数m
含义：
costs[i]表示i号项目的花费
profits[i]表示i号项目在扣除花费之后还能挣到的钱（利润）
k表示你只能串行的最多做k个项目
m表示你初始的资金
说明：
你每做完一个项目，马上获得的收益，可以支持你去做下一个项目。
输出：
你最后获得的最大钱数。*/

/*
 * 不使用堆的核心思路
 * 既然不能自动弹出最大值， 手动寻找当前能做的项目中利润最大的那一个
 *
 */
int get_max_benefit(int costs[], int costs_len, int profits[], int profits_len, int k, int m) {
    // 1. 创建一个标记数组，记录项目是否已经被做过
    // 这里使用 C99 标准的变长数组，或者可以使用 malloc 分配
    bool isUsed[costs_len];
    for (int i = 0; i < costs_len; i++) {
        isUsed[i] = false;
    }

    int current_capital = m;

    for (int i = 0; i < k; i++) {
        int best_project_idx = -1;
        int max_profit = -1;
        for (int j = 0; j < costs_len; j++) {
            // 如果当前项目未使用过，且花费小于等于当前拥有的钱
            if (!isUsed[j] && costs[j] <= current_capital) {
                // 贪心策略， 记录利润最大的那个
                if (profits[j] > max_profit) {
                    max_profit = profits[j];
                    best_project_idx = j;
                }
            }
        }

        // 判断本轮是否找到了合适的项目， 走到这里代表
        if (best_project_idx != -1) {
            // 找到了，更新资金，标记该项目已完成
            current_capital += profits[best_project_idx];
            isUsed[best_project_idx] = true;
        } else {
            // 如果扫完一遍发现一个买得起的都没有，提前退出
            break;
        }
    }
    return current_capital;
}

// 小根堆，大根堆实现

int main() {
    // 测试用例
    int costs[] = {0, 1, 1};
    int profits[] = {1, 2, 3};
    int k = 2;
    int m = 0;
    int n = sizeof(costs) / sizeof(costs[0]);

    int result = get_max_benefit(costs, n, profits, n, k, m);

    printf("初始资金: %d\n", m);
    printf("最大可做项目数: %d\n", k);
    printf("最终最大收益: %d\n", result);

    return 0;
}
