#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

int main(void){

    struct sigaction sa = {
        .sa_handler = SIG_IGN,
        .sa_flags = 0, 
    };
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    puts("^C locked out for 5 seconds! Try it!");
    sleep(5);

    sa.sa_handler = SIG_DFL;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    
    puts("\n^C reenabled! Try it!");
    sleep(10);

}