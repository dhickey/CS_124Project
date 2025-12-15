#include "myshell.h"

void execute_command(Command *cmd, char *raw_line) {
    if (cmd->args[0] == NULL) return;

    // --- Requirement 2.3: Built-in Commands ---
    if (strcmp(cmd->args[0], "exit") == 0 || strcmp(cmd->args[0], "quit") == 0) {
        free_command(cmd);
        if (log_fd != -1) close(log_fd);
        exit(0);
    }

    if (strcmp(cmd->args[0], "cd") == 0) {
        char *path = cmd->args[1];
        if (!path) path = getenv("HOME");
        if (chdir(path) != 0) {
            perror("cd failed");
        }
        return; // Built-ins run in parent
    }

    // --- Requirement 2.5: Pipes ---
    if (cmd->is_pipe) {
        int pfd[2];
        if (pipe(pfd) == -1) { perror("pipe"); return; }

        pid_t pid1 = fork();
        if (pid1 == 0) {
            // Left Child
            signal(SIGINT, SIG_DFL); // Restore Ctrl+C
            dup2(pfd[1], STDOUT_FILENO);
            close(pfd[0]); close(pfd[1]);
            execvp(cmd->args[0], cmd->args);
            perror("execvp left"); exit(1);
        }

        pid_t pid2 = fork();
        if (pid2 == 0) {
            // Right Child
            signal(SIGINT, SIG_DFL); // Restore Ctrl+C
            dup2(pfd[0], STDIN_FILENO);
            close(pfd[0]); close(pfd[1]);
            execvp(cmd->pipe_args[0], cmd->pipe_args);
            perror("execvp right"); exit(1);
        }

        // Parent
        close(pfd[0]); close(pfd[1]);
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        return;
    }

    // --- Requirement 2.2: External Commands ---
    pid_t pid = fork();

    if (pid == 0) {
        // --- Child Process ---
        signal(SIGINT, SIG_DFL); // Requirement 2.7: Restore Ctrl+C

        // Requirement 2.4: I/O Redirection
        if (cmd->input_file) {
            int fd = open(cmd->input_file, O_RDONLY);
            if (fd == -1) { perror("open input"); exit(1); }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (cmd->output_file) {
            int flags = O_WRONLY | O_CREAT | (cmd->append_mode ? O_APPEND : O_TRUNC);
            int fd = open(cmd->output_file, flags, 0644);
            if (fd == -1) { perror("open output"); exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(cmd->args[0], cmd->args);
        perror("execvp failed");
        exit(1);
    } else if (pid > 0) {
        // --- Parent Process ---
        if (cmd->background) {
            // Requirement 2.6: Background Jobs
            printf("[bg] started pid %d\n", pid);
            log_event(pid, raw_line, 0); // Log start
        } else {
            // Foreground: Wait
            int status;
            waitpid(pid, &status, 0);
            
            // Requirement 2.8: Log status
            if (WIFEXITED(status)) {
                log_event(pid, raw_line, WEXITSTATUS(status));
            }
        }
    } else {
        perror("fork failed");
    }
}