#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define ERROR_CODE -1
#define FILE_ARG_INDEX 1
#define CMND_ARG_INDEX 2

void usage(void)
{
    fprintf(stderr, "usage: redirout filename command [arg1 [arg2 ... ] ]");
    exit(1);
}

void add_command_output_to_file(char* filename, char* cmnd, char** args){
    int fd;
    if((fd = open(filename, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR | S_IWUSR)) == ERROR_CODE){
        perror("file open error");
        exit(1);
    }

    dup2(fd, 1);
    if(execvp(cmnd, args) == ERROR_CODE){
        perror("Invalid Command");
        exit(1);
    }
}

int main(int argc, char* argv[]){
    char* filename;
    char* cmnd;
    char** args;

    if (argc < 3){
        usage();
    }
    else{
        filename = argv[FILE_ARG_INDEX];
        cmnd = argv[CMND_ARG_INDEX];
        args = &argv[CMND_ARG_INDEX];
    }

    add_command_output_to_file(filename, cmnd, args);
}
