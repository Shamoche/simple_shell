#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100

/**
 * display_prompt - Display the shell prompt
 */
void display_prompt(void)
{
    printf("$ ");
}

/**
 * read_command - Read a command from the user
 * Returns the entered command as a string
 */
char *read_command(void)
{
    char *command = NULL;
    size_t len = 0;
    ssize_t read;

    display_prompt();

    read = getline(&command, &len, stdin);

    if (read == -1) {
        if (feof(stdin)) {
            printf("\n");
            exit(EXIT_SUCCESS);
        } else {
            perror("getline");
            exit(EXIT_FAILURE);
        }
    }

    // Remove the trailing newline character
    command[strcspn(command, "\n")] = '\0';

    return command;
}

/**
 * execute_command - Execute the given command
 */
void execute_command(char *command)
{
    pid_t child_pid;
    int status;

    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process
        if (execlp(command, command, NULL) == -1) {
            fprintf(stderr, "Error: Command not found\n");
            exit(EXIT_FAILURE);
        }
    } else {
        // Parent process
        waitpid(child_pid, &status, 0);
    }
}

int main(void)
{
    char *command;

    while (1) {
        command = read_command();

        if (strlen(command) > MAX_COMMAND_LENGTH) {
            fprintf(stderr, "Error: Command is too long\n");
        } else {
            execute_command(command);
        }

        free(command);
    }

    return 0;
}

