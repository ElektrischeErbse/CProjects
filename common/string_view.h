#pragma once

#include <stddef.h>

typedef struct StringView {
    const char *data;
    size_t size;
} StringView;

StringView create_sv(const char *data, size_t size);

StringView parse_line_sv(StringView *sv, char delimiter);

StringView trim_sv(StringView sv);

void print_sv(StringView sv);