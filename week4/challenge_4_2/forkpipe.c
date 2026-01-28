#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void){

    int pfds[2];  
    pipe(pfds);
    
    char *buf[128];

    pid_t pid = fork();

    if (pid == 0){
        close(pfds[0]);
        write(pfds[1], "hello, parent!\n", 15);
        exit(1);
    }
    else{
        close(pfds[1]);
        size_t byte_len = read(pfds[0], buf, 128);
        write(1, buf, byte_len);
        wait(NULL);
    }
}