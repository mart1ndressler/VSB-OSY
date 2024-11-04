#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <ctime>

using namespace std;

void generate_numbers(int pipe_fd, int N)
{
    int count = 0;
    char buf[128];

    while(N == -1 || count < N)
    {
        int num1 = rand()%100, num2 = rand()%100;
        snprintf(buf, sizeof(buf), "%d %d\n", num1, num2);

        write(pipe_fd, buf, strlen(buf));
        cout << "PID " << getpid() << " Generated: " << num1 << " " << num2 << endl;

        usleep(100000);
        count++;
    }

    write(pipe_fd, "", 1);
    close(pipe_fd);
}

void sum_numbers(int read_fd, int write_fd)
{
    char buf[128];

    while(1)
    {
        int ret = read(read_fd, buf, sizeof(buf)), num1, num2;
        if(ret <= 0 || buf[0] == '\0') break;
        buf[ret] = '\0';
        sscanf(buf, "%d %d", &num1, &num2);

        snprintf(buf, sizeof(buf), "%d\n", num1 + num2);
        write(write_fd, buf, strlen(buf));

        cout << "PID " << getpid() << " Summed: " << num1 << " + " << num2 << endl;
    }

    write(write_fd, "", 1);
    close(read_fd);
    close(write_fd);
}

int main(int argc, char *argv[])
{
    int N = -1, pipe_fd1[2], pipe_fd2[2];
    if(argc > 1) N = atoi(argv[1]);

    srand(time(NULL));
    pipe(pipe_fd1);
    pipe(pipe_fd2);

    if(fork() == 0)
    {
        close(pipe_fd1[0]);
        generate_numbers(pipe_fd1[1], N);
        exit(0);
    }
    if(fork() == 0)
    {
        close(pipe_fd1[1]);
        close(pipe_fd2[0]);
        sum_numbers(pipe_fd1[0], pipe_fd2[1]);
        exit(0);
    }
    if(fork() == 0)
    {
        close(pipe_fd2[1]);
        char buf[128];
        while(1)
        {
            int ret = read(pipe_fd2[0], buf, sizeof(buf));
            if(ret <= 0 || buf[0] == '\0') break;
            buf[ret] = '\0';
            cout << "PID " << getpid() << " Result: " << buf;
        }
        close(pipe_fd2[0]);
        exit(0);
    }

    close(pipe_fd1[0]);
    close(pipe_fd1[1]);
    close(pipe_fd2[0]);
    close(pipe_fd2[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}