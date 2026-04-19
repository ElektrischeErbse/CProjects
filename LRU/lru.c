#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <hash_table.h>
#include <linked_list.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct Word {
    char data[32];
} Word;

Word create_word(StringView sv)
{
    Word word = {0};
    assert(sv.size < sizeof(word) - 1);
    memcpy(word.data, sv.data, sv.size);
    return word;
}

Word word_normal(Word *word)
{
    Word result = {0};
    char *input = word->data;
    char *output = result.data;
    while (*input) {
        char c = *input;
        if (isalnum(c)) {
            *output++ = toupper(c);
        }
        ++input;
    }
    return result;
}

// 耗时函数
size_t word_count(StringView content, Word *search_word)
{
    size_t result = 0;
    while (content.size > 0) {
        StringView line = trim_sv(parse_line_sv(&content, '\n'));
        while (line.size > 0) {
            StringView word_sv = trim_sv(parse_line_sv(&line, ' '));
            if (word_sv.size > 0) {
                Word word = create_word(word_sv);
                Word normal_word = word_normal(&word);
                if (strcmp(normal_word.data, search_word->data) == 0) {
                    result++;
                }
            }
        }
    }
    return result;
}

#define LRU_SIZE 512

typedef struct Value {
    size_t count;
    ListNode *node;
} Value;

typedef struct Data {
    Word *word;
    size_t count;
} Data;

#ifdef LRU_ENABLE
size_t LRU_GET(HashTable *hash_table, LinkedList *list, Word *word)
{
    Value *value = hash_table_get(hash_table, word);
    if (value) {
        list_move_node_to_front(list, value->node);
        return value->count;
    }
    return 0;
}

void LRU_PUT(HashTable *hash_table, LinkedList *list, Word *word, size_t count)
{
    // 1. 先创建要存入哈希表的 key（必须堆分配）
    Word *key = malloc(sizeof(Word));
    memcpy(key->data, word->data, sizeof(key->data));

    // 2. 创建链表节点数据 Data
    Data *data = malloc(sizeof(Data));
    // data 中的word不在动态分配
    data->word = key;
    data->count = count;

    // 3. 加入链表头部
    list_push_front(list, data);

    // 4. 创建哈希表 value
    Value *value = malloc(sizeof(Value));
    value->count = count;
    value->node = list_front(list);

    // 5. 插入哈希表
    hash_table_put(hash_table, key, value);

    // 6. 如果超过容量，淘汰尾部
    if (list_size(list) > LRU_SIZE) {
        ListNode *tail = list_back(list);
        Data *tail_data = tail->data;

        hash_table_remove(hash_table, tail_data->word);

        list_pop_back(list);
    }
}
#else
size_t LRU_GET(HashTable *hash_table, LinkedList *list, Word *word)
{
    return 0;
}

void LRU_PUT(HashTable *hash_table, LinkedList *list, Word *word, size_t count)
{
}
#endif

size_t hash_func(const void *key)
{
    const Word *word = (const Word *) key;
    const char *s = word->data;
    size_t hash = 5381;
    while (*s != '\0') {
        hash = ((hash << 5) + hash) + *s;
        s++;
    }
    return hash;
}

int equal_data(const void *lsh, const void *rsh)
{
    Data *l = (Data *) lsh;
    Data *r = (Data *) rsh;
    if (strcmp(l->word->data, r->word->data) == 0) {
        return 0;
    }
    return 1;
}

void free_key(void *key)
{
    free(key);
}

void free_value(void *value)
{
    free(value);
}

void free_data(void *data)
{
    free(data);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("ERROR: The input path does not exist\n");
        printf("Usage: ./lru <input_path>\n");
        exit(1);
    }
    const char *file_path = argv[1];
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        printf("ERROR: Could not open file %s: %s", file_path, strerror(errno));
        exit(1);
    }
    struct stat stat_buf = {0};
    if (fstat(fd, &stat_buf) == -1) {
        printf("ERROR: Could not get file size %s: %s", file_path, strerror(errno));
        exit(1);
    }
    size_t content_size = stat_buf.st_size;
    char *content_data = (char *) mmap(NULL, content_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (content_data == MAP_FAILED) {
        printf("ERROR: Could not mmap file %s: %s", file_path, strerror(errno));
        exit(1);
    }
    HashTable *ht = create_hash_table(LRU_SIZE, hash_func, equal_data, free_key, free_value);
    LinkedList *list = create_linked_list(NULL, free_data, equal_data);
    StringView sv = create_sv(content_data, content_size);
    while (sv.size > 0) {
        StringView line = trim_sv(parse_line_sv(&sv, '\n'));
        while (line.size > 0) {
            StringView word_sv = trim_sv(parse_line_sv(&line, ' '));
            if (word_sv.size > 0) {
                Word word = create_word(word_sv);
                Word search_word = word_normal(&word);
                StringView content = create_sv(content_data, content_size);
                // printf("[%s]:(%zu)\n", search_word.data, word_count(content, &search_word));
                size_t count = LRU_GET(ht, list, &search_word);
                if (count == 0) {
                    size_t c = word_count(content, &search_word);
                    printf("[%s]:(%zu)\n", search_word.data, c);
                    LRU_PUT(ht, list, &search_word, c);
                } else {
                    printf("[%s]:(%zu)\n", search_word.data, count);
                }
            }
        }
    }
    destroy_linked_list(&list);
    destroy_hash_table(&ht);
    munmap(content_data, content_size);
    close(fd);
    return 0;
}
// 不开启LRU
// ./bin/lru LRU/shakespeare.txt  18.88s user 0.17s system 104% cpu 18.150 total

// 开启LRU
// ./bin/lru LRU/shakespeare.txt  6.35s user 0.05s system 86% cpu 7.374 total