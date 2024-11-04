#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) 
{
    int min=0, max=1000, count=0, binary=0;
    srand(time(NULL));

    if(argc < 2) 
    {
        cout << "Wrong number of arguments!";
        return -1;
    }
    
    for(int i=0; i < argc; i++)
    {
        if(strcmp(argv[i],"-b")==0) binary=1;
        else if(count==0) count = atoi(argv[i]);
        else if(min==0 && argc > 3 && binary==0) min = atoi(argv[i]);
        else if(max==1000 && argc > 3 && binary==0) max = atoi(argv[i]);
    }

    if(count <= 0)
    {
        cout << "Wrong count of numbers!";
        return -1;
    }
   
    for(int i=0; i<count; i++)
    {
        int x = rand()% (max-min+1)+min;
        binary==1 ? fwrite(&x, sizeof(int), 1, stdout) : printf("%d\n", x);
    }

    return 0;
}