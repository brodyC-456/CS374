#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_COUNT 4

int shared_value = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Q1
// The problem can manifest with 2 threads because there's a small chance they run concurrently, but it cannot manifest with 1 thread.

// Q2
// First, one thread must run with the shared value set to 1 so that it deincrements it. 
// At the same time, another thread must run, and since the data isnt yet updated by the other thread, it thinks the shared value is 1 so it deincrements. 
// Since the value was deincremented twice, it is now negative.

void *run(void *arg)
{
    (void)arg;
    
    pthread_mutex_lock(&mutex);
    if (shared_value > 0) {
        usleep(1);
        shared_value--;
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_create(threads + i, NULL, run, NULL);

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(threads[i], NULL);

    if (shared_value < 0)
        printf("How'd shared_value get to be %d?\n", shared_value);
    else
        puts("OK");
}