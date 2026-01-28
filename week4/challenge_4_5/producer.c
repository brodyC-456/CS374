#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
    (void) argc;
    int count = atoi(argv[1]);
    if(count > 255 || count < 1){
        perror("too many messages");
        exit(0);
    }


    mkfifo("my fifo", 0644);
    int fd = open("my fifo", O_WRONLY);
    
    for (int i = 0; i <= count; i++){
        char buf[16];
        buf[0] = i;
        sprintf(buf + 1, "Message %d", i);
        write(fd, buf, sizeof(buf));
        sleep(1);
    }

    close(fd);
    exit(1);
}