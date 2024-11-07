#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace std;

vector<pair<int, string>> clients;
int disconnect_pipe[2];
void list_clients(int client_socket) 
{
    string client_list = "Connected Clients:\n";
    for(int i=0; i < clients.size(); i++) client_list += "- " + clients[i].second + "\n";
    send(client_socket, client_list.c_str(), client_list.size(), 0);
}

void broadcast_message(string message, int sender_socket = -1) 
{
    for(int i=0; i < clients.size(); i++) send(clients[i].first, message.c_str(), message.size(), 0);
}

void handle_client(int client_socket) 
{
    char buffer[1024];
    string nickname;

    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if(bytes_read <= 0) 
    {
        close(client_socket);
        return;
    }

    buffer[bytes_read] = '\0';
    string command(buffer);
    if(command.find("#nick ") == 0) 
    {
        nickname = command.substr(6);
        clients.push_back({client_socket, nickname});
        cout << "Client " << nickname << " is connected.\n";
    }
    else {close(client_socket); return;}

    while((bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) 
    {
        buffer[bytes_read] = '\0';
        string message(buffer);

        if(message == "#list") list_clients(client_socket);
        else if(message == "#quit") break;
        else if(message[0] == '#') continue;
        else 
        {
            string formatted_message = nickname + ": " + message;
            broadcast_message(formatted_message, client_socket);
        }
    }

    cout << "Client " << nickname << " is disconnected.\n";
    write(disconnect_pipe[1], &client_socket, sizeof(client_socket));
    close(client_socket);
}

void monitor_disconnects() 
{
    int client_socket;
    while(read(disconnect_pipe[0], &client_socket, sizeof(client_socket)) > 0) 
    {
        for(int i=0; i < clients.size(); i++) 
        {
            if(clients[i].first == client_socket) 
            {
                clients.erase(clients.begin() + i);
                break;
            }
        }
    }
}

int main() 
{
    pipe(disconnect_pipe);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0), opt = 1;
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    ::bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    thread disconnect_thread(monitor_disconnects);
    disconnect_thread.detach();

    cout << "Server is running...\n";
    while(1) 
    {
        int client_socket = accept(server_fd, NULL, NULL);
        thread client_thread(handle_client, client_socket);
        client_thread.detach();
    }

    close(server_fd);
    return 0;
}