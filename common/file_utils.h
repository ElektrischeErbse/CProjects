#pragma once

#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>

bool is_dir(const char *path);

char *join_path(const char *base, const char *file);

typedef void (*FileHandler)(const char *, void *);

bool traversal_dir(const char *path, FileHandler handler, void *arg);

bool recursive_dir(const char *path, FileHandler handler, void *arg);

bool handle_file(const char *path, FileHandler handler, void *arg);