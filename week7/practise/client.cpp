#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <string>

using namespace std;

void receive_messages(int client_socket) 
{
    char buffer[1024];
    while(1) 
    {
        int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
        if(bytes_read > 0) 
        {
            buffer[bytes_read] = '\0';
            cout << "\nMessage from server: " << buffer << "\nEnter a mathematical expression: ";
            fflush(stdout);
        }
    }
}

int main() 
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    string expression;

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));
    
    thread receive_thread(receive_messages, sock);
    receive_thread.detach();
    while(1) 
    {
        cout << "\nEnter a mathematical expression: ";
        getline(cin, expression);
        expression += '\n';
        send(sock, expression.c_str(), expression.size(), 0);
    }

    close(sock);
    return 0;
}