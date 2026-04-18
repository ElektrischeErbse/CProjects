#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <linked_list.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

struct Word {
    char data[32];
};

struct Word create_word(struct string_view sv)
{
    struct Word word = {0};
    assert(sv.size < sizeof(word) - 1);
    memcpy(word.data, sv.data, sv.size);
    return word;
}

struct Word word_normal(struct Word *word)
{
    struct Word result = {0};
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
size_t word_count(struct string_view content, struct Word *search_word)
{
    size_t result = 0;
    while (content.size > 0) {
        struct string_view line = trim_sv(parse_line(&content, '\n'));
        while (line.size > 0) {
            struct string_view word_sv = trim_sv(parse_line(&line, ' '));
            if (word_sv.size > 0) {
                struct Word word = create_word(word_sv);
                struct Word normal_word = word_normal(&word);
                if (strcmp(normal_word.data, search_word->data) == 0) {
                    result++;
                }
            }
        }
    }
    return result;
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
    struct string_view sv = create_sv(content_data, content_size);
    while (sv.size > 0) {
        struct string_view line = trim_sv(parse_line(&sv, '\n'));
        while (line.size > 0) {
            struct string_view word_sv = trim_sv(parse_line(&line, ' '));
            if (word_sv.size > 0) {
                struct Word word = create_word(word_sv);
                struct Word search_word = word_normal(&word);
                struct string_view content = create_sv(content_data, content_size);
                printf("[%s]:(%zu)\n", search_word.data, word_count(content, &search_word));
            }
        }
    }
    munmap(content_data, content_size);
    close(fd);
    return 0;
}