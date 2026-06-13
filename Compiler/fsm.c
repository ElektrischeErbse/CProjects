#include "fsm.h"
#include "token.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_lexer(Lexer *lexer, const char *text)
{
    lexer->input = text;
    lexer->pos = 0;
}

Token *next_token(Lexer *lexer)
{
    Token *token = calloc(1, sizeof(Token));
    if (!token) {
        return NULL;
    }
    token->type = TOKEN_EOF;
    size_t token_len = 0;
    FsmState state = FSM_START;

    while (state != FSM_END) {
        char c = lexer->input[lexer->pos];
        switch (state) {
            case FSM_START:
                if (isspace(c)) {
                    lexer->pos++;
                } else if (isdigit(c)) {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_INT;
                } else if (c == '.') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_FLOAT;
                } else if (isalpha(c) || c == '_') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_IDENT;
                } else if (c == '"') {
                    lexer->pos++;
                    state = FSM_STR;
                } else if (c == '+') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_ADD;
                } else if (c == '-') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_SUB;
                } else if (c == '*') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_MUL;
                } else if (c == '/') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_DIV;
                } else if (c == '%') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_MOD;
                } else if (c == '=') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_EQ;
                } else if (c == '<') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_LT;
                } else if (c == '>') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_GT;
                } else if (c == '(') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_LPAREN;
                    state = FSM_END;
                } else if (c == ')') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_RPAREN;
                    state = FSM_END;
                } else if (c == '[') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_LBRACK;
                    state = FSM_END;
                } else if (c == ']') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_RBRACK;
                    state = FSM_END;
                } else if (c == '{') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_LBRACE;
                    state = FSM_END;
                } else if (c == '}') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_RBRACE;
                    state = FSM_END;
                } else if (c == ';') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_SEMICOLON;
                    state = FSM_END;
                } else if (c == ',') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_COMMA;
                    state = FSM_END;
                } else if (c == '\0') {
                    state = FSM_END;
                } else {
                    assert(0 && "unknown char type!");
                }
                break;
            case FSM_INT:
                if (isdigit(c)) {
                    token->text[token_len++] = c;
                    lexer->pos++;
                } else if (c == '.') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                    state = FSM_FLOAT;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_INT_NUM;
                    state = FSM_END;
                }
                break;
            case FSM_FLOAT:
                if (isdigit(c)) {
                    token->text[token_len++] = c;
                    lexer->pos++;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_FLOAT_NUM;
                    state = FSM_END;
                }
                break;
            case FSM_ADD:
                if (c == '=') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_ADD_ASSIGN;
                    state = FSM_END;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_ADD;
                    state = FSM_END;
                }
                break;
            case FSM_SUB:
                if (c == '=') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_SUB_ASSIGN;
                    state = FSM_END;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_SUB;
                    state = FSM_END;
                }
                break;
            case FSM_MUL:
                if (c == '=') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_MUL_ASSIGN;
                    state = FSM_END;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_MUL;
                    state = FSM_END;
                }
                break;
            case FSM_DIV:
                if (c == '=') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_DIV_ASSIGN;
                    state = FSM_END;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_DIV;
                    state = FSM_END;
                }
                break;
            case FSM_MOD:
                if (c == '=') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_MOD_ASSIGN;
                    state = FSM_END;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_MOD;
                    state = FSM_END;
                }
                break;
            case FSM_EQ:
                if (c == '=') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_EQ;
                    state = FSM_END;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_ASSIGN;
                    state = FSM_END;
                }
                break;
            case FSM_LT:
                if (c == '=') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_LE;
                    state = FSM_END;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_LT;
                    state = FSM_END;
                }
                break;
            case FSM_GT:
                if (c == '=') {
                    token->text[token_len++] = c;
                    token->text[token_len] = '\0';
                    lexer->pos++;
                    token->type = TOKEN_GE;
                    state = FSM_END;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_GT;
                    state = FSM_END;
                }
                break;
            case FSM_IDENT:
                if (isalnum(c) || c == '_') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_IDENT;
                    state = FSM_END;
                }
                break;
            case FSM_STR:
                if (c != '"') {
                    token->text[token_len++] = c;
                    lexer->pos++;
                } else {
                    token->text[token_len] = '\0';
                    token->type = TOKEN_STRING_VAL;
                    state = FSM_END;
                }
                break;
            default:
                assert(0 && "FSM state error!");
                break;
        }
    }
    return token;
}
