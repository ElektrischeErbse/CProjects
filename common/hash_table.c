#include "hash_table.h"
#include <stddef.h>
#include <stdlib.h>

static HashNode *create_hash_node(void *key, void *value)
{
    HashNode *node = malloc(sizeof(HashNode));
    if (node == NULL) {
        return NULL;
    }
    node->key = key;
    node->value = value;
    return node;
}

static void free_hash_node(void *data, FreeKey free_key, FreeValue free_value)
{
    if (data == NULL) {
        return;
    }
    HashNode *node = (HashNode *) data;
    if (free_key) {
        free_key(node->key);
    }
    if (free_value) {
        free_value(node->value);
    }
    free(node);
}

// 内部：链表遍历用（释放节点）
static void handle_free(void *data, void *arg)
{
    HashTable *ht = (HashTable *) arg;
    free_hash_node(data, ht->free_key, ht->free_value);
}

HashTable *create_hash_table(size_t capacity, HashFunc hash_func, EqualData equal_data, FreeKey free_key, FreeValue free_value)
{
    if (capacity == 0 || !hash_func || !equal_data) {
        return NULL;
    }

    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) {
        return NULL;
    }

    ht->table = malloc(sizeof(LinkedList *) * capacity);
    if (!ht->table) {
        free(ht);
        return NULL;
    }

    for (size_t i = 0; i < capacity; i++) {
        ht->table[i] = create_linked_list(NULL, NULL, equal_data);
        if (!ht->table[i]) {
            for (size_t j = 0; j < i; j++) {
                destroy_linked_list(&ht->table[j]);
            }
            free(ht->table);
            free(ht);
            return NULL;
        }
    }

    ht->capacity = capacity;
    ht->size = 0;
    ht->hash_func = hash_func;
    ht->free_key = free_key;
    ht->free_value = free_value;
    return ht;
}

int hash_table_put(HashTable *ht, void *key, void *value)
{
    if (!ht || !key) {
        return -1;
    }

    size_t idx = ht->hash_func(key) % ht->capacity;
    LinkedList *list = ht->table[idx];

    // 查找是否已存在
    HashNode arg = {key, value};
    ListNode *node = list_find(list, &arg);

    if (node) {
        // 更新：释放旧值
        HashNode *old = (HashNode *) node->data;
        if (ht->free_value) {
            ht->free_value(old->value);
        }
        old->value = value;
        return 0;
    }

    // 插入新节点
    HashNode *hnode = create_hash_node(key, value);
    if (!hnode) {
        return -1;
    }

    if (list_push_back(list, hnode) != 0) {
        free(hnode);
        return -1;
    }

    ht->size++;
    return 0;
}

void *hash_table_get(HashTable *ht, const void *key)
{
    if (!ht || !key) {
        return NULL;
    }

    size_t idx = ht->hash_func(key) % ht->capacity;
    LinkedList *list = ht->table[idx];

    HashNode arg = {(void *) key, NULL};
    ListNode *node = list_find(list, &arg);
    if (!node) {
        return NULL;
    }

    return ((HashNode *) node->data)->value;
}

int hash_table_remove(HashTable *ht, const void *key)
{
    if (!ht || !key) {
        return -1;
    }

    size_t idx = ht->hash_func(key) % ht->capacity;
    LinkedList *list = ht->table[idx];

    HashNode arg = {(void *) key, NULL};
    ListNode *node = list_find(list, &arg);
    if (!node) {
        return -1;
    }

    // 从链表摘除
    ListNode *prev = node->prev, *next = node->next;
    prev->next = next;
    next->prev = prev;

    // 释放
    free_hash_node(node->data, ht->free_key, ht->free_value);
    free(node);
    list->size--;
    ht->size--;
    return 0;
}

void destroy_hash_table(HashTable **ht)
{
    if (!ht || !*ht) {
        return;
    }
    HashTable *h = *ht;

    for (size_t i = 0; i < h->capacity; i++) {
        traversal_linked_list_ex(h->table[i], handle_free, h);
        destroy_linked_list(&h->table[i]);
    }

    free(h->table);
    free(h);
    *ht = NULL;
}

typedef struct {
    Visit visit;
    void *arg;
} TraverseArg;

static void traverse_handle(void *data, void *arg)
{
    TraverseArg *targ = (TraverseArg *) arg;
    HashNode *node = (HashNode *) data;
    targ->visit(node->key, node->value, targ->arg);
}

void hash_table_traverse(HashTable *ht, Visit visit, void *arg)
{
    if (!ht || !visit) return;
    TraverseArg targ = {visit, arg};
    for (size_t i = 0; i < ht->capacity; i++) {
        traversal_linked_list_ex(ht->table[i], traverse_handle, &targ);
    }
}

size_t hash_table_size(HashTable *ht)
{
    return ht ? ht->size : 0;
}