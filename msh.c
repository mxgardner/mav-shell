#include <stdio.h>
#include <stdlib.h>

// msh - maverick shell program
// made by Mariah Gardner 1001576678
// complied with gcc -o msh msh.c

int main(int argc, char *argv[]) {

    // track if program is on or off
    int programOnOff = 1;

    //prompt user for input
    printf("\n msh> ");

    // loop until user enters 'q'
    while(programOnOff) {
        char command[100]; // buffer for user input
        fgets(command, sizeof(command), stdin); // read user input

        // check if user entered 'q'
        if (command[0] == 'q') { 
            programOnOff = 0; // terminate program if 'q' is entered
        } else {
            // else, do the command
        }
    }
    return 0;
}