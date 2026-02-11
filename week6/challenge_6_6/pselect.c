#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/select.h>

volatile sig_atomic_t usr1_happened;

void sigusr1_handler()
{
    usr1_happened = 1;
}

int main(void){

    struct sigaction sa = {
        .sa_handler = sigusr1_handler,
        .sa_flags = 1, 
    };
    sigemptyset(&sa.sa_mask);

    sigaddset(&sa.sa_mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL);
    puts("SIGUSR1 is blocked");

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    sigset_t zero;
    sigemptyset(&zero);
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(0, &read_fds);

    puts("Either enter some text, or...");
    printf("go to another window and `kill -USR1 %d`.\n", getpid());

    int ps = pselect(1, &read_fds, NULL, NULL, NULL, &zero);

    if (ps == -1 && errno == EINTR){
        if (usr1_happened == 1){
            puts("Got USR1");
        }
    }
    else if (ps > 0){
        if (FD_ISSET(0, &read_fds)){
            char* buf[128];
            size_t size = read(0, buf, sizeof(buf));
            write(1, buf, size);
        }
    }
}