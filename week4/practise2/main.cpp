#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <ctime>

using namespace std;

void add_operation(int read_fd, int write_fd)
{
    char buf[128];
    int row, num1;
    char ops[] = {'+', '-', '*', '/'};

    while(1)
    {
        int ret = read(read_fd, buf, sizeof(buf)), num2 = rand() % 20 + 1;
        char op = ops[rand() % 4];
        if(ret <= 0 || buf[0] == '\0') break;
        buf[ret] = '\0';

        sscanf(buf, "%d. %d", &row, &num1);
        snprintf(buf, sizeof(buf), "%d. %d%c%d\n", row, num1, op, num2);
        cout << "[1. Child] PID " << getpid() << " -> OP and SNDnum: " << buf;
        write(write_fd, buf, strlen(buf));
    }

    write(write_fd, "", 1);
    close(read_fd);
    close(write_fd);
}

void calculate_result(int read_fd, int write_fd)
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
        cout << "[2. Child] PID " << getpid() << " -> Calculate: " << final_buf;
        write(write_fd, final_buf, strlen(final_buf));
    }

    write(write_fd, "", 1);
    close(read_fd);
    close(write_fd);
}

void print_data(int read_fd)
{
    char buf[128];
    while(1)
    {
        int ret = read(read_fd, buf, sizeof(buf));
        if(ret <= 0 || buf[0] == '\0') break;
        buf[ret] = '\0';
        cout << "[3. Child] PID " << getpid() << " -> Result: " << buf;
    }
    close(read_fd);
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
        close(pipe_fd1[1]);
        close(pipe_fd2[0]);
        add_operation(pipe_fd1[0], pipe_fd2[1]);
        exit(0);
    }
    if(fork() == 0)
    {
        close(pipe_fd2[1]);
        close(pipe_fd3[0]);
        calculate_result(pipe_fd2[0], pipe_fd3[1]);
        exit(0);
    }
    if(fork() == 0)
    {
        close(pipe_fd3[1]);
        print_data(pipe_fd3[0]);
        exit(0);
    }

    close(pipe_fd1[0]);
    for(int i = 0; i < N; i++)
    {
        char buf[128];
        int num1 = rand() % 20 + 1;
        snprintf(buf, sizeof(buf), "%d. %d\n", i + 1, num1);
        cout << "[Parent] Generated: " << buf;
        write(pipe_fd1[1], buf, strlen(buf));
        usleep(100000);
    }

    write(pipe_fd1[1], "", 1);
    close(pipe_fd1[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}