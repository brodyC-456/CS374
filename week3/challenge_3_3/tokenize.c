#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    (void)argc; // stop compiler warnings about ununsed variables

    char *token;
    int i = 0;
    if ((token = strtok(argv[1], " ")) != NULL) do {
        printf("%d: %s\n", i, token);
        i++;
    } while((token = strtok(NULL, " ")) != NULL);
}