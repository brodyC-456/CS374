#include <unistd.h>

int main(void){

    int pfds[2];  
    pipe(pfds);
    
    char *buf[128];

    write(pfds[1], "hello\n", 6);
    size_t byte_len = read(pfds[0], buf, 128);
    write(1, buf, byte_len);
}