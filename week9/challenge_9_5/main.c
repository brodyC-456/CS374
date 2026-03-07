#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_COUNT 10

// You would have to call sem_wait() from a different thread to close the turnstile at the end.

// TODO: Add some semaphores
// TODO: Add a global `count` variable

sem_t *mut, *sem;
int count = 0;

sem_t *sem_open_temp(const char *name, unsigned int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0666, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

void *run(void *args)
{
    int thread_id = *(int*)args;

    // TODO: Add turnstile stuff

    sem_wait(mut);
    count++;
    sem_post(mut);

    if (count == THREAD_COUNT){
        sem_post(sem);
        printf("Thread %i: Opening turnstile\n", thread_id);
    }
    printf("Thread %i: Waiting for turnstile\n", thread_id);
    sem_wait(sem);
    printf("Thread %i: In turnstile\n", thread_id);
    count--;
    printf("Thread %i: Reopening turnstile\n", thread_id);
    sem_post(sem);


    printf("Thread %i: Complete\n", thread_id);
    return NULL;
}

int main(void)
{
    pthread_t thread[THREAD_COUNT];
    int thread_id[THREAD_COUNT];

    // TODO: Add sem_open_temp() calls
    mut = sem_open_temp("mut", 1);
    sem = sem_open_temp("sem", 0);

    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_id[i] = i;
        pthread_create(thread + i, NULL, run, thread_id + i);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(thread[i], NULL);

    // TODO: Add sem_close() calls
    sem_close(sem);
    sem_close(mut);
}