#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main()
{
    int pipe1[2], pipe2[2];
    if(pipe(pipe1) == -1 || pipe(pipe2) == -1) exit(1);

    if(fork() == 0) 
    {
        dup2(pipe1[1], 1);
        close(pipe1[0]);
        close(pipe1[1]);
        execlp("sort", "sort", "names_short.txt", NULL);
        cerr << "Error: sort" << endl;
        exit(1);
    }
    if(fork() == 0) 
    {
        dup2(pipe1[0], 0);
        dup2(pipe2[1], 1);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execlp("tr", "tr", "[a-z]", "[A-Z]", NULL);
        cerr << "Error: tr" << endl;
        exit(1);
    }
    if(fork() == 0) 
    {
        dup2(pipe2[0], 0);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        execlp("nl", "nl", "-s", ". ", NULL);
        cerr << "Error: nl" << endl;
        exit(1);
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