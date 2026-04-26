/*
 * ============================================================
 *  comparator.h — 对数器通用模板（Header-Only）
 * ============================================================
 *
 *  使用方法（3步）：
 *
 *    1. #include "comparator.h"
 *
 *    2. 实现你的暴力解法和优化解法
 *
 *    3. 在 main 里一行宏启动：
 *
 *       ——— 单值返回（int）———
 *       int brute(const int *arr, int len);
 *       int opt(const int *arr, int len);
 *       CMP_RUN_VALUE(轮数, brute, opt);              // 默认小数据
 *       CMP_STRESS_VALUE(轮数, brute, opt);            // 百万级压测
 *       CMP_RUN_VALUE_CFG(轮数, brute, opt, cfg);     // 自定义
 *
 *       ——— 数组原地修改（排序等）———
 *       void brute(int *arr, int len);
 *       void opt(int *arr, int len);
 *       CMP_RUN_ARRAY(轮数, brute, opt);
 *       CMP_STRESS_ARRAY(轮数, brute, opt);
 *       CMP_RUN_ARRAY_CFG(轮数, brute, opt, cfg);
 *
 * ============================================================
 *  典型工作流：先验证正确性，再压测性能
 *
 *  int main(void) {
 *      CMP_RUN_VALUE(500000, brute, opt);    // 小数据 多轮 验逻辑
 *      CMP_STRESS_VALUE(10, brute, opt);     // 百万级 少轮 拼性能
 *  }
 * ============================================================
 *  配置预设：
 *    cmp_default_config()  — 小数据验证（len ≤ 100）
 *    cmp_stress_config()   — 百万级压测（len ~ 50万~100万）
 *    cmp_custom_config()   — 完全自定义
 * ============================================================
 */

#ifndef ALGORITHM_COMPARATOR_H
#define ALGORITHM_COMPARATOR_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int max_len; // 数组最大长度
    int min_len; // 数组最小长度
    int max_val; // 元素最大值
    int min_val; // 元素最小值
    bool non_negative; // true → min_val 自动设为 0
    bool sorted; // 生成后排序
    bool unique; // 元素不重复
} CmpConfig;

/* 1) 正确性验证 — 小数据 + 海量轮次，快速抓 bug */
static inline CmpConfig cmp_default_config(void) {
    return (CmpConfig){
        .max_len = 100,
        .min_len = 1,
        .max_val = 1000,
        .min_val = -1000,
    };
}

/* 2) 性能压测 — 百万级数据，才能拉开 O(n²) vs O(nlogn) */
static inline CmpConfig cmp_stress_config(void) {
    return (CmpConfig){
        .max_len = 1000000,
        .min_len = 500000,
        .max_val = 1000000,
        .min_val = -1000000,
    };
}

/* 3) 完全自定义 */
static inline CmpConfig cmp_custom_config(int min_len, int max_len, int min_val,
                                          int max_val) {
    return (CmpConfig){
        .max_len = max_len,
        .min_len = min_len,
        .max_val = max_val,
        .min_val = min_val,
    };
}

/* ======================== 内部工具 ======================== */

/* rand() 在某些平台只有 15 bit，拼两次保证大范围均匀 */
static inline int cmp__rand_range(int lo, int hi) {
    if (lo > hi) {
        int t = lo;
        lo = hi;
        hi = t;
    }
    unsigned long long r = ((unsigned long long) rand() << 16) ^ (unsigned long long) rand();
    return lo + (int) (r % (unsigned long long) (hi - lo + 1));
}

#ifdef _WIN32
#include <windows.h>

static inline double cmp__now_sec(void) {
    LARGE_INTEGER freq, count;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&count);
    return (double) count.QuadPart / (double) freq.QuadPart;
}
#else
#include <time.h>
static inline double cmp__now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}
#endif

static inline void cmp__print_array(const int *arr, int len) {
    int show = len < 20 ? len : 20;
    printf("  [");
    for (int i = 0; i < show; i++)
        printf("%d%s", arr[i], i < show - 1 ? ", " : "");
    if (len > 20) printf(", ... (共%d个)", len);
    printf("]\n");
}

static int cmp__cmp_asc(const void *a, const void *b) {
    int x = *(const int *) a, y = *(const int *) b;
    return (x > y) - (x < y);
}

static inline int cmp_generate(int **out, const CmpConfig *cfg) {
    int len = cmp__rand_range(cfg->min_len, cfg->max_len);
    int lo = cfg->non_negative ? 0 : cfg->min_val;
    int hi = cfg->max_val;

    *out = (int *) malloc(len * sizeof(int));
    if (!*out) {
        fprintf(stderr, "malloc %d ints failed\n", len);
        exit(1);
    }

    if (cfg->unique) {
        long long range = hi - lo + 1;
        /*
         * 如果值域范围比数组长度还小，就不能生成足够多不重复的元素
         */
        if (range < len) {
            hi = lo + len * 2;
            range = (long long) hi - lo + 1;
        }

        if (range <= 10000000LL) {
            int *pool = (int *) malloc(range * sizeof(int));
            if (!pool) {
                fprintf(stderr, "malloc %d ints failed\n", (int) range);
                exit(1);
            }
            for (int i = 0; i < (int) range; i++) pool[i] = lo + i;
            /*
             * 洗牌算法 从pool中选出不重复的元素
             * j = i + rand() % (range - i)，j 的范围是 [i, range-1]。
             * 每轮 i++ 之后，下一轮 j 从 i+1 开始，之前选好的 [0, i] 就再也访问不到了，自然就不会重复。
             */
            for (int i = 0; i < (int) range; i++) {
                int j = i + rand() % (range - i);
                int t = pool[i];
                pool[i] = pool[j];
                pool[j] = t;
                (*out)[i] = pool[i];
            }
            free(pool);
        } else {
            for (int i = 0; i < len; i++)
                (*out)[i] = cmp__rand_range(lo, hi);
            qsort(*out, len, sizeof(int), cmp__cmp_asc);
            int w = 1;
            for (int i = 1; i < len; i++)
                if ((*out)[i] != (*out)[i - 1]) (*out)[w++] = (*out)[i];
            len = w;
        }
    } else {
        for (int i = 0; i < len; i++)
            (*out)[i] = cmp__rand_range(lo, hi);
    }

    if (cfg->sorted)
        qsort(*out, len, sizeof(int), cmp__cmp_asc);
    return len;
}

/* ======================== 输出格式 ======================== */

static inline void cmp__banner(int rounds, const char *bn,
                               const char *on, const CmpConfig *c) {
    printf("\n╔══════════════════════════════════════════════════╗\n");
    printf("║              对 数 器  (Comparator)              ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║  测试轮数 : %-36d ║\n", rounds);
    printf("║  数组长度 : [%-8d, %-8d]                  ║\n", c->min_len, c->max_len);
    printf("║  值域范围 : [%-8d, %-8d]                  ║\n",
           c->non_negative ? 0 : c->min_val, c->max_val);
    printf("║  暴力解法 : %-36s ║\n", bn);
    printf("║  优化解法 : %-36s ║\n", on);
    printf("╚══════════════════════════════════════════════════╝\n\n");
}

static inline void cmp__result(bool ok, int rounds, double bt, double ot) {
    printf("\n╔══════════════════════════════════════════════════╗\n");
    if (ok)
        printf("║  ✅  全部 %-7d 轮测试通过！Nice!             ║\n", rounds);
    else
        printf("║  ❌  测试未通过，请检查优化算法                 ║\n");
    printf("╠══════════════════════════════════════════════════╣\n");
    printf("║  暴力耗时 : %-12.4f 秒                       ║\n", bt);
    printf("║  优化耗时 : %-12.4f 秒                       ║\n", ot);
    if (ot > 1e-9)
        printf("║  加速比   : %-12.2fx                         ║\n", bt / ot);
    printf("╚══════════════════════════════════════════════════╝\n");
}

/* ============================================================
 *  API 1：单值对比  int func(const int *arr, int len)
 * ============================================================ */

typedef int (*CmpValueFn)(const int *arr, int len);

static inline void cmp_run_value_timed(
    int rounds,
    CmpValueFn brute, const char *bn,
    CmpValueFn opt, const char *on,
    const CmpConfig *cfg) {
    srand((unsigned) time(NULL));
    cmp__banner(rounds, bn, on, cfg);

    double bt = 0, ot = 0;
    bool passed = true;

    for (int r = 0; r < rounds; r++) {
        int *arr = NULL;
        int len = cmp_generate(&arr, cfg);
        int *copy = (int *) malloc(sizeof(int) * len);
        memcpy(copy, arr, sizeof(int) * len);

        double t1 = cmp__now_sec();
        int b = brute(arr, len);
        double t2 = cmp__now_sec();
        int o = opt(copy, len);
        double t3 = cmp__now_sec();

        bt += (t2 - t1);
        ot += (t3 - t2);

        if (b != o) {
            passed = false;
            printf("!!! 第 %d 轮出错 !!!\n", r + 1);
            printf("  数组 (len=%d):\n", len);
            cmp__print_array(arr, len);
            printf("  暴力结果: %d\n", b);
            printf("  优化结果: %d\n", o);
            free(arr);
            free(copy);
            break;
        }
        free(arr);
        free(copy);

        int step = rounds / 5;
        if (step > 0 && (r + 1) % step == 0)
            printf("  进度: %d / %d\n", r + 1, rounds);
    }
    cmp__result(passed, rounds, bt, ot);
}

/* ============================================================
 *  API 2：数组对比  void func(int *arr, int len)
 * ============================================================ */

typedef void (*CmpArrayFn)(int *arr, int len);

static inline void cmp_run_array_impl(
    int rounds,
    CmpArrayFn brute, const char *bn,
    CmpArrayFn opt, const char *on,
    const CmpConfig *cfg) {
    srand((unsigned) time(NULL));
    cmp__banner(rounds, bn, on, cfg);

    double bt = 0, ot = 0;
    bool passed = true;

    for (int r = 0; r < rounds; r++) {
        int *arr1 = NULL;
        int len = cmp_generate(&arr1, cfg);
        int *arr2 = (int *) malloc(sizeof(int) * len);
        int *orig = (int *) malloc(sizeof(int) * len);
        memcpy(arr2, arr1, sizeof(int) * len);
        memcpy(orig, arr1, sizeof(int) * len);

        double t1 = cmp__now_sec();
        brute(arr1, len);
        double t2 = cmp__now_sec();
        opt(arr2, len);
        double t3 = cmp__now_sec();

        bt += (t2 - t1);
        ot += (t3 - t2);

        if (memcmp(arr1, arr2, sizeof(int) * len) != 0) {
            passed = false;
            printf("!!! 第 %d 轮出错 !!!\n", r + 1);
            printf("  原始 (len=%d):\n", len);
            cmp__print_array(orig, len);
            printf("  暴力结果:\n");
            cmp__print_array(arr1, len);
            printf("  优化结果:\n");
            cmp__print_array(arr2, len);
            free(arr1);
            free(arr2);
            free(orig);
            break;
        }
        free(arr1);
        free(arr2);
        free(orig);

        int step = rounds / 5;
        if (step > 0 && (r + 1) % step == 0)
            printf("  进度: %d / %d\n", r + 1, rounds);
    }
    cmp__result(passed, rounds, bt, ot);
}

/* ============================================================
 *  便捷宏  —  #brute 自动变成 "brute" 字符串
 * ============================================================ */

/* 单值 — 小数据验正确性 */
#define CMP_RUN_VALUE(rounds, brute, opt) do {              \
    CmpConfig _c = cmp_default_config();                    \
    cmp_run_value_timed(rounds, brute, #brute,              \
                        opt, #opt, &_c);                    \
} while(0)

/* 单值 — 百万级压测性能 */
#define CMP_STRESS_VALUE(rounds, brute, opt) do {           \
    CmpConfig _c = cmp_stress_config();                     \
    cmp_run_value_timed(rounds, brute, #brute,              \
                        opt, #opt, &_c);                    \
} while(0)

/* 单值 — 自定义配置 */
#define CMP_RUN_VALUE_CFG(rounds, brute, opt, cfg) do {     \
    cmp_run_value_timed(rounds, brute, #brute,              \
                        opt, #opt, &(cfg));                 \
} while(0)

/* 数组 — 小数据验正确性 */
#define CMP_RUN_ARRAY(rounds, brute, opt) do {              \
    CmpConfig _c = cmp_default_config();                    \
    cmp_run_array_impl(rounds, brute, #brute,               \
                       opt, #opt, &_c);                     \
} while(0)

/* 数组 — 百万级压测性能 */
#define CMP_STRESS_ARRAY(rounds, brute, opt) do {           \
    CmpConfig _c = cmp_stress_config();                     \
    cmp_run_array_impl(rounds, brute, #brute,               \
                       opt, #opt, &_c);                     \
} while(0)

/* 数组 — 自定义配置 */
#define CMP_RUN_ARRAY_CFG(rounds, brute, opt, cfg) do {     \
    cmp_run_array_impl(rounds, brute, #brute,               \
                       opt, #opt, &(cfg));                  \
} while(0)
#endif // ALGORITHM_COMPARATOR_H
