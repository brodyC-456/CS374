#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

// If each thread waited before posting, there would be a deadlock because the other thread waiting on the semaphore would never get a value of 1 and therefore never stop waiting.

// TODO: Add some semaphores
sem_t *sem1;
sem_t *sem2;

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
    int *params = args;
    int thread_id = params[0];
    int sleep_time = params[1];

    printf("Thread %d: sleeping for %d seconds...\n", thread_id, sleep_time);
    sleep(sleep_time);

    printf("Thread %d: waiting for other thread...\n", thread_id);

    // TODO: Add rendezvous code!
    if (thread_id == 0){
        sem_post(sem1);
        sem_wait(sem2);
    }
    else if (thread_id == 1)
    {
        sem_post(sem2);
        sem_wait(sem1);
    }

    printf("Thread %d: complete\n", thread_id);

    return NULL;
}

int main(void)
{
    int params[2][2] = {{0, 5}, {1, 3}};

    pthread_t t0, t1;

    // TODO: Add sem_open_temp() calls
    sem1 = sem_open_temp("sem1", 0);
    sem2 = sem_open_temp("sem2", 0);

    pthread_create(&t0, NULL, run, params[0]);
    pthread_create(&t1, NULL, run, params[1]);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    // TODO: Add sem_close() calls
    sem_close(sem1);
    sem_close(sem2);
}