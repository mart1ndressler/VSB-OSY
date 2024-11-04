#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <ctime>

using namespace std;

void generate_numbers(int read_fd, int write_fd)
{
    char buf[128];
    int N, Speed, row = 0;
    read(read_fd, buf, sizeof(buf));
    buf[strlen(buf)] = '\0';
    sscanf(buf, "%d %d", &N, &Speed);

    while(row < N)
    {
        int num1 = rand() % 20 + 1;
        snprintf(buf, sizeof(buf), "%d. %d\n", row + 1, num1);
        write(write_fd, buf, strlen(buf));
        cout << "[1. Child] Generated: " << buf;
        usleep(1000000 / Speed);
        row++;
    }

    write(write_fd, "", 1);
    close(read_fd);
    close(write_fd);
}

void add_operation(int read_fd, int write_fd)
{
    char buf[128], final_buf[128];
    int row, num1;
    char ops[] = {'+', '-', '*', '/'};

    while(1)
    {
        int ret = read(read_fd, buf, sizeof(buf)), num2 = rand() % 20 + 1;
        char op = ops[rand() % 4];
        if(ret <= 0 || buf[0] == '\0') break;
        buf[ret] = '\0';
        buf[strcspn(buf, "\n")] = '\0';

        sscanf(buf, "%d. %d", &row, &num1);
        snprintf(final_buf, sizeof(final_buf), "%s%c%d\n", buf, op, num2);
        cout << "[2. Child] Operation: " << final_buf;
        write(write_fd, final_buf, strlen(final_buf));
    }

    write(write_fd, "", 1);
    close(read_fd);
    close(write_fd);
}

void calculate_result(int read_fd)
{
    char buf[128];
    int row, num1, num2, result;
    char op;

    while(1)
    {
        int ret = read(read_fd, buf, sizeof(buf));
        if(ret <= 0 || buf[0] == '\0') break;
        buf[ret] = '\0';
        buf[strcspn(buf, "\n")] = '\0';

        sscanf(buf, "%d. %d%c%d", &row, &num1, &op, &num2);
        switch(op)
        {
            case '+': result = num1 + num2; break;
            case '-': result = num1 - num2; break;
            case '*': result = num1 * num2; break;
            case '/': result = num1 / num2; break;
        }
        
        snprintf(buf, sizeof(buf), "%s=%d\n", buf, result);
        cout << "[3. Child] Result: " << buf;
    }

    close(read_fd);
}

int main(int argc, char *argv[])
{
    int N = atoi(argv[1]), Speed = atoi(argv[2]), pipe_fd1[2], pipe_fd2[2], pipe_fd3[2];
    char buf[128];

    srand(time(NULL));
    pipe(pipe_fd1);
    pipe(pipe_fd2);
    pipe(pipe_fd3);
    
    if(fork() == 0)
    {
        close(pipe_fd1[1]);
        close(pipe_fd2[0]);
        generate_numbers(pipe_fd1[0], pipe_fd2[1]);
        exit(1);
    }
    if(fork() == 0)
    {   
        close(pipe_fd2[1]);
        close(pipe_fd3[0]);
        add_operation(pipe_fd2[0], pipe_fd3[1]);
        exit(1);
    }
    if(fork() == 0)
    {
        close(pipe_fd3[1]);
        calculate_result(pipe_fd3[0]);
        exit(1);
    }

    close(pipe_fd1[0]);
    close(pipe_fd2[0]);
    close(pipe_fd2[1]);
    close(pipe_fd3[0]);
    close(pipe_fd3[1]);
    snprintf(buf, sizeof(buf), "%d %d\n", N, Speed);
    write(pipe_fd1[1], buf, strlen(buf));
    close(pipe_fd1[1]);

    wait(NULL);
    wait(NULL);
    wait(NULL); 
    return 0;
}