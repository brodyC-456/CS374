#include <stdio.h>
#include <string.h>

int main()
{

    char line[1024];
    fgets(line, sizeof line, stdin);


    char *token;
    int i = 0;
    if ((token = strtok(line, " \n")) != NULL) do {
        printf("%d: %s\n", i, token);
        i++;
    } while((token = strtok(NULL, " \n")) != NULL);
    
}