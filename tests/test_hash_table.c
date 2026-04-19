#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t str_hash(const void *key)
{
    const char *s = (const char *) key;
    size_t hash = 0;
    while (*s) hash = hash * 131 + (*s++);
    return hash;
}

int str_equal(const void *a, const void *b)
{
    return strcmp((const char *) a, (const char *) b) == 0;
}

void print_kv(void *k, void *v, void *arg)
{
    printf("%s → %d\n", (char *) k, *(int *) v);
}

int main()
{
    HashTable *ht = create_hash_table(11, str_hash, str_equal, free, free);

    char *k1 = strdup("a");
    int *v1 = malloc(sizeof(int));
    *v1 = 10;
    char *k2 = strdup("b");
    int *v2 = malloc(sizeof(int));
    *v2 = 20;

    hash_table_put(ht, k1, v1);
    hash_table_put(ht, k2, v2);

    int *val = hash_table_get(ht, "a");
    if (val) printf("get a: %d\n", *val);

    hash_table_traverse(ht, print_kv, NULL);

    hash_table_remove(ht, "b");

    printf("======================\n");
    hash_table_traverse(ht, print_kv, NULL);
    destroy_hash_table(&ht);
    return 0;
}