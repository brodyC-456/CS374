# Brody's Consumer-Producer Multithreading with Semaphores

## Building

_[How to build the program. VS Code part is optional.]_

Command line:

* `make` to build. An executable called `pc` will be produced.
  
## Files

* `pc.c`: The main code to launch all consumer and producer threads
* `eventbuf.c`: Code that defines queue functionality for the event buffer

## Data

The main data structure is the event buffer, which is a queue full of events that producers enqueue to and consumers dequeue from.
There is two arrays of threads, one with all the consumer threads and one with all the producer threads.
There is also ab array of thread arguments for the producer threads.

## Functions

* `main()`
  * `usage()`: Prints a usage message
  * `sem_open_temp()`: Safely creates new semaphore
  * `run_prod`: Run code for producer threads, safely adds an event to the queue, but only if there aren't too many outstanding events
  * `run_cons`: Run code for consumer threads, safely dequeues an event to the queue, only if there is an outstanding event available, and ends when signaled.

* `^C` to quit.
* `life_init()` should allow different kinds of initializations, not
  just random.
* UI could be added so the user could draw patterns.
* Maybe add some preprogrammed patterns, spaceships, etc.
* Bug: display totally messed up on screens smaller than 80x24
