#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define DATA_SIZE 1024

void usage(){
    fprintf(stderr, "usage: mmap_write int float 'string'\n");
    exit(1);
}

int main(int argc, char* argv[]){
    if (argc != 4) {
        usage();
        exit(1);
    }

    int i = atoi(argv[1]);
    float f = atof(argv[2]);
    char *s = argv[3];

    int fd = open("data.dat", O_RDWR);
    void *data = mmap(NULL, DATA_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    int *p = data;
    float *q = data + sizeof(int);
    char *r = data + sizeof(int) + sizeof(float);

    *p = i;
    *q = f;
    strcpy(r, s);

    printf("Wrote: %d, %f, %s\n", *p, *q, r);
    
    munmap(data, DATA_SIZE);
    close(fd);

    

}