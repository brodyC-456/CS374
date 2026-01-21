#include <stdio.h>
#include <string.h>

int main()
{

    char line[1024];
    fgets(line, sizeof line, stdin);


    char *token;
    char *tokens[128] = {0};
    int j = 0;
    if ((token = strtok(line, " \n")) != NULL) do {
        tokens[j] = token;
        j++;
    } while((token = strtok(NULL, " \n")) != NULL);

    // Prints out the tokens
    for (int i = 0; tokens[i] != NULL; i++)
        printf("%d: %s\n", i, tokens[i]);
}