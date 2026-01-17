#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Dealing with some magic numbers
#define ERROR_CODE 0
#define PATH_ARG_NUM 1
#define PATH_BUF_SIZE PATH_MAX + 1

// Returns the stat struct we use for type and byte size
struct stat get_lstat(char* dir_path){
    struct stat buf;
    lstat(dir_path, &buf);
    return buf;
}

// Handles Permission Errors when opening directories
int file_read_error(char* pathname){
    fprintf(stderr, "%s: failed to open\n", pathname);
    return ERROR_CODE;
}

// Returns the size of a directory using its pathname
int count_bytes(char* dir_path){

    // Open Directory and Set byte_len
    DIR *dir = opendir(dir_path);
    if (dir == NULL){
        return file_read_error(dir_path);
    }
    struct dirent *de;
    int byte_len = 0;

    // Main Loop, reads directory entries until it reaches the end of the directory (NULL)
    while ((de = readdir(dir)) != NULL){
        // Sets the specific file path we are opening (used in get_lstat and the recursive call)
        char new_path[PATH_BUF_SIZE];
        snprintf(new_path, sizeof(new_path), "%s/%s", dir_path, de->d_name);
        struct stat buf = get_lstat(new_path);

        // Use st_mode macros to determine file type
        switch (buf.st_mode & S_IFMT){
            // Add size of regular files to the total
            case S_IFREG:
                byte_len += buf.st_size;
                break;
            // Recurse into other directories, adding their size to the total
            case S_IFDIR:
                if(strcmp(de->d_name, ".") && strcmp(de->d_name, "..")){
                    byte_len += count_bytes(new_path);
                    break;
                }
                else{
                    // Ignore .. and .
                    break;
                }
            // Ignore all other file types
            default:
                break;
        }
    }
    // Close the directory and print out the total byte_length
    closedir(dir);
    printf("%s: %d\n", dir_path, byte_len);
    return byte_len;
}

void usage(void)
{
    fprintf(stderr, "usage: filescanner [directory]\n");
    exit(1);
}

int main(int argc, char *argv[]){
    char* dir_path;
    if (argc == 1){
        dir_path = ".";
    }
    else if (argc == 2)
    {
        dir_path = argv[PATH_ARG_NUM];
    }
    else {
        usage();
    }

    count_bytes(dir_path);
}


