#pragma once
#include "token.h"
#include <stdio.h>

typedef enum {
    FSM_START, // 初始态
    FSM_IDENT, // 标识符和关键字
    FSM_INT,   // 整数
    FSM_FLOAT, // 浮点数
    FSM_STR,   // 字符串
    // 运算符
    FSM_ADD,
    FSM_SUB,
    FSM_MUL,
    FSM_DIV,
    FSM_MOD,
    FSM_EQ,
    FSM_LT,
    FSM_GT,
    FSM_END
} FsmState;

typedef struct {
    const char *input;
    size_t pos;
} Lexer;

void init_lexer(Lexer *lexer, const char *text);

Token *next_token(Lexer *lexer);