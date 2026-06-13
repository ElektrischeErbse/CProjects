#include "fsm.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *f = fopen("compiler_test.txt", "r");
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    char *text = malloc(file_size + 1);
    fseek(f, 0, SEEK_SET);
    fread(text, file_size, 1, f);
    text[file_size] = '\0';
    // printf("text = %s\n", text);

    Lexer lexer;
    init_lexer(&lexer, text);
    Token *token = next_token(&lexer);
    while (token) {
        if (token->type == TOKEN_EOF) {
            free(token);
            break;
        }
        print_token(token);
        free(token);
        token = next_token(&lexer);
    }

    free(text);
    fclose(f);
    return 0;
}