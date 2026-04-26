#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 字符串str中，最长回文子串的长度如何算（Manacher算法）
 * 如何做到时间复杂度O(n)
 */

// 中心扩散法
static char *getString(char *s) {
    int sLen = strlen(s);
    int new_len = 2 * sLen + 1;
    char *res = (char *) malloc(new_len + 1);
    for (int i = 0; i < new_len; i++) {
        res[i] = (i % 2 == 0) ? '#' : s[i / 2];
    }
    res[new_len] = '\0';
    return res;
}

static char *getMaxSubstringLength(char *s) {
    char *s1 = getString(s);
    int s1Len = strlen(s1);
    int *result = (int *) malloc(sizeof(int) * (s1Len + 1));

    int maxLen = 0, center = 0;

    for (int i = 1; i < s1Len - 1; i++) {
        int j = i - 1, k = i + 1;
        while (j >= 0 && k < s1Len && s1[j] == s1[k]) {
            j--;
            k++;
        }
        result[i] = (k - j - 2) / 2; // 回归原始字符串的回文长度

        if (result[i] > maxLen) {
            maxLen = result[i];
            center = i;
        }
    }

    // 还原到原始字符串的起始位置
    int start = (center - maxLen) / 2;
    char *res = (char *) malloc(maxLen + 1);
    strncpy(res, s + start, maxLen);
    res[maxLen] = '\0';

    free(s1);
    free(result);
    return res;
}

/*
i 在 r 外：          p[i] = 0，老老实实扩展
i 在 r 内且镜像没超出：p[i] = p[mirror]，直接用，不用扩展
i 在 r 内但镜像超出：  p[i] = r - i，从边界继续扩展
 *
 */
static char *manacher(char *s) {
    char *t = getString(s);
    int tLen = strlen(t);
    int *p = (int *) calloc(tLen, sizeof(int)); // p[i] = 以i为中心的回文半径

    int c = 0, r = 0; //当前最右回文的中心点和右边界
    int maxLen = 0, center = 0;

    for (int i = 0; i < tLen; i++) {
        if (i < r) {
            // 当前字符串在上一个字符串的右边界里面，找到i的对称点
            int mirror = 2 * c - i;
            p[i] = (r - i) < p[mirror] ? r - i : p[mirror];
        }

        // 在已知基础上继续扩展
        int j = i - p[i] - 1; // i往左扩
        int k = p[i] + 1; // i往右扩
        while (j >= 0 && k < tLen && t[j] == t[k]) {
            p[i]++;
            j--;
            k++;
        }

        // 更新最右边界
        if (i + p[i] > r) {
            c = i;
            r = i + p[i];
        }

        // 记录最长回文
        if (p[i] > maxLen) {
            maxLen = p[i];
            center = i;
        }
    }

    // 还原回原始串起始位置
    int start = (center - maxLen) / 2;
    char *res = (char *) malloc(maxLen + 1);
    strncpy(res, s + start, maxLen);
    res[maxLen] = '\0';

    free(t);
    free(p);
    return res;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 取两个整数中的最小值
#define MIN(a, b) ((a) < (b) ? (a) : (b))

void manacher_fix(char *s) {
    int n = strlen(s); // 获取原始字符串长度
    if (n == 0) return; // 如果字符串为空则直接返回

    // 1. 预处理：将 "aba" 变为 "$#a#b#a#@"，解决奇偶长度对称中心不统一的问题
    int new_len = 2 * n + 3; // 计算处理后的字符串长度（原字符 + #分隔符 + 两个边界哨兵）
    char *t = (char *) malloc(new_len * sizeof(char)); // 为处理后的字符串申请内存
    t[0] = '$'; // 开头放入哨兵 $，防止向左匹配时越界
    for (int i = 0; i < n; i++) {
        t[2 * i + 1] = '#'; // 字符间填充 #
        t[2 * i + 2] = s[i]; // 放入原始字符
    }
    t[2 * n + 1] = '#'; // 最后一个有效字符后补 #
    t[2 * n + 2] = '\0'; // C 语言字符串结束符（注：实际逻辑中可用哨兵 @ 代替避免检查越界）

    // 2. 定义辅助数组 P，P[i] 存储以 i 为中心的回文半径（包含中心自身）
    int *P = (int *) calloc(new_len, sizeof(int));
    int C = 0; // 当前探测到的最右回文区域的“中心位置”
    int R = 0; // 当前探测到的最右回文区域的“右边界位置” (C + P[C])

    int max_len = 0; // 记录全局最长回文子串的长度
    int center_index = 0; // 记录最长回文子串在预处理字符串 t 中的中心位置

    // 3. 线性扫描预处理后的字符串
    for (int i = 1; i < new_len - 1; i++) {
        // 计算 i 关于当前中心 C 的对称点 i_mirror
        int i_mirror = 2 * C - i;

        // 如果 i 在右边界 R 之内，利用对称性初始化 P[i]
        if (i < R) {
            // P[i] 至少是对称点半径 P[i_mirror] 和 距离边界距离 (R - i) 的最小值
            P[i] = MIN(R - i, P[i_mirror]);
        } else {
            // 如果 i 超出了 R，半径初始设为 1（即它自己）
            P[i] = 1;
        }

        // 4. 中心扩展法：基于已有的 P[i] 向两侧尝试扩展，直到字符不匹配或触碰边界
        while (t[i + P[i]] == t[i - P[i]]) {
            P[i]++; // 匹配成功，半径增加
        }

        // 5. 更新中心 C 和右边界 R
        // 如果当前回文的覆盖范围超过了旧的 R，则更新 R 和 C
        if (i + P[i] > R) {
            C = i;
            R = i + P[i];
        }

        // 6. 维护最长回文的长度和中心
        // P[i] - 1 刚好等于原始字符串中回文子串的长度
        if (P[i] - 1 > max_len) {
            max_len = P[i] - 1;
            center_index = i;
        }
    }

    // 7. 打印结果：根据中心和半径推算出原字符串中的起始位置
    int start = (center_index - max_len) / 2; // 起始下标换算公式
    printf("原字符串: %s\n", s);
    printf("最长回文子串: ");
    for (int i = start; i < start + max_len; i++) {
        printf("%c", s[i]); // 逐字符打印结果
    }
    printf("\n长度: %d\n", max_len);

    // 释放动态申请的内存
    free(t);
    free(P);
}

int main() {
    char s[] = "abbahoyoyoh"; // 测试用例
    manacher(s); // 执行算法
    return 0;
}
