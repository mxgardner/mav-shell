#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

// msh - maverick shell program
// made by Mariah Gardner 1001576678
// complied with gcc -o msh msh.c

#define MAX_INPUT 100 // maximum command input length
#define MAX_PARAMS 10 // maximum additional command-line parameters
#define MAX_HISTORY 15 // maximum number of commands to store in history

pid_t pid_history[MAX_HISTORY]; // array to hold process IDs of executed commands
int history_count = 0; // counter for number of commands executed
char command_history[MAX_HISTORY][MAX_INPUT]; // array to hold command history
int command_count = 0; // counter for number of commands in history

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
// if executed in a child process, would change the directory within the child but not affect the parent shell
void change_directory(char *path) {
    if (path == NULL) { // if no path is provided, print usage message
        printf(" Usage: cd <directory>\n"); 
        return;
    }
    if (chdir(path) != 0) { // use chdir to change directory 
        perror(" cd failed"); // return error message
    }
}

// function to print current working directory
// return: void
// if executed in a child process, would print the directory within the child but not affect the parent shell
void print_working_directory() {
    char cwd[1024]; // buffer to hold current working directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) { // use getcwd to get current working directory
        printf("Current Directory: %s\n", cwd); // print current working directory
    } else {
        perror(" getcwd failed"); // error handling for getcwd failure
    }
}

// function to list files in current directory
// return: void
// if executed in a child process, would list the files within the child but not affect the parent shell
void list_files() {
    DIR *dir; // struct to hold directory stream, defined in dirent.h
    struct dirent *entry; // struct to hold directory entries, defined in dirent.h
    // use opendir() from dirent.h to open current directory
    if ((dir = opendir(".")) == NULL) { // "." means current directory, if current directory cannot be opened, return error message
        perror(" opendir failed"); // e.g. dont have permissions to open directory
        return;
    }
    while ((entry = readdir(dir)) != NULL) { // use readdir to read directory entries, similar to read() in stdio.h, prints all files in current directory into the buffer
         // readdir returns NULL when there are no more entries to read
        printf("%s  ", entry->d_name); // print each file name from the buffer
    }
    printf("\n"); // print newline after listing files
    closedir(dir);
}

// function to print process ID history
// return: void
void print_pid_history() {
    printf("Last %d process IDs:\n", (history_count < MAX_HISTORY) ? history_count : MAX_HISTORY); // print number of process IDs stored in history
    for (int i = 0; i < history_count; i++) { // loop through process ID history
         // print each process ID stored in the array
         // if there are more than MAX_HISTORY process IDs, only print the most recent MAX_HISTORY
        printf("%d\n", pid_history[i]); // print each process ID
    }
}

// function to print command history
// return: void
void print_command_history() {
    for (int i = 0; i < command_count; i++) {
        printf("%d %s\n", i + 1, command_history[i]); // print each command in history with its index, add 1 to index to start from 1 instead of 0
    }
}

// Function to execute user commands
// params: input_command - the command to execute
// return: void
void execute_command(char *input_command) {

    // store command in history
     if (command_count < MAX_HISTORY) { // if there is space in the history array
        strcpy(command_history[command_count++], input_command); // store command in history array
    } else {
        for (int j = 1; j < MAX_HISTORY; j++) { // if there is no space in the history array, shift all elements to the left to make space for the new command
            strcpy(command_history[j - 1], command_history[j]);
        }
        strcpy(command_history[MAX_HISTORY - 1], input_command); // store new command in last position
    }
    
    if (input_command[0] == '!' && strlen(input_command) > 1) { // if the command starts with '!', it is a history command
        int index = atoi(&input_command[1]); // convert the string to an integer
         // if the index is valid, copy the command from history to input_command
         // and re-execute it
        if (index >= 0 && index < command_count) {  // check if index is valid
            strcpy(input_command, command_history[index]); // copy command from history to input_command
            printf("Re-executing: %s\n", input_command);
        } else {
            printf("Command not in history.\n"); // if the index is invalid, print error message
            return;
        }
    }

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

    // handle some common commands without forking a new process 
    // if the command is "cd", change directory
    if (strcmp(args[0], "cd") == 0) { 
        change_directory(args[1]);
        return;
    }

    // if the command is "pwd", print current working directory
    if (strcmp(args[0], "pwd") == 0) {
        print_working_directory();
        return;
    }

    // if the command is "ls", list files in current directory
    if (strcmp(args[0], "ls") == 0) {
        list_files();
        return;
    }

    // if the command is "clear", clear the screen
    // simple command that does not require a new process
    if (strcmp(args[0], "clear") == 0) {
        printf("\033[H\033[J"); // ANSI escape codes to clear the screen
        return;
    }

    // if the command is "pidhistory", print process ID history
    if (strcmp(args[0], "pidhistory") == 0) {
        print_pid_history();
        return;
    }

    // if the command is "history", print command history
    if (strcmp(args[0], "history") == 0) {
        print_command_history();
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
        if (history_count < MAX_HISTORY) { // if there is space in the history array
            pid_history[history_count++] = pid; // store process ID in history array
        } else {
            for (int j = 1; j < MAX_HISTORY; j++) { // if there is no space in the history array, shift all elements to the left
                 // to make space for the new process ID
                pid_history[j - 1] = pid_history[j];
            }
            pid_history[MAX_HISTORY - 1] = pid; // store new process ID in last position
        }
        wait(NULL); // wait for child process to terminate
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

