#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─── 接口头文件（hashmap.h）─── */
typedef struct HashMap HashMap;

/* 用这个包装需要按字节比较的 key */
typedef struct {
    const void *data;
    size_t size;
} BytesKey;

/* 不透明指针，外部只知道有这个类型，不知道内部结构 */
typedef struct HashMapIter HashMapIter;

/* 只暴露函数 */
HashMapIter *hashmap_iter_create(HashMap *map);

int hashmap_iter_next(HashMapIter *iter);

void *hashmap_iter_key(HashMapIter *iter);

void *hashmap_iter_value(HashMapIter *iter);

void hashmap_iter_free(HashMapIter *iter);

/* 用户需要提供的函数 */
typedef unsigned int (*HashFn)(const void *key);

typedef int (*EqualFn)(const void *a, const void *b);

typedef void (*FreeFn)(void *ptr);

HashMap *hashmap_create(int capacity, HashFn hash, EqualFn equal);

void hashmap_put(HashMap *map, const void *key, size_t key_size,
                 const void *val, size_t val_size);

void *hashmap_get(HashMap *map, const void *key);

int hashmap_contains(HashMap *map, const void *key);

void hashmap_delete(HashMap *map, const void *key);

int hashmap_size(HashMap *map);

void hashmap_free(HashMap *map);

void hashmap_free_with(HashMap *map, void (*free_value)(void *));

/* ─── 内置 hash / equal ─── */
unsigned int hash_int(const void *key);

unsigned int hash_float(const void *key);

unsigned int hash_str(const void *key);

unsigned int hash_bytes(const void *key); /* key 是 BytesKey* */

int equal_int(const void *a, const void *b);

int equal_float(const void *a, const void *b);

int equal_str(const void *a, const void *b);

int equal_bytes(const void *a, const void *b); /* key 是 BytesKey* */

unsigned int hash_ptr(const void *key);

int equal_ptr(const void *a, const void *b);

#define BYTES_KEY(val) \
((BytesKey){ &(val), sizeof(val) })

/* ─── 便利宏：省去每次写 sizeof ─── */
#define MAP_PUT(map, key, val) \
    hashmap_put(map, &(key), sizeof(key), &(val), sizeof(val))

#define MAP_GET(map, key, type) \
    ((type *)hashmap_get(map, &(key)))

#define MAP_DEL(map, key) \
    hashmap_delete(map, &(key))

#define MAP_HAS(map, key) \
    hashmap_contains(map, &(key))
