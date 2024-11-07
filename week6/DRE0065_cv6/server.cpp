#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/wait.h>

using namespace std;

void send_file(int client_socket, string filename) 
{
    FILE* file = fopen(filename.c_str(), "rb");
    char buffer[1024];
    int bytes_read;
    while((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) 
    {
        send(client_socket, buffer, bytes_read, 0);
        usleep(450000);
    }
    fclose(file);
}

void handle_client(int client_socket) 
{
    char buffer[1024];
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if(bytes_read > 0) 
    {
        buffer[bytes_read] = '\0';
        string animal(buffer), out = to_string(getpid()) + ".srv";
        animal.pop_back();

        if(fork() == 0) 
        {
            execlp("g++", "g++", "hello.cpp", ("-D" + animal).c_str(), "-o", out.c_str(), NULL);
            exit(1);
        }
        wait(NULL);
        send_file(client_socket, out);
    }
    close(client_socket);
    cout << "Client disconnected!\n";
}

int main() 
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0), opt = 1;
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    cout << "Server is running...\n";
    while(1) 
    {
        int client_socket = accept(server_fd, nullptr, nullptr);
        cout << "New client connected!\n";
        if(fork() == 0) 
        {
            close(server_fd);
            handle_client(client_socket);
            exit(0);
        } 
        else close(client_socket);
    }
    close(server_fd);
    return 0;
}