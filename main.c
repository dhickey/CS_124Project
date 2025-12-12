#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MaxLen 1024

int main() 
{
    char input[MaxLen];

    while(1)
    {
        printf("myprog> ");
        fflush(stdout);
        //reads input
        if (fgets(input, MaxLen, stdin) == NULL) 
        {
            printf("\n");
            break; // Exit on EOF
        }
        //removes \n
        input[strcspn(input, "\n")] = 0;

        if(input[0] == "\0")
        {
            continue;
        }

        //allocate mem
        char **args = malloc(64 * sizeof(char*));

        int i = 0;
        //delimits on space or tab
        char *token = strtok(input, " \t");
        while (token != NULL) 
        {
            args[i++] = token;
            token = strtok(NULL, " \t");
        }

        args[i] = NULL; // Null-terminate the array

        if(strcmp(args[0], "exit") == 0)
        {
            free(args);
            break;
        }

        if(strcmp(args[0], "cd") == 0)
        {
            char *target_dir = args[1];

            if(args[1] == NULL)
            {
                target_dir = getenv("HOME");
            }
            if (target_dir == NULL) 
            {
                fprintf(stderr, "myshell: cd: HOME not set ln57\n"); 
            } 
            else if (chdir(target_dir) != 0)
            {
                perror("myshell: cd");
            }
        }

        pid_t  pid = fork();

        if(pid == 0)
        {
            execvp(args[0] , args);
            perror("line 47, command not found");
            exit(1);
        }

        else if (pid > 0)
        {
            int status;
            waitpid(pid, &status, 0);
        }

        else
        {
            perror("Fork failed. line 58");
        }

        free(args);



    }   
    
    return 0;
}
