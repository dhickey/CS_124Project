#ifndef MYSHELL_H
#define MYSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_CMD_LEN 1024
#define MAX_TOKENS 64

// Structure to hold parsed command info
typedef struct {
    char **args;       // Arguments array
    char *input_file;  // < file
    char *output_file; // > or >> file
    int append_mode;   // 1 for >>, 0 for >
    int background;    // 1 if & is present
    int is_pipe;       // 1 if pipe logic needed
    char **pipe_args;  // Arguments for the right side of pipe
} Command;

// Global log file descriptor
extern int log_fd;

// Function Prototypes
void setup_signals();
void log_event(pid_t pid, char *cmd, int status);
Command *parse_input(char *line);
void free_command(Command *cmd);
void execute_command(Command *cmd, char *raw_line);

#endif