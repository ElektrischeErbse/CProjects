#include "input_parser.h"
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char *emit_token(const char *s, size_t len)
{
    if (len == 0) {
        return NULL;
    }
    char *token = malloc(sizeof(char) * len + 1);
    if (!token) {
        return NULL;
    }
    strncpy(token, s, len);
    token[len] = '\0';
    return token;
}

char **input_parser(char *input)
{
    if (!input) {
        return NULL;
    }

    char **tokens = malloc(sizeof(char *) * TOKEN_SIZE + 1);
    if (!tokens) {
        return NULL;
    }

    size_t input_len = strlen(input);
    size_t token_len = 0;
    char *p = input;
    size_t token_pos = 0;
    for (size_t i = 0; i < input_len; ++i) {
        char c = input[i];
        if (isspace(c)) {
            // generate a token
            char *token = emit_token(p, token_len);
            if (token) {
                tokens[token_pos++] = token;
            }
            p = p + token_len + 1;
            token_len = 0;
        } else {
            ++token_len;
        }
    }
    char *token = emit_token(p, token_len);
    if (token) {
        tokens[token_pos++] = token;
    }
    assert(token_pos <= TOKEN_SIZE);
    // Terminate tokens with NULL
    tokens[token_pos] = NULL;
    return tokens;
}

void debug_tokens(char **tokens)
{
    if (!tokens) {
        return;
    }
    size_t i = 0;
    while (tokens[i]) {
        printf("token: %s\n", tokens[i++]);
    }
}

void free_tokens(char **tokens)
{
    if (!tokens) {
        return;
    }
    size_t i = 0;
    while (tokens[i]) {
        free(tokens[i++]);
    }
    free(tokens);
}

void free_env(char **env)
{
    if (!env) {
        return;
    }
    for (int i = 0; env[i]; ++i) {
        free(env[i]);
    }
    free(env);
}

void debug_env(char **env)
{
    if (!env) {
        return;
    }
    size_t i = 0;
    while (env[i]) {
        printf("%s\n", env[i++]);
    }
}

char *my_getenv(char **env, const char *name)
{
    if (!env || !name) {
        return NULL;
    }
    size_t name_len = strlen(name);
    int found = -1;
    for (int i = 0; env[i]; ++i) {
        if (strncmp(name, env[i], name_len) == 0 && env[i][name_len] == '=') {
            found = i;
            break;
        }
    }
    if (found != -1) {
        return env[found];
    }
    return NULL;
}

// Implement builts-in commands
int cd_command(char **args, char **env, char **shell_dir)
{
    if (args[1] == NULL || args[2] != NULL) {
        fprintf(stderr, "Error: usage cd <directory>\n");
        return -1;
    }
    if (chdir(args[1]) == -1) {
        perror("cd");
        return -1;
    }
    char *pwd = getcwd(NULL, 0);
    assert(pwd != NULL);
    free(*shell_dir);
    *shell_dir = pwd;
    return 0;
}

int pwd_command(char **args, char **env, char **shell_dir)
{
    char *pwd = getcwd(NULL, 0);
    if (pwd == NULL) {
        perror("pwd");
        return -1;
    }
    printf("%s\n", pwd);
    free(pwd);
    return 0;
}

int echo_command(char **args, char **env, char **shell_dir)
{
    int new_line = 1;
    size_t i = 1;
    while (args[i]) {
        // Handle environment variables
        if (args[i][0] == '$') {
            // Skip '$' character
            char *content = my_getenv(env, args[i] + 1);
            if (content) {
                printf("%s", content);
            }
        } else {
            printf("%s", args[i]);
        }

        if (args[i + 1]) {
            printf(" ");
        }
        ++i;
    }
    if (new_line) {
        printf("\n");
    }
    return 0;
}

int env_command(char **args, char **env, char **shell_dir)
{
    size_t i = 0;
    while (env[i]) {
        printf("%s\n", env[i++]);
    }
    return 0;
}

int set_env_first = 1;
char **setenv_command(char **args, char **env, char **shell_dir)
{
    if (args[1] == NULL || args[2] == NULL || args[3] != NULL) {
        printf("Error: usage setenv <variable> <value>\n");
        return NULL;
    }
    int env_cnt = 0;
    while (env[env_cnt]) {
        ++env_cnt;
    }
    size_t new_env_len = env_cnt + 2;
    char **new_env = malloc(sizeof(char *) * new_env_len);
    if (!new_env) {
        perror("setenv");
        return NULL;
    }
    // copy existing environment variables
    for (int i = 0; env[i]; ++i) {
        new_env[i] = strdup(env[i]);
        if (!new_env[i]) {
            free_env(new_env);
            return NULL;
        }
    }
    new_env[env_cnt] = NULL;

    // create env var
    // format: "name=value"
    size_t new_var_len = strlen(args[1]) + strlen(args[2]) + 2;
    char *new_var = malloc(sizeof(char) * new_var_len);
    if (!new_var) {
        perror("setenv");
        free_env(new_env);
        return NULL;
    }
    snprintf(new_var, new_var_len, "%s=%s", args[1], args[2]);
    // printf("new_var: %s\n", new_var);
    new_env[new_env_len - 2] = new_var;
    new_env[new_env_len - 1] = NULL;
    // debug_env(new_env);
    // set env var first, no need to free env
    if (set_env_first) {
        set_env_first = 0;
        return new_env;
    }
    // set env var not first, need to free env
    free_env(env);
    return new_env;
}

int unset_env_first = 1;
char **unsetenv_command(char **args, char **env, char **shell_dir)
{
    if (args[1] == NULL || args[2] != NULL) {
        printf("Error: usage unsetenv <variable>\n");
        return NULL;
    }
    int env_cnt = 0;
    while (env[env_cnt]) {
        ++env_cnt;
    }
    size_t new_env_len = env_cnt + 1;
    char **new_env = malloc(sizeof(char *) * new_env_len);
    if (!new_env) {
        perror("setenv");
        return NULL;
    }
    // copy existing environment variables
    for (int i = 0; env[i]; ++i) {
        new_env[i] = strdup(env[i]);
        if (!new_env[i]) {
            free_env(new_env);
            return NULL;
        }
    }
    new_env[env_cnt] = NULL;

    // search for the environment variables with specified name
    size_t name_len = strlen(args[1]);
    int found = -1;
    for (int i = 0; new_env[i]; ++i) {
        if (strncmp(args[1], new_env[i], name_len) == 0 && new_env[i][name_len] == '=') {
            found = i;
        }
    }
    if (found != -1) {
        free(new_env[found]);
        new_env[found] = new_env[new_env_len - 2];
        new_env[new_env_len - 2] = NULL;
    }

    if (unset_env_first) {
        unset_env_first = 0;
        return new_env;
    }
    free_env(env);
    return new_env;
}

char *find_cmd_in_path(const char *cmd, const char *path)
{
    if (!cmd || !path) {
        return NULL;
    }
    size_t cmd_len = strlen(cmd);
    size_t path_len = strlen(path);
    char *full_path = calloc(cmd_len + path_len + 2, sizeof(char));
    if (!full_path) {
        return NULL;
    }
    strcat(full_path, path);
    strcat(full_path, "/");
    strcat(full_path, cmd);
    if (access(full_path, R_OK | X_OK) == -1) {
        // perror("access");
        return NULL;
    }
    return full_path;
}

const char *built_in_cmds[] = {"cd", "pwd", "echo", "env", "setenv", "unsetenv", "which", "exit", NULL};
int which_command(char **args, char **env, char **shell_dir)
{
    if (args[1] == NULL || args[2] != NULL) {
        fprintf(stderr, "Error: usage which <command>\n");
        return -1;
    }
    // built-in commands
    for (size_t i = 0; built_in_cmds[i]; ++i) {
        if (strcmp(args[1], built_in_cmds[i]) == 0) {
            printf("[%s] is a built-in command\n", args[1]);
            return 0;
        }
    }
    // get PATH env
    char *tmp = my_getenv(env, "PATH");
    if (!tmp) {
        printf("which: get PATH env failed!\n");
        return -1;
    }
    char *path = strdup(tmp);
    // split path with ':'
    char *p = strtok(path, ":");
    char *full_path = NULL;
    while (p) {
        // printf("%s\n", p);
        full_path = find_cmd_in_path(args[1], p);
        if (full_path) {
            printf("%s\n", full_path);
            break;
        }
        p = strtok(NULL, ":");
    }
    if (!full_path) {
        perror("which");
        free(path);
        return -1;
    }
    free(path);
    return 0;
}

int executor(char **args, char **env, char **shell_dir)
{
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // command start with "./"
        // for example: ./a.out
        if (strncmp(args[0], "./", 2) == 0) {
            // get current working directory
            char *cwd = getcwd(NULL, 0);
            if (cwd) {
                char full_path[BUFFER_SIZE] = {0};
                snprintf(full_path, sizeof(full_path), "%s/%s", cwd, args[0]);
                free(cwd);
                if (execve(full_path, args, env) == -1) {
                    perror("execve");
                    exit(EXIT_FAILURE);
                }
            }
            exit(EXIT_SUCCESS);
        }
        // get PATH env
        char *tmp = my_getenv(env, "PATH");
        if (!tmp) {
            printf("%s: get PATH env failed!\n", args[0]);
            exit(EXIT_FAILURE);
        }
        char *path = strdup(tmp);
        // printf("path: %s\n", path);
        // split path with ':'
        char *p = strtok(path, ":");
        char *full_path = NULL;
        while (p) {
            full_path = find_cmd_in_path(args[0], p);
            if (full_path) {
                break;
            }
            p = strtok(NULL, ":");
        }
        if (!full_path) {
            printf("%s: %s\n", args[0], strerror(errno));
            free(path);
            exit(EXIT_FAILURE);
        }
        free(path);
        // printf("full_path: %s\n", full_path);
        if (execve(full_path, args, env) == -1) {
            perror("execve");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    } else {
        // printf("wait child process exit\n");
        if (waitpid(-1, NULL, 0) == -1) {
            perror("waitpid");
            return -1;
        }
        // printf("child process has exited\n");
        return 0;
    }
}