#include <stdio.h>   
#include <fcntl.h>   
#include <unistd.h>

int main(int argc, char *argv[])
{
    // Integer to hold the file descriptior
    int fd;
    fd = open(argv[1], 0600);

    if (fd == -1) {
        // If the open fails, print out an error message prepended with
        // "open"
        perror("open");
        return 1;  // Exit with error code 1
    }

    char buf[2048];
    int bytes_read = read(fd, buf, 2048);
    write(1, buf, bytes_read);

    // Close the file when done
    close(fd);
}