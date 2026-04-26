#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * 生成 next 数组 (左神风格)
 * next[i] 表示 i 位置之前的字符串的最长相等前后缀长度
 */
int *getNextArray(char *ms, int mLen) {
    if (mLen == 1) {
        int *next = (int *) malloc(sizeof(int));
        next[0] = -1;
        return next;
    }
    int *next = (int *) malloc(sizeof(int) * mLen);
    next[0] = -1; // 规定 0 位置为 -1
    next[1] = 0; // 规定 1 位置为 0

    int i = 2; // 当前在哪个位置求 next 值
    int cn = 0; // cn是指哪个位置的字符跟i - 1位置的字符进行比较

    while (i < mLen) {
        if (ms[i - 1] == ms[cn]) {
            // 情况一：配上了，长度加 1，继续下一个
            next[i++] = ++cn;
        } else if (cn > 0) {
            // 情况二：没配上，但还能往前跳
            cn = next[cn];
        } else {
            // 情况三：跳到头了也没配上
            next[i++] = 0;
        }
    }
    return next;
}

/**
 * KMP 主函数
 * 返回匹配成功的起始索引，失败返回 -1
 */
int getIndexOf(char *s, char *m) {
    if (s == NULL || m == NULL || strlen(m) < 1 || strlen(s) < strlen(m)) {
        return -1;
    }

    int sLen = strlen(s);
    int mLen = strlen(m);
    int *next = getNextArray(m, mLen);

    int i1 = 0; // 主串指针
    int i2 = 0; // 模式串指针

    // i1 不回头的核心循环
    while (i1 < sLen && i2 < mLen) {
        if (s[i1] == m[i2]) {
            // 配对了，两个指针一起走
            i1++;
            i2++;
        } else if (next[i2] == -1) {
            // 也就是 i2 == 0 且配不上
            // 模式串已经退无可退了，主串往前走
            i1++;
        } else {
            // 模式串根据 next 数组跳跃
            i2 = next[i2];
        }
    }
    free(next);
    // 如果 i2 走到了最后，说明全配上了
    return i2 == mLen ? i1 - i2 : -1;
}

int main() {
    char *str = "abcabcababaccc";
    char *match = "ababac";

    int index = getIndexOf(str, match);
    printf("匹配到的起始索引是: %d\n", index); // 预期输出 6

    return 0;
}
