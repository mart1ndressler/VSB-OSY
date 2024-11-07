#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <poll.h>

using namespace std;

vector<pair<int, string>> clients;
int arrival_pipe[2];
int disconnect_pipe[2];

void list_clients(int client_socket) 
{
    string client_list = "Connected Clients:\n";
    for(auto client : clients) {client_list += "- " + client.second + "\n";}
    send(client_socket, client_list.c_str(), client_list.size(), 0);
}

void broadcast_message(string message) 
{
    for(auto client : clients) send(client.first, message.c_str(), message.size(), 0);
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
        if(nickname.empty()) 
        {
            close(client_socket);
            return;
        }
        write(arrival_pipe[1], &client_socket, sizeof(client_socket));
        write(arrival_pipe[1], nickname.c_str(), nickname.size() + 1);
        cout << "Client " << nickname << " is connected.\n";
    } 
    else {close(client_socket); return;}

    while((bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) 
    {
        buffer[bytes_read] = '\0';
        string message(buffer);

        if(message == "#list") list_clients(client_socket);
        else if(message == "#quit") break;
        else 
        {
            string formatted_message = nickname + ": " + message;
            broadcast_message(formatted_message);
        }
    }

    cout << "Client " << nickname << " is disconnected.\n";
    write(disconnect_pipe[1], &client_socket, sizeof(client_socket));
    close(client_socket);
}

void prichod_odchod() 
{
    int client_socket;
    char nickname[1024];
    
    struct pollfd fds[2];
    fds[0].fd = arrival_pipe[0];
    fds[0].events = POLLIN;
    fds[1].fd = disconnect_pipe[0];
    fds[1].events = POLLIN;

    while(1) 
    {
        int poll_count = poll(fds, 2, -1);
        if(fds[0].revents & POLLIN) 
        {
            if(read(arrival_pipe[0], &client_socket, sizeof(client_socket)) > 0) 
            {
                read(arrival_pipe[0], nickname, sizeof(nickname));
                clients.push_back({client_socket, string(nickname)});
            }
        }

        if(fds[1].revents & POLLIN) 
        {
            if(read(disconnect_pipe[0], &client_socket, sizeof(client_socket)) > 0) 
            {
                for(auto it = clients.begin(); it != clients.end(); it++) 
                {
                    if(it->first == client_socket) {clients.erase(it); break;}
                }
            }
        }
    }
}

int main() 
{
    if(pipe(arrival_pipe) == -1 || pipe(disconnect_pipe) == -1) 
    {
        cerr << "Failed to create pipes.\n";
        return 1;
    }
    
    int server_fd = socket(AF_INET, SOCK_STREAM, 0), opt = 1;
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    ::bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    thread prichod_odchod_thread(prichod_odchod);
    prichod_odchod_thread.detach();

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