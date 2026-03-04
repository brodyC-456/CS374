#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// When I called sleep(3), the signal calls from the other thread seemed to all happen at the same time.

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int count = 0;

void *run(void *arg)
{
    (void)arg;

    while (count < 5){
        pthread_mutex_lock(&mut);
        count++;
        printf("Thread 2: count is %i\n", count);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mut); 
        sleep(1);
    }

    return NULL;
}

int main(void)
{
    pthread_t t1;

    pthread_create(&t1, NULL, run, NULL);

    while (count < 5) {
        pthread_mutex_lock(&mut);
        pthread_cond_wait(&cond, &mut);
        printf("Thread 1: count is %i\n", count);
        pthread_mutex_unlock(&mut);

    }

    pthread_join(t1, NULL);
}