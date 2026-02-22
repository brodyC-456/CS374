#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 3

void *run(void *args){
    (void)args;
    puts("running");
    return NULL;
}

int main(void){

    pthread_t threads[NUM_THREADS];
    puts("parent: hello!");
    
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, run, NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    puts("parent: goodbye!");
}