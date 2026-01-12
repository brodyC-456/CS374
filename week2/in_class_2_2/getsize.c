#include <stdio.h>  
#include <sys/stat.h>

int main(int argc, char *argv[]){

    for (int i = 1; i < argc; i++){
        struct stat buf;
        stat(argv[i], &buf);
        printf("Size: %ld bytes\n", buf.st_size);
    }

}