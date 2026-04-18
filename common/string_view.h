#pragma once

#include <stddef.h>

struct string_view {
    const char *data;
    size_t size;
};

struct string_view create_sv(const char *data, size_t size);

struct string_view parse_line(struct string_view *sv, char delimiter);

struct string_view trim_sv(struct string_view sv);

void print_sv(struct string_view sv);