#include "myshell.h"

Command *parse_input(char *line) {
    Command *cmd = malloc(sizeof(Command));
    memset(cmd, 0, sizeof(Command)); // Initialize all to NULL/0

    cmd->args = malloc(MAX_TOKENS * sizeof(char *));
    
    // Check for Pipe '|'
    char *pipe_loc = strchr(line, '|');
    char *left_part = line;
    char *right_part = NULL;

    if (pipe_loc) {
        cmd->is_pipe = 1;
        *pipe_loc = '\0'; // Split string
        right_part = pipe_loc + 1;
        cmd->pipe_args = malloc(MAX_TOKENS * sizeof(char *));
    }

    // --- Helper to parse tokens into an array ---
    void tokenize(char *str, char **arr) {
        int i = 0;
        char *token = strtok(str, " \t\n");
        while (token != NULL) {
            arr[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        arr[i] = NULL;
    }

    tokenize(left_part, cmd->args);

    if (cmd->is_pipe && right_part) {
        tokenize(right_part, cmd->pipe_args);
    }

    // --- Parse Redirection & Background (Only on main args) ---
    int i = 0;
    while (cmd->args[i] != NULL) {
        // Requirement 2.6: Background &
        if (strcmp(cmd->args[i], "&") == 0) {
            cmd->background = 1;
            cmd->args[i] = NULL; // Remove & from args
            break;
        }
        // Requirement 2.4: Redirection
        else if (strcmp(cmd->args[i], ">") == 0) {
            cmd->output_file = cmd->args[i+1];
            cmd->append_mode = 0;
            cmd->args[i] = NULL; // Truncate args here
            break;
        }
        else if (strcmp(cmd->args[i], ">>") == 0) {
            cmd->output_file = cmd->args[i+1];
            cmd->append_mode = 1;
            cmd->args[i] = NULL;
            break;
        }
        else if (strcmp(cmd->args[i], "<") == 0) {
            cmd->input_file = cmd->args[i+1];
            cmd->args[i] = NULL;
            // Don't break, might have output redirect later
        }
        i++;
    }

    return cmd;
}

void free_command(Command *cmd) {
    free(cmd->args);
    if (cmd->pipe_args) free(cmd->pipe_args);
    free(cmd);
}