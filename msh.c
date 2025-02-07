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

// function to get user input
int get_user_input(char *input_command) {
    // reads user input from stdin then stores it in input_command
    if (fgets(input_command, MAX_INPUT, stdin) == NULL) { 
        printf("Error reading input.\n"); // if fgets fails, print error message
        return 1;
    }
    input_command[strcspn(input_command, "\n")] = 0;    // remove the trailing newline character
    return 0;
}

// function to validate user command
int validate_command(const char *input_command, const char *valid_commands[], int num_commands) {
    if (strcmp(input_command, "quit") == 0) {
        return 0; // signals to exit the program by setting programOnOff to 0
    }

    // perform a binary search to find the input_command in the valid_commands array
    const char *search_key = input_command;
    const char **found = (const char **)bsearch(&search_key, valid_commands, num_commands, sizeof(char *), compare_strings);
    
    // if command is found, print a success message
    if (found != NULL) {
        printf("Command '%s' is valid.\n", input_command);
    } else {
        printf("Command not found.\n");
    }
    return 1; // continue program execution by keeping programOnOff as 1
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

        // declare input_command 
        char input_command[MAX_INPUT];

        // get user input and store it in input_command
        get_user_input(input_command);

        // validate user input by comparing it to the list of valid commands
        // if input_command is "quit", programOnOff is set to 0 and the program exits
        programOnOff = validate_command(input_command, valid_commands, num_commands);

    }
    return 0;
}