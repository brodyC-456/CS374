#include <stdio.h>   
#include <fcntl.h>   
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

int main(int argc, char *argv[]){
    int fd = 0;
    char buf[2048];
    if(argc == 1){
        while (1){
            int bytes_read = read(fd, buf, 2048);
            for (int i = 0; i < buf[i] != '\0'; i++){
                buf[i] = (char)toupper((unsigned char)buf[i]);
            } 
            write(1, buf, bytes_read);
        }

    }
    else{

        for (int i = 1; i < argc; i++){
            bool finished = false;
            fd = open(argv[i], 0600);
            while(finished == false){
                int bytes_read = read(fd, buf, 2048);
                if(bytes_read == 0 || bytes_read == -1){
                    finished = true;
                }
                for (int i = 0; buf[i] != '\0'; i++){
                    buf[i] = (char)toupper((unsigned char)buf[i]);
                }
                write(1, buf, bytes_read);
            }
            write(1, "\n\n", 3);
            close(fd);
        }
    }
}