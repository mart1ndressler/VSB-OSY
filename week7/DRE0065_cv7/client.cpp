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
            cout << buffer << "\n> ";
            fflush(stdout);
        } 
        else {cout << "Disconnected from server.\n";  break;}
    }
}

void display_sleeping_message() 
{
    while(1) 
    {
        sleep(15);
        cout << "sleeping...\n> ";
        fflush(stdout);
    }
}

int main(int argc, char* argv[]) 
{
    string nickname = argv[1];
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

    string nick_command = "#nick " + nickname;
    send(sock, nick_command.c_str(), nick_command.size(), 0);

    thread receive_thread(receive_messages, sock);
    receive_thread.detach();
    thread sleeping_thread(display_sleeping_message);
    sleeping_thread.detach();

    string message;
    while(1) 
    {
        cout << "> ";
        getline(cin, message);
        if(message == "#quit") 
        {
            send(sock, message.c_str(), message.size(), 0); 
            break;
        }
        send(sock, message.c_str(), message.size(), 0);
    }
    close(sock);
    return 0;
}