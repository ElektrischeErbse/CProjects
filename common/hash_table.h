#pragma once

#include "linked_list.h"

typedef struct HashNode {
    void *key;
    void *value;
} HashNode;

typedef size_t (*HashFunc)(const void *key);
typedef int (*EqualData)(const void *lsh, const void *rsh);
typedef void (*FreeKey)(void *key);
typedef void (*FreeValue)(void *value);
typedef void (*Visit)(void *key, void *value, void *arg);

typedef struct HashTable {
    LinkedList **table;
    size_t capacity;
    size_t size;

    HashFunc hash_func;
    FreeKey free_key;
    FreeValue free_value;
} HashTable;

HashTable *create_hash_table(size_t capacity, HashFunc hash_func, EqualData equal_data, FreeKey free_key, FreeValue free_value);

int hash_table_put(HashTable *ht, void *key, void *value);

void *hash_table_get(HashTable *ht, const void *key);

int hash_table_remove(HashTable *ht, const void *key);

void destroy_hash_table(HashTable **ht);

void hash_table_traverse(HashTable *ht, Visit visit, void *arg);

size_t hash_table_size(HashTable *ht);