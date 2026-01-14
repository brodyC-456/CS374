#include <stdio.h>
#include <dirent.h>

int main(void){
    DIR *dir = opendir("/");
    struct dirent *de;
    while ((de = readdir(dir)) != NULL){
        printf("%s\n", de->d_name);
    }
    closedir(dir);
}