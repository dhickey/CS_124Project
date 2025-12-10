# Systems_in_C
Build a mini unix shell in C that supports various features
REQUIERMENTS:
1. The Input & Parsing Lifecycle
Your shell needs to handle the raw text input before it even thinks about executing commands.

The "Enter" Key: Users often just hit enter. The shell must ignore empty lines and reprint the prompt.

Tokenization: Splitting a string like ls -l -a into {"ls", "-l", "-a", NULL}.

Cleanup: The ability to free() every single byte allocated for the current command line before prompting for the next one.

"Exit" or "Quit": Recognizing these specific keywords to break the main loop cleanly.

2. Execution Modes (The "What")
Your shell has to distinguish between three distinct types of execution:

Built-ins (cd, exit): These must run in the parent process. If you fork cd, the child changes directory, exits, and the parent stays exactly where it was.

Edge case: cd with no arguments (must go to HOME).

Edge case: cd to a non-existent folder (must print error, not crash).

Foreground External (ls, grep): The standard mode. Parent forks, child executes, parent blocks (waits) until the child is done.

Background External (&): The async mode. Parent forks, child executes, parent does not wait and immediately prints the prompt again.

Requirement: You must print the PID of the background job.

3. I/O Manipulation (The "How")
This is where the shell modifies where the data goes before the command runs.

Overwrite (>): Open file with O_TRUNC. If file doesn't exist, create it.

Append (>>): Open file with O_APPEND. If file doesn't exist, create it.

Input (<): Open file with O_RDONLY. If file doesn't exist, the command should fail gracefully.

The Pipe (|): The most complex handling.

Connecting stdout of Command A to stdin of Command B.

Critical: Closing the pipe file descriptors in the parent process (otherwise processes hang forever waiting for EOF).

4. Signal Handling (The "Interruption")
Your shell needs to be resilient to users trying to stop things.

Ctrl-C (SIGINT):

The Shell: Must ignore it. If I type Ctrl-C at the prompt, myshell should not close.

The Child: Must receive it. If I run sleep 10 and hit Ctrl-C, the sleep command should die, but the shell should live.

Child Death (SIGCHLD):

When a background process finishes, it becomes a "zombie." Your shell needs a signal handler that automatically cleans up these zombies so they don't clutter the process table.

5. Administrative Duties
These are the invisible requirements that are graded strictly.

Logging: Every time an external command runs, you must write the PID, command string, and exit status to myshell.log.

Constraint: You must use the write() system call, not fprintf.

Memory Hygiene: You must pass valgrind. This means no memory leaks. If you malloc a command struct, you must free it, even if the user types a command that fails.

Summary Checklist for Devs
If you are building your struct, it needs to be able to hold all this information at once. For example, a single command line might look like this:

grep "error" < input.txt > errors.log &

Your shell sees:

Command: grep

Args: "error"

Input Redirection: input.txt

Output Redirection: errors.log

Background Flag: True
