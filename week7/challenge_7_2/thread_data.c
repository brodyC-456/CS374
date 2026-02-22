#include <stdio.h>
#include <pthread.h>

struct ThreadArgs {
    char *hello;
    char *goodbye;
};

void *run(void *args_void){
    struct ThreadArgs *args = (struct ThreadArgs *)args_void;
    printf("%s\n", args->hello);
    printf("%s\n", args->goodbye);
    return NULL;
}

int main(void){

    struct ThreadArgs args;
    args.hello = "child: hello!";
    args.goodbye = "child: goodbye!";

    pthread_t t1;
    puts("parent: hello!");
    pthread_create(&t1, NULL, run, &args);
    pthread_join(t1, NULL);
    puts("parent: goodbye!");
}