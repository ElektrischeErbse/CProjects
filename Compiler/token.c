#include "token.h"
#include <assert.h>
#include <stdio.h>

const char *token_type_to_str(TokenType type)
{
    switch (type) {
        case TOKEN_INT:
            return "TOKEN_INT";
        case TOKEN_DOUBLE:
            return "TOKEN_DOUBLE";
        case TOKEN_CHAR:
            return "TOKEN_CHAR";
        case TOKEN_VOID:
            return "TOKEN_VOID";
        case TOKEN_IF:
            return "TOKEN_IF";
        case TOKEN_ELSE:
            return "TOKEN_ELSE";
        case TOKEN_FOR:
            return "TOKEN_FOR";
        case TOKEN_WHILE:
            return "TOKEN_WHILE";
        case TOKEN_RETURN:
            return "TOKEN_RETURN";
        case TOKEN_CONST:
            return "TOKEN_CONST";
        case TOKEN_STATIC:
            return "TOKEN_STATIC";
        case TOKEN_IDENT:
            return "TOKEN_IDENT";
        case TOKEN_INT_NUM:
            return "TOKEN_INT_NUM";
        case TOKEN_FLOAT_NUM:
            return "TOKEN_FLOAT_NUM";
        case TOKEN_CHAR_VAL:
            return "TOKEN_CHAR_VAL";
        case TOKEN_STRING_VAL:
            return "TOKEN_STRING_VAL";
        case TOKEN_ADD:
            return "TOKEN_ADD";
        case TOKEN_SUB:
            return "TOKEN_SUB";
        case TOKEN_MUL:
            return "TOKEN_MUL";
        case TOKEN_DIV:
            return "TOKEN_DIV";
        case TOKEN_MOD:
            return "TOKEN_MOD";
        case TOKEN_EQ:
            return "TOKEN_EQ";
        case TOKEN_NE:
            return "TOKEN_NE";
        case TOKEN_LT:
            return "TOKEN_LT";
        case TOKEN_GT:
            return "TOKEN_GT";
        case TOKEN_LE:
            return "TOKEN_LE";
        case TOKEN_GE:
            return "TOKEN_GE";
        case TOKEN_AND:
            return "TOKEN_AND";
        case TOKEN_OR:
            return "TOKEN_OR";
        case TOKEN_NOT:
            return "TOKEN_NOT";
        case TOKEN_ASSIGN:
            return "TOKEN_ASSIGN";
        case TOKEN_ADD_ASSIGN:
            return "TOKEN_ADD_ASSIGN";
        case TOKEN_SUB_ASSIGN:
            return "TOKEN_SUB_ASSIGN";
        case TOKEN_MUL_ASSIGN:
            return "TOKEN_MUL_ASSIGN";
        case TOKEN_DIV_ASSIGN:
            return "TOKEN_DIV_ASSIGN";
        case TOKEN_MOD_ASSIGN:
            return "TOKEN_MOD_ASSIGN";
        case TOKEN_LPAREN:
            return "TOKEN_LPAREN";
        case TOKEN_RPAREN:
            return "TOKEN_RPAREN";
        case TOKEN_LBRACK:
            return "TOKEN_LBRACK";
        case TOKEN_RBRACK:
            return "TOKEN_RBRACK";
        case TOKEN_LBRACE:
            return "TOKEN_LBRACE";
        case TOKEN_RBRACE:
            return "TOKEN_RBRACE";
        case TOKEN_SEMICOLON:
            return "TOKEN_SEMICOLON";
        case TOKEN_COMMA:
            return "TOKEN_COMMA";
        case TOKEN_DOT:
            return "TOKEN_DOT";
        case TOKEN_EOF:
            return "TOKEN_EOF";
        default:
            assert(0 && "unknown token type!");
    }
}

void print_token(Token *token)
{
    printf("%-20s%s\n", token_type_to_str(token->type), token->text);
}