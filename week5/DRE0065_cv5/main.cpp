#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 3) exit(1);
    char *file = argv[1], *znak = argv[2];

    int pipe1[2], pipe2[2], pipe3[2], bytes, row=1;
    if(pipe(pipe1) == -1 || pipe(pipe2) == -1 || pipe(pipe3) == -1) exit(1);

    if(fork() == 0) //sort
    {
        dup2(pipe1[1], 1);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        close(pipe3[0]);
        close(pipe3[1]);
        execlp("sort", "sort", "names.txt", NULL);
        cerr << "Error: sort" << endl;
        exit(1);
    }
    if(fork() == 0) //tr
    {
        dup2(pipe1[0], 0);
        dup2(pipe2[1], 1);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        close(pipe3[0]);
        close(pipe3[1]);
        execlp("tr", "tr", "[a-z]", "[A-Z]", NULL);
        cerr << "Error: tr" << endl;
        exit(1);
    }
    if(fork() == 0) //grep
    {
        dup2(pipe2[0], 0);
        dup2(pipe3[1], 1);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        close(pipe3[0]);
        close(pipe3[1]);

        char buff[1024];
        while(fgets(buff, sizeof(buff), stdin))
        {
            if(strstr(buff, znak) == buff) cout << row++ << ". " << buff;
        }
        exit(1);
    }
    if(fork() == 0) //tee
    {
        dup2(pipe3[0], 0);
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);
        close(pipe3[0]);
        close(pipe3[1]);

        FILE *out_file = fopen(file, "w");
        char buff[1024];
        while((bytes = read(0, buff, sizeof(buff))) > 0)
        {
            fwrite(buff,1,bytes, out_file);
            write(1, buff, bytes);
        }
        fclose(out_file);
        exit(1);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);
    close(pipe3[0]);
    close(pipe3[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}