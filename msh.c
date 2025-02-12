#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// msh - maverick shell program
// made by Mariah Gardner 1001576678
// complied with gcc -o msh msh.c

#define MAX_INPUT 100 // maximum input size
#define MAX_PARAMS 10 // maximum additional command-line parameters

// function to get user input 
// params: input_command - the buffer to store user input
// return: 0 on success, 1 on error
int get_user_input(char *input_command) {
    printf("msh> "); // prompt for user input
    if (fgets(input_command, MAX_INPUT, stdin) == NULL) { // use fgets to get user input, return 1 on error
        printf(" Error reading input.\n");
        return 1;
    }
    input_command[strcspn(input_command, "\n")] = 0; // remove newline character
    return 0;
}

// function to change directory
// params: path - the directory to change to
// return: void
void change_directory(char *path) {
    if (path == NULL) { // if no path is provided, print usage message
        printf(" Usage: cd <directory>\n"); 
        return;
    }
    if (chdir(path) != 0) { // use chdir to change directory, return error message on failure
        perror(" cd failed"); // error handling for chdir failure
    }
}

// Function to execute user commands
// params: input_command - the command to execute
// return: void
void execute_command(char *input_command) {
    char *args[MAX_INPUT / 2 + 1]; // array to hold command-line arguments (to be tokenized) (MAX_INPUT/2 is a rough estimate of the maximum number of arguments) (+1 for null terminator)
    int i = 0;
    char *token = strtok(input_command, " "); // tokenize input_command by spaces
    
    while (token != NULL && i < (MAX_INPUT / 2)) { // loop through tokens until NULL or max args reached
        args[i++] = token; // store token in args array by index
        token = strtok(NULL, " "); // get next token
    }
    args[i] = NULL; // execvp requires NULL-terminated array of strings
    
    if (args[0] == NULL) {
        return; // if no command is entered, return
    }
    
    // if the command is "quit", "exit", or "q", terminate the shell
    if (strcmp(args[0], "quit") == 0 || strcmp(args[0], "exit") == 0 || strcmp(args[0], "q") == 0) {
        printf(" Exiting program. \n");
        exit(0); // exit the program by calling exit(0)
    }

    // if the command is "cd", change directory
    if (strcmp(args[0], "cd") == 0) { 
        change_directory(args[1]);
        return;
    }
    
    // fork a child process to execute the command
    pid_t pid = fork();
    if (pid < 0) {
        perror(" Fork failed"); // error handling for fork failure
        return;
    } else if (pid == 0) { // this is the child process
        execvp(args[0], args); // use execvp to execute the command
        perror(" Command execution failed"); // error handling for exec failure
        exit(1); // terminate child process on exec failure
    } else {
        wait(NULL); // wait for child process to finish
    }
}

int main(int argc, char *argv[]) {
    // if command-line arguments are provided, validate them
    if (argc > 1) {
        if (argc - 1 > MAX_PARAMS) {
            printf(" Error: Too many parameters. Max allowed is %d.\n", MAX_PARAMS);
            return 1;
        }
        return 0;
    }

    // interactive loop to get user input
    char input_command[MAX_INPUT]; // buffer to hold user input
    while (1) { // loop until user exits
        get_user_input(input_command); // get user input
        execute_command(input_command); // process the command
    }
    return 0;
}

