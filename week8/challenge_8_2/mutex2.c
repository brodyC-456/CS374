#include <stdio.h>
#include <pthread.h>

#define ELEMENTS 1000000
#define THREADS 4

// Q1

// Locking inside the iteration is way slower than locking outside of the iteration, most likely because the locks and unlocks are being applied over and over again in the iteration which adds computing time.

// Q2

// The spin lock takes longer than the mutexes because they are constantly checking for the spinlock variable, which uses more computing resources than simple mutexes.

int a[ELEMENTS] = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
volatile int spinlock1 = 0;

void lock(void) {
    while (__sync_lock_test_and_set(&spinlock1, 1))
        ;
}

void unlock(void) {
    __sync_synchronize(); // Memory barrier.
    spinlock1 = 0;
}

void *run(void *arg)
{
    (void)arg;
    
    lock();
    for (int i = 0; i < ELEMENTS; i++){  
        a[i]++;   
    }
    unlock();
    return NULL;
}

int main(void)
{
    pthread_t thread[THREADS];

    for (int i = 0; i < THREADS; i++)
        pthread_create(thread + i, NULL, run, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(thread[i], NULL);

    int expected = THREADS;

    for (int i = 0; i < ELEMENTS; i++)
        if (a[i] != expected)
            printf("a[%d] = %d, expected %d\n", i, a[i], expected);
}