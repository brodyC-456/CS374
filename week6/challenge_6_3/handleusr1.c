#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

// SA_RESTART makes it so the process doesn't exit when it recieves the usr1 signal.

void sigusr1_handler(int sig)
{
    (void)sig;
    write(1, "SIGUSR1 handled\n", 16);
}

int main(void){

    struct sigaction sa = {
        .sa_handler = sigusr1_handler,
        .sa_flags = SA_RESTART, 
    };
    sigemptyset(&sa.sa_mask);

    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    puts("Reading from the keyboard. But don't type anything.");
    printf("Go to another window and `kill -USR1 %d`\n", getpid());
    
    char* buf[128];
    if (read(0, buf, sizeof(buf)) == -1){
        perror("read");
        exit(1);
    }

}