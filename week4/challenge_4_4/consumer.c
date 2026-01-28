#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(void){
    mkfifo("my fifo", 0644);

    int fd = open("my fifo", O_RDONLY);
    
    char buf[2048];

    int bytes = 1;
    while(bytes != 0){
        bytes = read(fd, buf, sizeof(buf));
        if (bytes > 0){
            write(1, buf, bytes);
        }
    }
}