#include <stdarg.h>
#include <stdio.h>

static void print_error(const char *fmt, ...)
{
    char buffer[1024] = {0};
    va_list va;
    va_start(va, fmt);
    int ret = vsnprintf(buffer, sizeof(buffer), fmt, va);
    va_end(va);
    perror(buffer);
}

#define SYSCALL_ERROR(fmt, ...) print_error(fmt, __VA_ARGS__)