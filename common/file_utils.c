#include "file_utils.h"
#include "common.h"
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define PATH_SEP "/"
#define SEP_LEN (sizeof(PATH_SEP) - 1)

bool is_dir(const char *path)
{
    struct stat statbuf = {0};
    if (lstat(path, &statbuf) == -1) {
        SYSCALL_ERROR("lstat error [path: %s]", path);
        return false;
    }

    if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
        return true;
    }
    return false;
}

char *join_path(const char *base, const char *file)
{
    if (!base || !file) {
        return NULL;
    }
    size_t base_len = strlen(base);
    size_t file_len = strlen(file);
    char *begin = malloc(base_len + file_len + SEP_LEN + 1);
    assert(begin != NULL);
    char *end = begin;
    memcpy(end, base, base_len);
    end += base_len;
    memcpy(end, PATH_SEP, SEP_LEN);
    end += SEP_LEN;
    memcpy(end, file, file_len);
    end += file_len;
    *end = '\0';
    return begin;
}

bool traversal_dir(const char *path, FileHandler handler, void *arg)
{
    DIR *dir = opendir(path);
    if (dir == NULL) {
        SYSCALL_ERROR("opendir error [path: %s]", path);
        return false;
    }

    errno = 0;
    struct dirent *entry = readdir(dir);
    char *child_path = NULL;
    while (entry) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            child_path = join_path(path, entry->d_name);
            handler(child_path, arg);
            free(child_path);
        }
        entry = readdir(dir);
    }
    if (errno != 0) {
        SYSCALL_ERROR("readdir error [path: %s]", path);
        closedir(dir);
        return false;
    }
    closedir(dir);
    return true;
}

bool recursive_dir(const char *path, FileHandler handler, void *arg)
{
    DIR *dir = opendir(path);
    if (dir == NULL) {
        SYSCALL_ERROR("opendir error [path: %s]", path);
        return false;
    }

    errno = 0;
    struct dirent *entry = readdir(dir);
    while (entry) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char *child_path = join_path(path, entry->d_name);
            if (is_dir(child_path)) {
                recursive_dir(child_path, handler, arg);
            } else {
                handler(child_path, arg);
            }
            free(child_path);
        }
        entry = readdir(dir);
    }
    if (errno != 0) {
        SYSCALL_ERROR("readdir error [path: %s]", path);
        closedir(dir);
        return false;
    }
    closedir(dir);
    return true;
}

bool handle_file(const char *path, FileHandler handler, void *arg)
{
    if(is_dir(path)){
        return recursive_dir(path, handler, arg);
    }
    handler(path, arg);
    return true;
}