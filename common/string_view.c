#include "string_view.h"
#include <ctype.h>
#include <stdio.h>

struct string_view create_sv(const char *data, size_t size)
{
    struct string_view sv;
    sv.data = data;
    sv.size = size;
    return sv;
}

struct string_view parse_line(struct string_view *sv, char delimiter)
{
    struct string_view result;
    result.data = sv->data;
    for (size_t i = 0; i < sv->size; ++i) {
        if (sv->data[i] == delimiter) {
            result.size = i;
            sv->data += (i + 1);
            sv->size -= (i + 1);
            return result;
        }
    }
    result.size = sv->size;
    sv->size = 0;
    return result;
}

struct string_view trim_sv(struct string_view sv)
{
    const char *left = sv.data;
    const char *right = sv.data + sv.size;
    while (left != right) {
        char c = *left;
        if (isspace(c)) {
            ++left;
        } else {
            break;
        }
    }
    while (right != left) {
        char c = *(right - 1);
        if (isspace(c)) {
            --right;
        } else {
            break;
        }
    }
    sv.data = left;
    sv.size = right - left;
    return sv;
}

void print_sv(struct string_view sv)
{
    printf("[%.*s]\n", (int) sv.size, sv.data);
}