#include <stdio.h>

int main(void){
    int x = 12;
    float y = 3.14;
    char* s = "Hello World";
    
    printf("x is %d, y is %f\n", x, y);
    printf("%s\n", s);
    
    for (int i = 0; i < 5; i++){
        int product = i * 5;
        printf("%d x 5 = %i\n", i, product);
    }
}