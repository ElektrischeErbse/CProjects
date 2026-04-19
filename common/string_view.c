#include "string_view.h"
#include <ctype.h>
#include <stdio.h>

StringView create_sv(const char *data, size_t size)
{
    StringView sv;
    sv.data = data;
    sv.size = size;
    return sv;
}

StringView parse_line_sv(StringView *sv, char delimiter)
{
    StringView result;
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

StringView trim_sv(StringView sv)
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

void print_sv(StringView sv)
{
    printf("[%.*s]\n", (int) sv.size, sv.data);
}