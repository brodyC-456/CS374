#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/wait.h>

#define ERROR_CODE -1
#define CHILD_PID 0
#define MAX_CMND_SIZE 2048
#define MAX_TOKENS 128

// Handles Directory Changing
void handle_cd(char *dir){
    if (chdir(dir) == ERROR_CODE){
        perror(dir);
    }
}

// Handles all non-cd, non-exit commands
void handle_command(char *tokens[]){

    // Fork a process
    pid_t pid = fork();

    // Run it if its the child process, wait if its the parent.
    if (pid == CHILD_PID){
        if (execvp(tokens[0], tokens) == ERROR_CODE){
            perror("Error");
                
        }
        exit(1);
    }
    else{
        wait(NULL);
    }
}

// Handles Cases for Running Commands
void run_command(char *tokens[]){

    // If a user hits enter, exit back to our main loop (create a new line)
    if (tokens[0] == NULL){
        return;
    }

    // CD Handling
    if(strcmp(tokens[0], "cd") == 0){
        handle_cd(tokens[1]);
    }

    // Exit Handling
    else if (strcmp(tokens[0], "exit") == 0){
        exit(0);
    }

    // Normal Commands
    else {
        handle_command(tokens);
    }
}

// Sets up prompt, reads input, splits it up into tokens, then runs the command
int main(void){
    while(1){
        // Print a prompt
        printf("mush$ ");
        fflush(stdout);

        // Read input from command line
        char line[MAX_CMND_SIZE];
        fgets(line, sizeof line, stdin);

        if (feof(stdin)){
            exit(0);
        }

        // Create tokens array, token, and Iterator
        char *token;
        char *tokens[MAX_TOKENS] = {0};
        int i = 0;

        // Read "line", split on space and \n, and store each token in the tokens array
        if ((token = strtok(line, " \n")) != NULL) do {
            tokens[i] = token;
            i++;
        } while((token = strtok(NULL, " \n")) != NULL);

        // Run the Command
        run_command(tokens);
        
    }
}