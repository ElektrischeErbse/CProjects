#include "file_utils.h"
#include <errno.h>
#include <openssl/evp.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

// 分块大小：4KB（可改成 8192、65536 等）
#define BUFFER_SIZE 4096
#define HASH_BLOCK_SIZE 32
#define HASH_STR_LEN (2 * HASH_BLOCK_SIZE + 1)

// 计算文件的 SHA256
int file_sha256(const char *filename, unsigned char *out_hash, unsigned int *out_len)
{
    int old_errno = errno;
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen failed");
        return -1;
    }
    // 创建哈希上下文
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) {
        fclose(fp);
        fprintf(stderr, "EVP_MD_CTX_new failed\n");
        return -1;
    }
    // 初始化 SHA256
    if (EVP_DigestInit_ex(ctx, EVP_sha256(), NULL) != 1) {
        EVP_MD_CTX_free(ctx);
        fclose(fp);
        return -1;
    }
    // 分块读取文件 + 分块更新哈希
    unsigned char buffer[BUFFER_SIZE];
    size_t n;
    while ((n = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        EVP_DigestUpdate(ctx, buffer, n);
    }
    // 最终计算哈希
    EVP_DigestFinal_ex(ctx, out_hash, out_len);
    // 释放资源
    EVP_MD_CTX_free(ctx);
    fclose(fp);
    errno = old_errno;
    return 0;
}

char hex_digit(unsigned n)
{
    n = n % 16;
    if (n <= 9) {
        return n + '0';
    } else {
        return n - 10 + 'a';
    }
}

void hex_hash(unsigned char *hash, unsigned int len, unsigned char *output)
{
    for (unsigned i = 0; i < len; ++i) {
        unsigned n1 = hash[i] / 16;
        unsigned n2 = hash[i];
        char c1 = hex_digit(n1);
        char c2 = hex_digit(n2);
        output[i * 2 + 0] = c1;
        output[i * 2 + 1] = c2;
    }
}

// 打印十六进制哈希
void print_hash(unsigned char *hash, unsigned int len)
{
    for (unsigned int i = 0; i < len; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

typedef struct Hash {
    unsigned char hash[HASH_BLOCK_SIZE];
} Hash;

typedef struct Record {
    Hash key;
    char **paths;
} Record;

Record *db = NULL;

void handler(const char *path, void *arg)
{
    Hash hash;
    unsigned len = sizeof(hash);
    file_sha256(path, (unsigned char *) &hash, &len);

    // 生成hash表
    ptrdiff_t idx = hmgeti(db, hash);
    if (idx < 0) {
        Record record;
        record.paths = NULL;
        record.key = hash;
        arrput(record.paths, strdup(path));
        hmputs(db, record);
    } else {
        arrput(db[idx].paths, strdup(path));
    }
    // unsigned char hash_str[HASH_STR_LEN] = {0};
    // hex_hash(hash, len, hash_str);
    // printf("name:[%s] => %s\n", path, hash_str);
    // printf("name:[%s]\n", path);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("ERROR: Usage dup_files <path>\n");
        exit(1);
    }
    handle_file(argv[1], handler, NULL);
    // 查找重复文件
    for (ptrdiff_t i = 0; i < hmlen(db); ++i) {
        if (arrlen(db[i].paths) > 1) {
            unsigned char hash_str[HASH_STR_LEN] = {0};
            hex_hash((unsigned char *) &db[i].key, sizeof(Hash), hash_str);
            printf("%s\n", hash_str);
            for (ptrdiff_t j = 0; j < arrlen(db[i].paths); ++j) {
                printf("    %s\n", db[i].paths[j]);
            }
        }
    }
    // 释放内存
    for (ptrdiff_t i = 0; i < hmlen(db); ++i) {
        for (ptrdiff_t j = 0; j < arrlen(db[i].paths); ++j) {
            free(db[i].paths[j]);
        }
        arrfree(db[i].paths);
    }
    hmfree(db);
    return 0;
}