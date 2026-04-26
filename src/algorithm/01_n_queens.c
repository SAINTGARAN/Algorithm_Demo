#include <stdio.h>
#include <stdlib.h>

static int isValue(int *record, int i, int j) {
    for (int k = 0; k < i; k++) {
        // 之前的某个K行的皇后
        /* 不共列 列坐标的绝对值 = 之前行坐标的绝对值 */
        if (j == record[k] || abs(record[k] - j) == abs(i - k)) {
            return 0;
        }
    }
    return 1;
}

// record[0, i - 1]表示之前的行，放了皇后的位置
// n 代表整体一共有多少行
// 返回值是，拜完所有的皇后，合理的摆法有多少种

static int process1(int i, int *record, int n) {
    if (i == n) {
        // 终止行
        return 1;
    }
    int res = 0;
    for (int j = 0; j < n; j++) {
        if (isValue(record, i, j)) {
            record[i] = j; // 第i行的皇后放在了第j列
            res += process1(i + 1, record, n);
        }
    }
    return res;
}

static int num1(int n) {
    if (n < 1) return 0;

    int record[n];
    return process1(0, record, n);
}


// 32皇后问题
/*
 * 列限制：哪些列放了皇后
 * 左斜线限制：那些位置处于之前皇后的左下方的攻击线上
 * 右斜线限制：那些位置处于之前皇后的右下方的攻击线上
 */
static int process2(int limits, int colLim, int leftDiaLim, int rightDiaLim) {
    /*
    * 如果 colLim 变得和 limits 一模一样，说明二进制中的那 n个位置全部变成了 1。
既然每一位都是 1，意味着 n个不同的列都已经有了皇后。
因为我们是一行一行放的，既然 n个不同的列都有了皇后，那肯定也已经放满了n行。
     */
    if (colLim == limits) {
        // 所有列都放满了皇后
        return 1;
    }
    // 1. 计算当前行哪些位置可以放皇后
    // (colLim | leftDiaLim | rightDiaLim) 是所有被攻击的列
    // 取反(~)后，1 表示可以放的位置
    // & limits 是为了把高位的 1 去掉，只留棋盘范围内的
    int pos = limits & (~(colLim | leftDiaLim | rightDiaLim));

    int res = 0;
    while (pos != 0) {
        // 2. 提取出最右边的那个 1（即尝试放一个皇后）
        int mostRightOne = pos & (~pos + 1);

        // 3. 把这个位置从可选列表中减去
        pos = pos - mostRightOne;

        // 4. 递归到下一行，更新限制条件
        res += process2(limits,
                        colLim | mostRightOne, // 列限制更新
                        (leftDiaLim | mostRightOne) << 1, // 左斜线限制更新
                        (rightDiaLim | mostRightOne) >> 1); // 右斜线限制更新
    }
    return res;
}

static int num2(int n) {
    // 限制 n 在 1 到 32 之间，因为 int 通常只有 32 位
    if (n < 1 || n > 32) {
        return 0;
    }

    // 生成一个低 n 位全为 1 的掩码
    // 如果 n = 8，limits 为二进制 11111111
    // 如果 n = 32，1 << 32 会溢出，所以特判处理
    int limits = (n == 32) ? -1 : (1 << n) - 1;

    return process2(limits, 0, 0, 0);
}
