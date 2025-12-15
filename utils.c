#include "myshell.h"

int log_fd = -1;

// Requirement 2.7: SIGCHLD Handler to reap zombies
void handle_sigchld(int sig) {
    (void)sig; // Silence unused warning
    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {
        // Zombie reaped
    }
}

// Requirement 2.7: Setup Signals
void setup_signals() {
    // Parent ignores SIGINT (Ctrl+C)
    signal(SIGINT, SIG_IGN);

    // Handle SIGCHLD
    struct sigaction sa;
    sa.sa_handler = &handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    if (sigaction(SIGCHLD, &sa, 0) == -1) {
        perror("sigaction");
        exit(1);
    }
}

// Requirement 2.8: Log using write() only
void log_event(pid_t pid, char *cmd, int status) {
    if (log_fd == -1) return;

    char buffer[512];
    // Strip newline from cmd for cleaner log
    char clean_cmd[MAX_CMD_LEN];
    strncpy(clean_cmd, cmd, MAX_CMD_LEN);
    clean_cmd[strcspn(clean_cmd, "\n")] = 0;

    int len = snprintf(buffer, sizeof(buffer), "[pid=%d] cmd=\"%s\" status=%d\n", pid, clean_cmd, status);
    
    // Write system call
    write(log_fd, buffer, len);
}