#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// msh - maverick shell program
// made by Mariah Gardner 1001576678
// complied with gcc -o msh msh.c

#define MAX_INPUT 100 // maximum input size
#define MAX_PARAMS 10 // maximum additional command-line parameters

// store list acceptable commands globally
const char *valid_commands[] = {"ls", "pwd", "quit", "exit", "q"};
int num_commands = sizeof(valid_commands) / sizeof(valid_commands[0]);

// function to compare two strings
// params: a - first string, b - second string
// return: 0 if equal, non-zero otherwise
int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// function to get user input 
// params: input_command - the buffer to store user input
// return: 0 on success, 1 on error
int get_user_input(char *input_command) {
    printf(" msh> ");
    if (fgets(input_command, MAX_INPUT, stdin) == NULL) {
        printf(" Error reading input.\n");
        return 1;
    }
    input_command[strcspn(input_command, "\n")] = 0;
    return 0;
}

// function to validate user command 
// params: input_command - the command to validate
// valid_commands - list of valid commands
// num_commands - number of valid commands
// return: void
void validate_command(const char *input_command, const char *valid_commands[], int num_commands) {
    const char *search_key = input_command;
    const char **found = (const char **)bsearch(&search_key, valid_commands, num_commands, sizeof(char *), compare_strings); 
    
    if (found != NULL) {
        printf(" Command '%s' is valid.\n", input_command);
    } else {
        printf(" Command not found.\n");
    }
}

int main(int argc, char *argv[]) {
    // quick sort the valid commands to be able to run a binary search
    qsort(valid_commands, num_commands, sizeof(char *), compare_strings);

    // if command-line arguments are provided, validate them
    if (argc > 1) {
        if (argc - 1 > MAX_PARAMS) {
            printf(" Error: Too many parameters. Max allowed is %d.\n", MAX_PARAMS);
            return 1;
        }
        validate_command(argv[1], valid_commands, num_commands);
        return 0;
    }

    // interactive loop to get user input
    char input_command[MAX_INPUT]; // buffer to hold user input
    while(1) { // loop until user enters a quit command
        // get user input and validate it by searching the list of valid commands
        get_user_input(input_command); // get user input
        validate_command(input_command, valid_commands, num_commands); // validate user input bv using bsearch

        // use string comparison to check what command was entered and run it
        // if the command is quit, exit, or q, break the loop
        if (strcmp(input_command, "quit") == 0 || strcmp(input_command, "exit") == 0 || strcmp(input_command, "q") == 0) {
            printf(" Exiting program. \n");
            break;
        } else if (strcmp(input_command, "ls") == 0) { // if the command is ls, run the ls command
            printf(" ls command executed. \n");
        } else if (strcmp(input_command, "pwd") == 0) {
            printf(" pwd command executed. \n");
        }
    }
    return 0;
}

// execvp 
