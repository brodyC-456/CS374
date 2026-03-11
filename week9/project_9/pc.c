#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

#include "eventbuf.h"


struct ThreadArgs {
    int thread_id;
    int num_events;
};

struct eventbuf *buf;
sem_t *spaces, *items, *mutex;
int consumers_done = 0;


void usage(){
    fprintf(stderr, "usage: pc num_producers num_consumers num_events max_events\n");
    exit(1);
}

sem_t *sem_open_temp(const char *name, int value)
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

// Run functions for threads
void *run_prod(void *arg)
{
    struct ThreadArgs *args = (struct ThreadArgs *)arg;

    for (int i = 0; i < args->num_events; i++){
        int event_num = (args->thread_id * 100) + i;
        sem_wait(spaces);
        sem_wait(mutex);
        eventbuf_add(buf, event_num);
        printf("P%i: adding event %i\n", args->thread_id, event_num);
        sem_post(mutex);
        sem_post(items);
    }

    printf("P%i: Exiting\n", args->thread_id);
    return NULL;
}

void *run_cons(void *arg)
{
    int *id = (int *)arg;

    while(1){
        sem_wait(items);
        sem_wait(mutex);
        // Recieve the signal, but keep going if there's still events in the buffer
        if (consumers_done == 1 && eventbuf_empty(buf) == 1){
            sem_post(mutex);
            break;
        }
        int event_num = eventbuf_get(buf);
        printf("C%i: got event %i\n", *id, event_num);
        sem_post(mutex);
        sem_post(spaces);
    }

    printf("C%i: Exiting\n", *id);
    return NULL;
}

int main(int argc, char *argv[]){
    if (argc != 5){
        usage();
    }

    // Parse Command Line
    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);
    int num_events = atoi(argv[3]);
    int max_events = atoi(argv[4]);
    
    // Initializing variables and creating semaphores
    buf = eventbuf_create();
    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

    struct ThreadArgs prod_args[num_producers];
    
    spaces = sem_open_temp("max_events", max_events);
    mutex = sem_open_temp("mutex", 1);
    items = sem_open_temp("events", 0);

    // Create Producer Threads
    for (int i = 0; i < num_producers; i++){
        prod_args[i].thread_id = i;
        prod_args[i].num_events = num_events;
        pthread_create(&producers[i], NULL, run_prod, &prod_args[i]);
    }

    int ids[num_consumers];
    // Create Consumer Threads
    for (int i = 0; i < num_consumers; i++){
        ids[i] = i;
        pthread_create(&consumers[i], NULL, run_cons, &ids[i]);
    }

    // Wait for producers to finish
    for (int i = 0; i < num_producers; i++){
        pthread_join(producers[i], NULL);
    }
    
    // Notify Consumers that they are done
    consumers_done = 1;

    // Need to wake them up so there's no deadlocks
    for (int i = 0; i < num_consumers; i++) {
        sem_post(items);
    }

    // Wait for all consumers to finish
    for (int i = 0; i < num_consumers; i++){
        pthread_join(consumers[i], NULL);
    }

    eventbuf_free(buf);
}