#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


void usage(void)
{
    fprintf(stderr, "usage: redirout filename command [arg1 [arg2 ... ] ]");
    exit(1);
}

int main(int argc, char* argv[]){

    if (argc < 3){
        usage();
    }
    else{
        char* filename = argv[1]
    }
}
