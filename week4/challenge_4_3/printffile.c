#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>


int main(void){
    int fd = open("output.txt", O_CREAT|O_TRUNC|O_WRONLY, 0644);

    if (fd == -1){
        perror("open");
        exit(1);
    }

    if (dup2(fd, 1) == -1){
        perror("dup2 error");
        exit(1);
    }

    printf("Hello, world\nThis is a test!\n");
    fflush(stdout);
    close(fd);
}

// Answer to the question: If we want to save the original stdout, we could do something like "stdout2 = dup(1)" to create a 
// temporary stdout that we can use in dup2. Then, in printf we could specify that we are printing to stdout2, and afterwards
// close stdout2 so we start printing to the original stdout in the future.