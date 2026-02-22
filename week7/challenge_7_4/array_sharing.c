#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10
#define INT_SIZE 100000

void *run(void *args){
    int *a = args;
    for (int i = 0; i < INT_SIZE; i++){
        a[i] += 1;
    }
    return NULL;
}

int main(void){

    pthread_t threads[NUM_THREADS];
    int a[INT_SIZE] = {0};
    
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, run, &a);
    }

    for (int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < INT_SIZE; i++){
        if (a[i] != 10){
            printf("a[%i] == %i, expected %i\n", i, a[i], NUM_THREADS);
        }      
    }

    puts("Done!");
}

// The output is probably not as expected or consistent because threads are running concurrently and therefore overwriting eachother