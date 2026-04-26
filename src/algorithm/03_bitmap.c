#include <stdio.h>
#include <Stdlib.h>

static int process() {
    int *num = (int *) malloc(32 * sizeof(int));
    if (!num) {
        printf("malloc failed\n");
    }
    int i = 178;

    int index = i / 32;
    int bitIndex = i % 32;
    // 拿到第178位的状态
    // 去除高32 - bitIndex
    int status = (num[index] >> bitIndex) & 1;

    // 状态改为1
    num[index] |= 1 << bitIndex;
    // 状态改为0
    num[index] &= ~(1 << bitIndex);
}

// 布隆过滤器

/*
映射阶段：利用 k个不同的哈希函数，将输入数据（URL）映射到一个长度为 m的位图（Bitset）中的 k个点。
取模运算：hash(url) % m 是为了确保哈希值落在位图的有效索引范围内。
判定准则：
100% 确定不存在：只要这 k个位置中有一个为 0，说明该数据一定没有被存入过（因为如果存过，这 k个位置必然全被置为了 1）。
可能存在：如果 k个位置全为 1，说明该数据可能在黑名单中（但也可能是因为其他多个数据产生的哈希冲突，把这几个位置凑巧都涂黑了，这被称为“假阳性”或“误判”）。
 */
static void bloom() {
}
