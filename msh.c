#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// msh - maverick shell program
// made by Mariah Gardner 1001576678
// complied with gcc -o msh msh.c

#define MAX_INPUT 100 // maximum input size

// function to compare two strings
int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(int argc, char *argv[]) {

    // track if program is on or off
    int programOnOff = 1;

    //prompt user for input
    printf("\n msh> ");

    // list of acceptable commands 
    const char *valid_commands[] = {"ls"};
    int num_commands = sizeof(valid_commands) / sizeof(valid_commands[0]);

    // quick sort the valid commands to be able to run a binary search
    qsort(valid_commands, num_commands, sizeof(char *), compare_strings);

    // loop until user enters quit command
    while(programOnOff) {

        // get user input
        char input_command[MAX_INPUT];
        if (fgets(input_command, MAX_INPUT, stdin) == NULL) {
            printf("Error reading input.\n");
            return 1;
        }

        // Remove the trailing newline character if present
        input_command[strcspn(input_command, "\n")] = 0;

        // validate user's command
        if (strcmp(input_command, "quit") == 0) { // if user enters 'quit', end the program
            programOnOff = 0; // terminate program by breaking while loop
        } else { // else, search for the user's command in the valid commands list
            const char *search_key = input_command;
            const char **found = (const char **)bsearch(&search_key, valid_commands, num_commands, sizeof(char *), compare_strings);

            if (found != NULL) {
                // if command is found, print a success message
                printf("Command '%s' is valid.\n", input_command);
            } else {
                // if command is not found, print an error message
                printf("Command not found.\n");
            }
        }
    }
    return 0;
}