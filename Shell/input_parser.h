#pragma once

#define TOKEN_SIZE 1024
#define BUFFER_SIZE 512

char **input_parser(char *input);

void debug_tokens(char **tokens);

void free_tokens(char **tokens);

void free_env(char **env);

void debug_env(char **env);

// Implement builts-in commands
int cd_command(char **args, char **env, char **shell_dir);
int pwd_command(char **args, char **env, char **shell_dir);
int echo_command(char **args, char **env, char **shell_dir);
int env_command(char **args, char **env, char **shell_dir);
char **setenv_command(char **args, char **env, char **shell_dir);
char **unsetenv_command(char **args, char **env, char **shell_dir);
int which_command(char **args, char **env, char **shell_dir);

// Executor
int executor(char **args, char **env, char **shell_dir);