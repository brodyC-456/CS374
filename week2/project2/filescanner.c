#include <stdio.h>
#include <sys/stat.h>


int main(int argc, char *argv[]){
    char* dir;
    if (argc == 1){
        dir = ".";
    }
    else if (argc == 2)
    {
        dir = argv[1];
    }
    else {
        usage()
    }

    printf(count_bytes(dir));

}

int count_bytes(dir){
    printf("this will count the bytes at some point");
}

void usage(void)
{
    fprintf(stderr, "usage: filescanner [directory]");
    exit(1);
}


