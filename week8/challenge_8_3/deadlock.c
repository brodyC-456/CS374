#include <stdio.h>
#include <pthread.h>

#define ELEMENTS 50
#define THREADS 8

pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER;

int a[ELEMENTS] = {0};
int b[ELEMENTS] = {0};

// Why did the deadlock occur?
// First, thread 1 acquires lock a. Then, thread 2 acquires lock B. 
// In the next lines, this results in thread 1 waiting to acquire lock B that thread 2 has, and thread 2 waiting to acquire lock A that thread 1 has.
// This results in a deadlock.

void *run1(void *arg)
{
    (void)arg;

    for (int i = 0; i < ELEMENTS; i++) {
        pthread_mutex_lock(&lock_b);
        pthread_mutex_lock(&lock_a);
        a[i]++;
        pthread_mutex_unlock(&lock_a);
        pthread_mutex_unlock(&lock_b);
    }

    return NULL;
}

void *run2(void *arg)
{
    (void)arg;

    for (int i = 0; i < ELEMENTS; i++) {
        pthread_mutex_lock(&lock_b);
        pthread_mutex_lock(&lock_a);
        a[i]++;
        pthread_mutex_unlock(&lock_a);
        pthread_mutex_unlock(&lock_b);
    }

    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, run1, NULL);
    pthread_create(&t2, NULL, run2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}