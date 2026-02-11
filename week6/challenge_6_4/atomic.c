#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t usr1_happened;

void sigusr1_handler()
{
    usr1_happened = 1;
}

int main(void){

    struct sigaction sa = {
        .sa_handler = sigusr1_handler,
        .sa_flags = 0, 
    };
    sigemptyset(&sa.sa_mask);

    
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("Go to another window and `kill -USR1 %d`\n", getpid());

    while(usr1_happened == 0){
        sleep(10);
    }

}

// Questions
    // 1: Continually checking if the flag is set uses a lot of CPU resources so it's better to just use sleep()
    // 2: The volatile keyword prevents the compiler from optimizing the variable by creating registers for it, forcing it to
    // change the variable directly in memory. This is needed for sig_atomic_t because we are changing the variable in both the
    // signal handler and the main code, so if it's made into a register it might be overwritten.