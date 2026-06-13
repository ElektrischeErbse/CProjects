#pragma once

#define MAX_TEXT_LEN 64

typedef enum {
    // 1. 关键字
    TOKEN_INT,    // int
    TOKEN_DOUBLE, // double
    TOKEN_CHAR,   // char
    TOKEN_VOID,   // void
    TOKEN_IF,     // if
    TOKEN_ELSE,   // else
    TOKEN_FOR,    // for
    TOKEN_WHILE,  // while
    TOKEN_RETURN, // return
    TOKEN_CONST,  // const
    TOKEN_STATIC, // static

    // 2. 标识符
    TOKEN_IDENT,

    // 3. 字面量
    TOKEN_INT_NUM,
    TOKEN_FLOAT_NUM,
    TOKEN_CHAR_VAL,
    TOKEN_STRING_VAL,

    // 4. 单目/双目运算符
    TOKEN_ADD, // +
    TOKEN_SUB, // -
    TOKEN_MUL, // *
    TOKEN_DIV, // /
    TOKEN_MOD, // %

    TOKEN_EQ, // ==
    TOKEN_NE, // !=
    TOKEN_LT, // <
    TOKEN_GT, // >
    TOKEN_LE, // <=
    TOKEN_GE, // >=

    TOKEN_AND, // &&
    TOKEN_OR,  // ||
    TOKEN_NOT, // !

    // 赋值
    TOKEN_ASSIGN,     // =
    TOKEN_ADD_ASSIGN, // +=
    TOKEN_SUB_ASSIGN, // -=
    TOKEN_MUL_ASSIGN, // *=
    TOKEN_DIV_ASSIGN, // /=
    TOKEN_MOD_ASSIGN, // %=

    // 5. 分隔符
    TOKEN_LPAREN,    // (
    TOKEN_RPAREN,    // )
    TOKEN_LBRACK,    // [
    TOKEN_RBRACK,    // ]
    TOKEN_LBRACE,    // {
    TOKEN_RBRACE,    // }
    TOKEN_SEMICOLON, // ;
    TOKEN_COMMA,     // ,
    TOKEN_DOT,       // .

    // 结束
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char text[MAX_TEXT_LEN];
} Token;

const char *token_type_to_str(TokenType type);

void print_token(Token *token);