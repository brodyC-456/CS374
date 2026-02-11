#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

// The program exited immediately when I hit ^C.
// Sleep is returning how many seconds are left in the sleep time after exiting the process.

void sigint_handler(int sig)
{
    const char msg[] = "\nI got sigint!\n";
    write(1, msg, sizeof(msg));
}

int main(void){

    struct sigaction sa = {
        .sa_handler = sigint_handler,
        .sa_flags = 0, 
    };
    sigemptyset(&sa.sa_mask);

    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    puts("Program will exit in 10 seconds. Hit ^C!");
    int i = sleep(10);
    printf("%i\n", i);

}