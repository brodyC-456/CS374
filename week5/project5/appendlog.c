#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define HEADER_LEN (sizeof(int) * 2)
#define FILE_SIZE (sizeof(int) * 2 + 1048576 * 32)
#define RECORD_SIZE 32

// Globally Defined Locks
struct flock header_lock = {
    .l_type=F_WRLCK, 
    .l_whence=SEEK_SET,
    .l_start=0,
    .l_len= HEADER_LEN,
};

struct flock header_unlock = {
    .l_type=F_UNLCK,
    .l_whence=SEEK_SET,
    .l_start=0,
    .l_len=HEADER_LEN,
};

struct flock full_lock = {
    .l_type=F_RDLCK, 
    .l_whence=SEEK_SET,
    .l_start=0,
    .l_len=0,
};

struct flock full_unlock = {
    .l_type=F_UNLCK,
    .l_whence=SEEK_SET,
    .l_start=0,
    .l_len=0,
};

// Pair Structure for command line arguments
struct Pair {
    int count;
    char* prefix;
};

// Adds all the necessary logs to the file, forking a process per command line pair
void append_log(int fd, int num_of_pairs, struct Pair pairs[], void *data){
    // Fork Code -- Super Long :)
    for(int i = 0; i < num_of_pairs; i++){
        pid_t pid = fork();
        // Child
        if (pid == 0) {
            for (int k = 0; k < pairs[i].count; k++){
                // Header
                fcntl(fd, F_SETLKW, &header_lock);
                // Get the offset FROM THE START OF THE FIRST RECORD and the count
                int *offset_loc = (int *)data;
                int *count_loc = offset_loc+1;
                int offset = *offset_loc + HEADER_LEN;
                // Count is second int, increment it
                (*count_loc)++;
                // Change the new official offset to the next record size
                *offset_loc += RECORD_SIZE;
                fcntl(fd, F_SETLK, &header_unlock);
                
                // New locks for where the record will be written
                struct flock record_lock = {
                    .l_type=F_WRLCK, 
                    .l_whence=SEEK_SET,
                    .l_start= offset,
                    .l_len=RECORD_SIZE,
                };
                struct flock record_unlock = {
                    .l_type=F_UNLCK, 
                    .l_whence=SEEK_SET,
                    .l_start=offset,
                    .l_len= RECORD_SIZE,
                };

                // Record Lock and Adding the data to the file
                fcntl(fd, F_SETLKW, &record_lock);
                char *loc = (char*)data + offset;
                snprintf(loc, RECORD_SIZE, "%s %d", pairs[i].prefix, k);
                fcntl(fd, F_SETLK, &record_unlock); 
            }
        _exit(0);
        }
    }

    // Parent waits for all child processes to run
    for (int i = 0; i < num_of_pairs; i++) {
        wait(NULL);
    }
        
}

// Prints the data from every log entry (within the count)
void dump_log(int fd, void *data){
    fcntl(fd, F_SETLKW, &full_lock);

    // Store the count so we know how long to loop!
    int *count_loc = (int *)data + 1;
    int c = *count_loc;

    int offset = HEADER_LEN;

    for (int i = 0; i < c; i++) {
        // Find the location of the log, print it, then update the offset
        char *loc = (char *)data + offset;
        printf("%d: %s\n", i, loc);
        offset += RECORD_SIZE;
    }
    fcntl(fd, F_SETLK, &full_unlock);
}

void usage(){
    fprintf(stderr, "usage: appendlog [count, prefix]");
    exit(1);
}

int main(int argc, char* argv[]){

    // Setting up the data file
    int fd = open("log.dat", O_RDWR | O_CREAT | O_TRUNC, 0666);
    ftruncate(fd, FILE_SIZE);
    void *data = mmap(NULL, FILE_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED){
        perror("mmap");
        exit(1);
    }

    // Parse Command Line
     if ((argc - 1) % 2 == 1 || argc < 3){
        usage();
    }
    int num_of_pairs = (argc - 1) / 2;
    // I used a pair structure because it was the only thing i could think of :)
    struct Pair pairs[num_of_pairs];
    for (int i = 1, j = 0; i < argc; i += 2, j++){
        pairs[j].count = atoi(argv[i]);
        pairs[j].prefix = argv[i+1];
    }

    int *offset_loc = (int *)data;
    int *count_loc = offset_loc + 1;
    *offset_loc = 0;
    *count_loc = 0;


    // Add the records then print out the data
    append_log(fd, num_of_pairs, pairs, data);
    dump_log(fd, data);

    // Unmap the memory and close the file!
    munmap(data, FILE_SIZE);
    close(fd);
}