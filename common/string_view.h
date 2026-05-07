#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct StringView {
    const char *data;
    size_t size;
} StringView;

StringView create_sv(const char *data, size_t size);

StringView split_sv(StringView *sv, char delimiter);

StringView trim_sv(StringView sv);

bool starts_with_sv(StringView sv, const char *prefix);

const char *c_str_sv(StringView sv);

StringView remove_prefix_sv(StringView *sv, size_t n);

void print_sv(StringView sv);

#define SV_FMT "%.*s"
#define SV_ARGS(sv) (int) (sv).size, (sv).data