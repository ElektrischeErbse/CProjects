#define _XOPEN_SOURCE 500
#include <assert.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_MAX_SIZE 512
#define FILE_ENTRY_SIZE 10

typedef struct {
    char path[PATH_MAX_SIZE];
    int size;
} FileEntry;

FileEntry *files = NULL;
size_t cur_idx = 0;
size_t total_size = 0;

int fn(const char *path, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if (cur_idx >= total_size) {
        total_size *= 2;
        files = realloc(files, total_size * sizeof(FileEntry));
        assert(files != NULL);
    }
    strcpy(files[cur_idx].path, path);
    files[cur_idx].size = sb->st_size;
    ++cur_idx;
    return 0;
}

void init_files()
{
    files = calloc(FILE_ENTRY_SIZE, sizeof(FileEntry));
    if (!files) {
        printf("ERROR: Could not allocate memory\n");
        exit(1);
    }
    total_size = FILE_ENTRY_SIZE;
}

void print_files()
{
    for (size_t i = 0; i < cur_idx; ++i) {
        printf("%d\t%s\n", files[i].size, files[i].path);
    }
}

int compare(const void *lsh, const void *rsh)
{
    FileEntry *l = (FileEntry *) lsh;
    FileEntry *r = (FileEntry *) rsh;
    if (l->size == r->size) {
        return 0;
    }
    if (l->size > r->size) {
        return -1;
    } else {
        return 1;
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("Usage: disk_usage <path>\n");
        exit(1);
    }
    printf("Your disk usage:\n");
    init_files();
    if (nftw(argv[1], fn, 5, 0) == -1) {
        perror("nftw failed");
        exit(1);
    }
    qsort(files, cur_idx, sizeof(FileEntry), compare);
    print_files();
    free(files);
    return 0;
}