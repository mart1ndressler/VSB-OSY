#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) 
{
    int sum=0, binary=0, x;
    if(argc > 1) binary = strcmp(argv[1], "-b") == 0 ? 1 : 0;

    if(binary == 1) 
    {
        while(fread(&x, sizeof(int), 1, stdin)) sum = sum + x;
    }
    else while(scanf("%d", &x) != EOF) sum = sum + x;

    printf("%d\n", sum);
    return 0;
}