#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define DATA_SIZE 1024


int main(){
    int fd = open("data.dat", O_RDONLY);
    void *data = mmap(NULL, DATA_SIZE, PROT_READ, MAP_SHARED, fd, 0);

    int *p = data;
    float *q = data + sizeof(int);
    char *r = data + sizeof(int) + sizeof(float);

    printf("Read: %d, %f, %s\n", *p, *q, r);

    munmap(data, DATA_SIZE);
    close(fd);
}