#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/select.h>

#define DECREASE_NUM 1
#define INCREASE_NUM 2
#define MAX_INPUT_SIZE 128

// Global sig_atomic variables
volatile sig_atomic_t usr1_happened;
volatile sig_atomic_t usr2_happened;

// Signal Handlers

void sigusr1_handler(){
    usr1_happened = 1;
}

void sigusr2_handler(){
    usr2_happened = 1;
}

// Updates speed based on "type", prints a message, then returns the resulting speed
// Used the globals INCREASE_NUM and DECREASE_NUM to make it more clear
int change_speed(int s, int type){
    if (type == DECREASE_NUM) {
        // if we're already at 0 don't go below it (message could be different but spec doesn't want it to be)
        if (s == 0){
            printf("decrease speed to: %i\n", s);
            return 0;
        }
        s -= 1;
        printf("decrease speed to: %i\n", s);
        return s;
    }
    else if (type == INCREASE_NUM){
        s += 1;
        printf("increase speed to: %i\n", s);
        return s;
    }
    // This should never happen but i was getting a warning so i added it
    else{
        perror("invalid type");
        exit(0);
    }
}

// Handles the two different signals we'll be recieving, and returns the new speed
int signal_handler(int speed){
    if (usr1_happened == 1){
        speed = change_speed(speed, DECREASE_NUM);
        usr1_happened = 0;
    }
    else if (usr2_happened == 1){
        speed = change_speed(speed, INCREASE_NUM);
        usr2_happened = 0;
    }
    return speed;
}

// Handles all user inputs, returns the resulting speed
int input_handler(int speed, ssize_t size, char buf[]){
    if (buf[0] == 'q'){
        exit(0);
    }
    // iterate through input
    for (ssize_t i = 0; i < size; i++) {
        if (buf[i] == '+'){
            speed = change_speed(speed, INCREASE_NUM);
        }
        else if (buf[i] == '-'){
            speed = change_speed(speed, DECREASE_NUM);
        }
        else{
            continue;
        }
    }
    return speed;
}

int main(void){
    // Setting up signal structs
    struct sigaction sa1 = {
        .sa_handler = sigusr1_handler,
        .sa_flags = 0, 
    };
    sigemptyset(&sa1.sa_mask);

    struct sigaction sa2 = {
        .sa_handler = sigusr2_handler,
        .sa_flags = 0, 
    };
    sigemptyset(&sa2.sa_mask);

    // Blocks
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    // General Things
    printf("PID=%i\n", getpid());
    int speed = 0;
    char buf[MAX_INPUT_SIZE];

    // Defining Signals
    if (sigaction(SIGUSR1, &sa1, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    if (sigaction(SIGUSR2, &sa2, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    // Defining Sets (for pselect)
    sigset_t zero;
    sigemptyset(&zero);
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(0, &read_fds);

    // Main Loop
    while(1){
        int ps = pselect(1, &read_fds, NULL, NULL, NULL, &zero);
        // Singal Handling
        if (ps == -1 && errno == EINTR){
            speed = signal_handler(speed);
        }
        // Input Handler
        if (ps > 0){
            if (FD_ISSET(0, &read_fds)){
                // read
                ssize_t size = read(0, buf, sizeof(buf));
                speed = input_handler(speed, size, buf);
            }
        }
    }
}