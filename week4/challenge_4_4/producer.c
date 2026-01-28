#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
    char* content = argv[1];
    
    mkfifo("my fifo", 0644);
    int fd = open("my fifo", O_WRONLY);

    write(fd, content, strlen(content));
    close(fd);
    exit(1);
}