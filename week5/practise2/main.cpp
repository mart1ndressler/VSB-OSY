#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <cstdio>

using namespace std;

int main() 
{
    int pipe1[2], pipe2[2];
    if(pipe(pipe1) == -1 || pipe(pipe2) == -1) exit(1);

    if(fork() == 0) 
    {
        close(pipe1[0]);
        FILE *fd = fopen("names.txt", "r");

        char buffer[1024];
        while(fgets(buffer, sizeof(buffer), fd)) 
        {
            int len = strlen(buffer);
            if(buffer[len - 1] == '\n') len--;
            dprintf(pipe1[1], "%02d %s", len, buffer);
        }

        fclose(fd);
        close(pipe1[1]);
        exit(0);
    }
    if(fork() == 0) 
    {
        dup2(pipe1[0], 0);
        dup2(pipe2[1], 1);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execlp("sh", "sh", "-c", "sort | grep '05 A'", NULL);
        cerr << "Error: sort | grep" << endl;
        exit(1);
    }
    if(fork() == 0) 
    {
        dup2(pipe2[0], 0);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        char buffer[1024];
        int line_number = 1;
        while(fgets(buffer, sizeof(buffer), stdin)) printf("%d. %s", line_number++, buffer);
        exit(0);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}