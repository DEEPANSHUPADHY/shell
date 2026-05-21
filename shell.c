#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAX_LINE 80

int main() {

    char *args[MAX_LINE/2 + 1];
    char input[1000];

    pid_t pid;
    int should_run = 1;

    while (should_run) {

        int count = 0;
        int background = 0;

        printf("osh> ");
        fflush(stdout);

        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = '\0';

        char *token = strtok(input, " ");

        while (token != NULL) {
            args[count++] = token;
            token = strtok(NULL, " ");
        }

        args[count] = NULL;

        if (count == 0)
            continue;

        // check for &
        if (strcmp(args[count - 1], "&") == 0) {
            background = 1;
            args[count - 1] = NULL;
        }

        pid = fork();

        if (pid == 0) {

            execvp(args[0], args);

            printf("Command not found\n");
            exit(1);

        } else if (pid > 0) {

            if (!background)
                wait(NULL);
        }
    }

    return 0;
}
