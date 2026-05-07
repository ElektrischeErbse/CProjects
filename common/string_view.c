#include "string_view.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StringView create_sv(const char *data, size_t size)
{
    StringView sv;
    sv.data = data;
    sv.size = size;
    return sv;
}

StringView split_sv(StringView *sv, char delimiter)
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

bool starts_with_sv(StringView sv, const char *prefix)
{
    if (sv.size == 0 || prefix == NULL) {
        return false;
    }
    size_t prefix_size = strlen(prefix);
    if (sv.size < prefix_size) {
        return false;
    }
    for (size_t i = 0; i < prefix_size; ++i) {
        if (sv.data[i] != prefix[i]) {
            return false;
        }
    }
    return true;
}

const char *c_str_sv(StringView sv)
{
    if (sv.size == 0) {
        return NULL;
    }
    char *buf = malloc(sizeof(char) * sv.size + 1);
    if (buf == NULL) {
        return NULL;
    }
    memcpy(buf, sv.data, sv.size);
    buf[sv.size] = '\0';
    return buf;
}

StringView remove_prefix_sv(StringView *sv, size_t n)
{
    StringView result;
    result.data = sv->data;
    result.size = 0;
    if (n >= sv->size) {
        sv->data += sv->size;
        result.size = sv->size;
        sv->size = 0;
        return result;
    }
    sv->data += n;
    sv->size -= n;
    result.size = n;
    return result;
}

void print_sv(StringView sv)
{
    printf(SV_FMT "\n", SV_ARGS(sv));
}