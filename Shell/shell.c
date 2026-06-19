#include "input_parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int stop = 0;
char *shell_dir = NULL;
int need_free_env = 0;

// cd, pwd, echo, env, setenv, unsetenv, which, exit
int shell_builts(char **args, char ***env, char **shell_dir)
{
    if (!args || !args[0]) {
        return -1;
    }
    if (strcmp(args[0], "cd") == 0) {
        return cd_command(args, *env, shell_dir);
    } else if (strcmp(args[0], "pwd") == 0) {
        return pwd_command(args, *env, shell_dir);
    } else if (strcmp(args[0], "echo") == 0) {
        return echo_command(args, *env, shell_dir);
    } else if (strcmp(args[0], "env") == 0) {
        return env_command(args, *env, shell_dir);
    } else if (strcmp(args[0], "setenv") == 0) {
        *env = setenv_command(args, *env, shell_dir);
        if (*env) {
            need_free_env = 1;
        }
    } else if (strcmp(args[0], "unsetenv") == 0) {
        *env = unsetenv_command(args, *env, shell_dir);
        if (*env) {
            need_free_env = 1;
        }
    } else if (strcmp(args[0], "which") == 0) {
        return which_command(args, *env, shell_dir);
    } else if (strcmp(args[0], "exit") == 0) {
        stop = 1;
    } else {
        // not a built-in commands
        return executor(args, *env, shell_dir);
    }
    return 0;
}

void shell_loop(char **env)
{
    shell_dir = getcwd(NULL, 0);
    assert(shell_dir != NULL);
    while (!stop) {
        printf("[%s]> ", shell_dir);
        char *input = NULL;
        size_t input_size = 0;
        // failure or eof return -1
        if (getline(&input, &input_size, stdin) == -1) {
            break;
        }
        // printf("input: %s", input);
        char **tokens = input_parser(input);
        // debug_tokens(tokens);
        shell_builts(tokens, &env, &shell_dir);
    loop_end:
        free_tokens(tokens);
        free(input);
    }
    free(shell_dir);
    if (need_free_env) {
        free_env(env);
    }
}

int main(int argc, char **argv, char **env)
{
    // Shell loop
    shell_loop(env);
    return 0;
}