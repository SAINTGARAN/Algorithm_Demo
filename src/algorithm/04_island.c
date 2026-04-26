#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

/**
 * 深度优先搜索（DFS）感染函数
 * @param grid: 二维网格
 * @param r: 当前行索引
 * @param c: 当前列索引
 * @param rows: 总行数
 * @param cols: 总列数
 */
void dfs(char **grid, int r, int c, int rows, int cols) {
    // 边界检查：
    // 1. 索引越界（r < 0, r >= rows ...）
    // 2. 当前格子不是陆地 '1'（可能是水 '0' 或已访问过的 '2'）
    if (r < 0 || c < 0 || r >= rows || c >= cols || grid[r][c] != '1') {
        return;
    }

    // 将当前陆地标记为 '2'（表示已访问，防止死循环和重复计算）
    grid[r][c] = '2';

    // 递归感染上下左右四个方向
    dfs(grid, r - 1, c, rows, cols); // 上
    dfs(grid, r + 1, c, rows, cols); // 下
    dfs(grid, r, c - 1, rows, cols); // 左
    dfs(grid, r, c + 1, rows, cols); // 右
}

/**
 * 计算岛屿数量的主函数
 */
int numIslands(char **grid, int gridSize, int *gridColSize) {
    if (gridSize == 0) return 0;

    int rows = gridSize;
    int cols = gridColSize[0];
    int count = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // 当遇到一块未访问的陆地 '1' 时
            if (grid[i][j] == '1') {
                count++; // 岛屿数量加 1
                dfs(grid, i, j, rows, cols); // 开启 DFS 感染该岛屿的所有陆地
            }
        }
    }

    return count;
}

// 并查集实现
typedef struct {
    int *parent; // 父节点数组
    int *size; // 按秩合并优化：记录每个集合的大小
    int sets; // 当前岛屿（集合）的数量
    int cols; // 矩阵列数，用于索引转换
} UnionFind;

// 初始化并查集
UnionFind *createUnionFind(char **grid, int r, int c) {
    UnionFind *uf = (UnionFind *) malloc(sizeof(UnionFind));
    int n = r * c;
    uf->parent = (int *) malloc(sizeof(int) * n);
    uf->size = (int *) malloc(sizeof(int) * n); // 记录每个集合的大小
    uf->sets = 0;
    uf->cols = c;
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (grid[i][j] == '1') {
                // 如果遍历到岛
                int index = i * c + j;
                uf->parent[index] = index;
                uf->size[index] = 1;
                uf->sets++;
            }
        }
    }
    return uf;
}

// 查找根节点（带路径压缩：非递归版防止栈溢出）
int find(UnionFind *uf, int i) {
    int root = i;
    while (root != uf->parent[root]) {
        // 判断一个人的上级是不是自己
        root = uf->parent[root]; // 顺着这个人的上级一直往上找， parten里面存的是上级的索引
    }
    // 路径压缩，走到这里说明已经找到了Boss
    while (i != root) {
        int next = uf->parent[i]; // 当前节点的父节点
        uf->parent[i] = root; // 将当前节点的父节点指向BOSS
        i = next; // 查找当前节点的父节点的父节点
    }
    return root;
}

// 释放内存
void freeUF(UnionFind *uf) {
    free(uf->parent);
    free(uf->size);
    free(uf);
}

// 合并两个位置
void unite(UnionFind *uf, int r1, int c1, int r2, int c2) {
    int a = r1 * uf->cols + c1; // 第一个陆地格子的索引，也就是在patent数组里的下标
    int b = r2 * uf->cols + c2; // 第二个要合并的陆地格子的索引
    int rootA = find(uf, a);
    int rootB = find(uf, b);

    if (rootA != rootB) {
        // 按规模合并：小树并入大树
        if (uf->size[rootA] >= uf->size[rootB]) {
            uf->size[rootA] += uf->size[rootB];
            uf->parent[rootB] = rootA;
        } else {
            uf->size[rootB] += uf->size[rootA];
            uf->parent[rootA] = rootB;
        }
        uf->sets--;
    }
}

// 主算法
int numslands_DSU(char **grid, int r, int c) {
    if (r == 0 || c == 0) return 0;
    UnionFind *uf = createUnionFind(grid, r, c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            if (grid[i][j] == '1') {
                // 当前陆地格子的索引
                // 向右看
                if (j + 1 < c && grid[i][j + 1] == '1') {
                    unite(uf, i, j, i, j + 1);
                }
                // 向下看
                if (i + 1 < r && grid[i + 1][j] == '1') {
                    unite(uf, i, j, i + 1, j);
                }
            }
        }
    }
    int result = uf->sets;
    freeUF(uf);
    return result;
}

// ======= 测试代码 =======
int main() {
    // 定义一个 4x5 的网格
    char row1[] = {'1', '1', '0', '0', '0'};
    char row2[] = {'1', '1', '0', '0', '0'};
    char row3[] = {'0', '0', '1', '0', '0'};
    char row4[] = {'0', '0', '0', '1', '1'};

    char *grid[] = {row1, row2, row3, row4};
    int gridSize = 4;
    int gridColSize[] = {5, 5, 5, 5};

    int result = numslands_DSU(grid, gridSize, gridColSize[0]);
    printf("岛屿的数量是: %d\n", result);

    return 0;
}
