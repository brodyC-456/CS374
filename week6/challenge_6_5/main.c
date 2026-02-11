#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>


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

    sigaddset(&sa.sa_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL);
    puts("SIGUSR1 is blocked");

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    puts("Reading from the keyboard. But don't type anything.");
    printf("Go to another window and `kill -USR1 %d` several times.\n", getpid());
    puts("Then press RETURN to continue.");
    
    char* buf[128];
    if (read(0, buf, sizeof(buf)) == -1){
        perror("read");
        exit(1);
    }

    sigprocmask(SIG_UNBLOCK, &sa.sa_mask, NULL);
    puts("SIGUSR1 is unblocked");

}

// When multiple signals are blocked, they converge into one single signal, which is why only one came through after we hit return