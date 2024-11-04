#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 3) exit(1);
    char *file = argv[1], *znak = argv[2];
    int pipe1[2], pipe2[2];

    if(pipe(pipe1) == -1 || pipe(pipe2) == -1) exit(1);

    if(fork() == 0) 
    {
        close(pipe1[0]);
        FILE *fd = fopen(file, "r");
        char buffer[1024];
        int n, total_bytes = 0;
        while((n = fread(buffer, 1, sizeof(buffer), fd)) > 0) {
            write(pipe1[1], buffer, n);
            total_bytes += n;
        }
        fclose(fd);
        close(pipe1[1]);
        fprintf(stderr, "Bytes transferred: %d\n", total_bytes);
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
        execlp("sh", "sh", "-c", ("sort | grep -i -v " + string(znak)).c_str(), NULL);
        fprintf(stderr, "Error: sort | grep\n");
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
        fprintf(stderr, "Error: nl\n");
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