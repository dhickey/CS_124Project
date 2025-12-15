#include "myshell.h"

int main() {
    char line[MAX_CMD_LEN];
    
    // Requirement 2.8: Open log file once
    log_fd = open("myshell.log", O_WRONLY | O_CREAT | O_APPEND, 0644);

    // Requirement 2.7: Setup Signals
    setup_signals();

    while (1) {
        // Requirement 2.1: Prompt MUST be exactly "myshell> "
        // (Autograder will fail if you print the directory path)
        printf("myshell> ");
        fflush(stdout);

        if (fgets(line, MAX_CMD_LEN, stdin) == NULL) {
            break; // Exit on Ctrl+D (EOF)
        }

        // Ignore empty lines
        if (line[0] == '\n') continue;

        // Keep a copy of raw line for logging
        char raw_line[MAX_CMD_LEN];
        strncpy(raw_line, line, MAX_CMD_LEN);

        // Remove trailing newline before parsing
        line[strcspn(line, "\n")] = 0;

        // Double check for empty line after strip
        if (line[0] == '\0') continue;

        // Parse
        Command *cmd = parse_input(line);

        // Execute
        execute_command(cmd, raw_line);

        // Requirement 2.9: Free memory
        free_command(cmd);
    }

    if (log_fd != -1) close(log_fd);
    return 0;
}