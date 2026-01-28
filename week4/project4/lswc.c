#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void usage(void)
{
    fprintf(stderr, "usage: lswc [pathname]");
    exit(1);
}

// Forks a child process then runs wc on the read end of the pipe and ls on the write end of the pipe
void run_pipe(int pfds[2], char* path){
    pid_t pid = fork();

    if(pid == 0){
        dup2(pfds[0], 0);
        close(pfds[1]);
        execlp("wc", "wc", "-l", (char*) NULL);
    }
    else{
        dup2(pfds[1], 1);
        close(pfds[0]);
        execlp("ls", "ls", "-1a", path, (char*) NULL);
    }
}

int main(int argc, char* argv[]){

    // Parsing Command Line Argument
    char* path;
    if(argc > 2){
        usage();
    }
    else if(argc == 2){
        path = argv[1];
    }
    else{
        path = ".";
    }

    // Sets up Pipe

    int pfds[2];  
    pipe(pfds);

    // Uses the Pipe
    run_pipe(pfds, path);

}



