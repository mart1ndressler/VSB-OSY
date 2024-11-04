#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <ctime>

using namespace std;

void generate_numbers(int pipe_fd, int N)
{
    int row = 0;
    char buf[128];

    while(N == -1 || row < N)
    {
        int num1 = rand() % 20 + 1;
        snprintf(buf, sizeof(buf), "%d. %d\n", row + 1, num1);
        write(pipe_fd, buf, strlen(buf));
        usleep(100000);
        row++;
    }
    write(pipe_fd, "", 1);
    close(pipe_fd);
}

void process_numbers(int read_fd, int write_fd)
{
    char buf[128];
    int row, num1, num2;
    char op, ops[] = {'+', '-', '*', '/'};

    while(1)
    {
        int ret = read(read_fd, buf, sizeof(buf));
        if(ret <= 0 || buf[0] == '\0') break;
        buf[ret] = '\0';

        sscanf(buf, "%d. %d", &row, &num1);
        op = ops[rand() % 4];
        num2 = rand() % 20 + 1;

        snprintf(buf, sizeof(buf), "%d. %d%c%d\n", row, num1, op, num2);
        cout << "PID " << getpid() << " Processed: " << buf;
        write(write_fd, buf, strlen(buf));
    }

    write(write_fd, "", 1);
    close(read_fd);
    close(write_fd);
}

void result(int read_fd, int write_fd)
{
    char buf[128], final_buf[128];
    int row, num1, num2, result;
    char op;

    while(1)
    {
        int ret = read(read_fd, buf, sizeof(buf));
        if(ret <= 0 || buf[0] == '\0') break;
        buf[ret] = '\0';

        sscanf(buf, "%d. %d%c%d", &row, &num1, &op, &num2);
        switch(op)
        {
            case '+': result = num1 + num2; break;
            case '-': result = num1 - num2; break;
            case '*': result = num1 * num2; break;
            case '/': result = num1 / num2; break;
        }
        snprintf(final_buf, sizeof(final_buf), "%d. %d%c%d=%d\n", row, num1, op, num2, result);
        cout << "PID " << getpid() << " Finalized: " << final_buf;
        write(write_fd, final_buf, strlen(final_buf));
    }

    write(write_fd, "", 1);
    close(read_fd);
    close(write_fd);
}

int main(int argc, char *argv[])
{
    int N = -1, pipe_fd1[2], pipe_fd2[2], pipe_fd3[2];
    if(argc > 1) N = atoi(argv[1]);

    srand(time(NULL));
    pipe(pipe_fd1);
    pipe(pipe_fd2);
    pipe(pipe_fd3);

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
        process_numbers(pipe_fd1[0], pipe_fd2[1]);
        exit(0);
    }
    if(fork() == 0)
    {
        close(pipe_fd2[1]);
        close(pipe_fd3[0]);
        result(pipe_fd2[0], pipe_fd3[1]);
        exit(0);
    }

    close(pipe_fd1[0]);
    close(pipe_fd1[1]);
    close(pipe_fd2[0]);
    close(pipe_fd2[1]);
    close(pipe_fd3[1]);

    char buf[128];
    while(1)
    {
        int ret = read(pipe_fd3[0], buf, sizeof(buf));
        if(ret <= 0 || buf[0] == '\0') break;
        buf[ret] = '\0';
        cout << "Parent received: " << buf;
    }

    close(pipe_fd3[0]);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}