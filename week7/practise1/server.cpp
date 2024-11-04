#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <string>

using namespace std;

vector<pair<int, string>> clients;
void list_clients(int client_socket) 
{
    string client_list = "Connected Clients:\n";
    for(int i=0; i < clients.size(); i++) client_list += "- " + clients[i].second + "\n";
    send(client_socket, client_list.c_str(), client_list.size(), 0);
}

void send_private_message(string target_nick, string message, int sender_socket) 
{
    string sender_nick;
    for(int i=0; i < clients.size(); i++) 
    {
        if(clients[i].first == sender_socket) 
        {
            sender_nick = clients[i].second;
            break;
        }
    }

    for(int i=0; i < clients.size(); i++) 
    {
        if(clients[i].second == target_nick) 
        {
            string private_msg = "Private Message From " + sender_nick + ": " + message + "\n";
            send(clients[i].first, private_msg.c_str(), private_msg.size(), 0);
            return;
        }
    }

    string error_msg = "User Not Found!\n";
    send(sender_socket, error_msg.c_str(), error_msg.size(), 0);
}

void handle_client(int client_socket) 
{
    char buffer[1024];
    string nickname;

    send(client_socket, "Enter Your Nickname: ", 21, 0);
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    if(bytes_read <= 0) {close(client_socket); return;}
    buffer[bytes_read] = '\0';
    nickname = buffer;

    nickname.erase(remove(nickname.begin(), nickname.end(), '\n'), nickname.end());
    clients.push_back({client_socket, nickname});

    cout << "Client " << nickname << " connected!\n";
    while((bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) > 0) 
    {
        buffer[bytes_read] = '\0';
        string message(buffer);
        message.erase(remove(message.begin(), message.end(), '\n'), message.end());

        if(message == "#list") list_clients(client_socket);
        else if(message[0] == '#' && message.find(" ") != -1) 
        {
            int space_pos = message.find(" ");
            string target_nick = message.substr(1, space_pos - 1);
            string private_msg = message.substr(space_pos + 1);
            send_private_message(target_nick, private_msg, client_socket);
        } 
        else if(message == "#close" || message == "#quit") break;
    }

    for(int i=0; i < clients.size(); i++) 
    {
        if(clients[i].first == client_socket) 
        {
            clients.erase(clients.begin() + i);
            break;
        }
    }
    close(client_socket);
    cout << "Client " << nickname << " disconnected!\n";
}

int main() 
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0), opt = 1;
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(::bind(server_fd, (sockaddr*)&address, sizeof(address)) == -1) 
    {
        cerr << "Binding failed: " << strerror(errno) << endl;
        close(server_fd);
        return -1;
    }
    listen(server_fd, 10);

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