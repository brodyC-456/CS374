#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{

    printf("prompt> ");
    fflush(stdout);  // flush output since there's no \n after prompt

    char line[1024];
    fgets(line, sizeof line, stdin);


    char *token;
    char *tokens[128] = {0};
    int j = 0;
    if ((token = strtok(line, " \n")) != NULL) do {
        tokens[j] = token;
        j++;
    } while((token = strtok(NULL, " \n")) != NULL);

    execvp(tokens[0], tokens);



        
}