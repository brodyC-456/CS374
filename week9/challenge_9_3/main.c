#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THREAD_COUNT 15

// To make 3 threads run simultaneously, I just changed the initial value of the semaphore to 3 instead of 1.

sem_t *sem;

// Grab the sem_open_temp() function and drop it here!

sem_t *sem_open_temp(const char *name, unsigned int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

void *run(void *arg)
{
    
    sem_wait(sem);
    
    int *id = arg;

    printf("Thread %02d: running\n", *id);

    sleep(1);  // This is important work

    printf("Thread %02d: exiting\n", *id);
    sem_post(sem);

    return NULL;
    
}

int main(void)
{
    // You can add code here, but...
    sem = sem_open_temp("main", 3);

    // Do not modify code between these marks vvv
    pthread_t runners[THREAD_COUNT];
    int runner_id[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        runner_id[i] = i;
        pthread_create(runners + i, NULL, run, runner_id + i);  // Run, runner!
    }

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(runners[i], NULL);
    // Do not modify code between these marks ^^^

    sem_close(sem);

    
}