#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


void usage(void)
{
    fprintf(stderr, "usage: redirout filename command [arg1 [arg2 ... ] ]");
    exit(1);
}
