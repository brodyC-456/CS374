#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void){
    mkfifo("my fifo", 0644);

    int fd = open("my fifo", O_RDONLY);
    
    char buf[16];

    int bytes = 1;
    while(bytes != 0){
        bytes = read(fd, buf, sizeof(buf));
        if(bytes != 0){
            printf("%d: %s\n", buf[0], buf + 1);
        }
    }
}