#include "hashmap.h"
#include "queue.h"

typedef struct Entry {
    void *key;
    void *value;
    struct Entry *next;
} Entry;

struct HashMap {
    Entry **buckets;
    int capacity;
    int size;
    float load_factor;
    HashFn hash;
    EqualFn equal;
};

struct HashMapIter {
    HashMap *map;
    int bucket_idx;
    Entry *node;
    void *key;
    void *value;
};

HashMap *hashmap_create(int capacity, HashFn hash, EqualFn equal) {
    HashMap *map = malloc(sizeof(HashMap));
    map->capacity = capacity > 0 ? capacity : 16;;
    map->size = 0;
    map->load_factor = 0.75f;
    map->hash = hash;
    map->equal = equal;
    map->buckets = calloc(capacity, sizeof(Entry *));
    return map;
}

static void rehash(HashMap *map) {
    int old_cap = map->capacity;
    Entry **old_buckets = map->buckets;

    map->capacity = old_cap * 2;
    map->buckets = calloc(map->capacity, sizeof(Entry *));
    map->size = 0;

    for (int i = 0; i < old_cap; i++) {
        Entry *entry = old_buckets[i];
        while (entry) {
            Entry *next = entry->next;
            unsigned int idx = map->hash(entry->key) % map->capacity;
            entry->next = map->buckets[idx];
            map->buckets[idx] = entry;
            map->size++;
            entry = next;
        }
    }
    free(old_buckets);
}

void hashmap_put(HashMap *map, const void *key, size_t key_size,
                 const void *val, size_t val_size) {
    if ((float) map->size / map->capacity > map->load_factor)
        rehash(map);

    unsigned int idx = map->hash(key) % map->capacity;
    Entry *entry = map->buckets[idx];

    /* key 已存在则更新 value */
    while (entry) {
        if (map->equal(entry->key, key)) {
            void *new_val = malloc(val_size);
            memcpy(new_val, val, val_size);
            free(entry->value);
            entry->value = new_val;
            return;
        }
        entry = entry->next;
    }

    /* 新建 entry，key 和 value 都复制一份 */
    Entry *e = malloc(sizeof(Entry));
    e->key = malloc(key_size);
    e->value = malloc(val_size);
    memcpy(e->key, key, key_size);
    memcpy(e->value, val, val_size);
    e->next = map->buckets[idx];
    map->buckets[idx] = e;
    map->size++;
}

void *hashmap_get(HashMap *map, const void *key) {
    unsigned int idx = map->hash(key) % map->capacity;
    Entry *entry = map->buckets[idx];
    while (entry) {
        if (map->equal(entry->key, key)) return entry->value;
        entry = entry->next;
    }
    return NULL;
}

int hashmap_contains(HashMap *map, const void *key) {
    return hashmap_get(map, key) != NULL;
}

void hashmap_delete(HashMap *map, const void *key) {
    unsigned int idx = map->hash(key) % map->capacity;
    Entry **cur = &map->buckets[idx];
    while (*cur) {
        if (map->equal((*cur)->key, key)) {
            Entry *tmp = *cur;
            *cur = (*cur)->next;
            free(tmp->key);
            free(tmp->value);
            free(tmp);
            map->size--;
            return;
        }
        cur = &(*cur)->next;
    }
}

int hashmap_size(HashMap *map) {
    return map->size;
}

void hashmap_free(HashMap *map) {
    for (int i = 0; i < map->capacity; i++) {
        Entry *entry = map->buckets[i];
        while (entry) {
            Entry *tmp = entry;
            entry = entry->next;
            free(tmp->key);
            free(tmp->value);
            free(tmp);
        }
    }
    free(map->buckets);
    free(map);
}

/* hashmap.c 实现 */
void hashmap_free_with(HashMap *map, void (*free_value)(void *)) {
    for (int i = 0; i < map->capacity; i++) {
        Entry *entry = map->buckets[i];
        while (entry) {
            Entry *tmp = entry;
            entry = entry->next;
            free(tmp->key);
            if (free_value) free_value(tmp->value); /* 释放 value */
            free(tmp);
        }
    }
    free(map->buckets);
    free(map);
}

HashMapIter *hashmap_iter_create(HashMap *map) {
    HashMapIter *iter = malloc(sizeof(HashMapIter));
    iter->map = map;
    iter->bucket_idx = 0;
    iter->node = NULL;
    iter->key = NULL;
    iter->value = NULL;
    return iter;
}

int hashmap_iter_next(HashMapIter *iter) {
    HashMap *map = iter->map;

    if (iter->node && iter->node->next) {
        iter->node = iter->node->next;
        iter->key = iter->node->key;
        iter->value = iter->node->value;
        return 1;
    }
    /*
     * 调用hashmap->iter->next()
     * 判断iter->node是否为NULL
     * node有效 --> 看node->next
     *         node非NULL --> 走链表返回1
     *         node为NULL ---> 链表走完，去找下一个节点
     * node无效 --> 说明node是刚初始化
     *          桶索引不变，从当前bucket_idx开始找
     */
    int i = iter->node ? iter->bucket_idx + 1 : iter->bucket_idx;
    while (i < map->capacity) {
        if (map->buckets[i]) {
            iter->bucket_idx = i;
            iter->node = map->buckets[i];
            iter->key = iter->node->key;
            iter->value = iter->node->value;
            return 1;
        }
        i++;
    }
    iter->node = NULL;
    iter->key = NULL;
    iter->value = NULL;
    return (0);
}

void *hashmap_iter_key(HashMapIter *iter) { return iter->key; }
void *hashmap_iter_value(HashMapIter *iter) { return iter->value; }
void hashmap_iter_free(HashMapIter *iter) { free(iter); }

/* ═══════════════════════════════════════════
   以下是各种类型的 hash / equal 函数
   ═══════════════════════════════════════════ */

/* int */
unsigned int hash_int(const void *key) {
    int k = *(int *) key;
    k = ((k >> 16) ^ k) * 0x45d9f3b; /* 扰动，让分布更均匀 */
    k = ((k >> 16) ^ k);
    return (unsigned int) k;
}

int equal_int(const void *a, const void *b) {
    return *(int *) a == *(int *) b;
}

/* float */
unsigned int hash_float(const void *key) {
    unsigned int k;
    memcpy(&k, key, sizeof(float)); /* 把 float 的位模式当 int 来 hash */
    return hash_int(&k);
}

int equal_float(const void *a, const void *b) {
    return *(float *) a == *(float *) b;
}

/* 字符串 */
unsigned int hash_str(const void *key) {
    const char *s = *(const char **) key; /* key 是 char** */
    unsigned int h = 5381;
    while (*s) h = h * 33 + (unsigned char) *s++;
    return h;
}

int equal_str(const void *a, const void *b) {
    return strcmp(*(const char **) a, *(const char **) b) == 0;
}

unsigned int hash_bytes(const void *key) {
    const BytesKey *bk = key;
    const unsigned char *p = bk->data;
    unsigned int h = 5381;
    for (size_t i = 0; i < bk->size; i++)
        h = h * 33 + p[i];
    return h;
}

int equal_bytes(const void *a, const void *b) {
    const BytesKey *ba = a;
    const BytesKey *bb = b;
    if (ba->size != bb->size) return 0;
    return memcmp(ba->data, bb->data, ba->size) == 0;
}

unsigned int hash_ptr(const void *key) {
    uintptr_t p = *(uintptr_t *) key;
    p = ((p >> 16) ^ p) * 0x45d9f3b;
    p = ((p >> 16) ^ p);
    return (unsigned int) p;
}

int equal_ptr(const void *a, const void *b) {
    return *(uintptr_t *) a == *(uintptr_t *) b;
}
