#include <stdio.h>   
#include <fcntl.h>   
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>


void usage(void)
{
    fprintf(stderr, "usage: mymkdir [directory]\n");
    exit(1);
}

int main(int argc, char *argv[]){
    if(argc != 2){
        usage();
    }

    char *dir = argv[1];

    if (mkdir(dir, 0777) == -1){
        perror(dir);
    }
    
}

