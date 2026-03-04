#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// If you use signal instead of broadcast, the wrong thread could be called at the halfway point, resulting in the first thread being called in the next iteration and therefore being late.

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int count = 0;

void *run1(void *arg)
{
    (void)arg;

    while (count > 3){
        pthread_mutex_lock(&mut);
        pthread_cond_wait(&cond, &mut);
        pthread_mutex_unlock(&mut); 
    }

    puts("Halfway There!");
    return NULL;
}

void *run2(void *arg){
    (void)arg;

    while (count != 0){
        pthread_mutex_lock(&mut);
        pthread_cond_wait(&cond, &mut);
        pthread_mutex_unlock(&mut); 
    }
    puts("Liftoff!");
    return NULL;

}


int main(void)
{
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, run1, NULL);
    pthread_create(&t2, NULL, run2, NULL);

    count = 6;

    while (count > 0) {
        pthread_mutex_lock(&mut);
        count--;
        printf("%i\n", count);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mut);
        sleep(1);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}