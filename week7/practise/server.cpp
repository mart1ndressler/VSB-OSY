#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <string>

using namespace std;

vector<pair<int, int>> client_sockets;
int client_id_counter = 1;

int calculate_expression(string expression) 
{
    int result = 0, term = 0, index = 0;
    char op = '+';

    while(index < expression.size()) 
    {   
        term = 0;
        if(expression[index] == '-' || expression[index] == '+') op = expression[index++];
        while(index < expression.size() && expression[index] >= '0' && expression[index] <= '9') 
        {
            term = term * 10 + (expression[index++] - '0');
        }
        result += (op == '+') ? term : -term;
        if(index < expression.size()) op = expression[index++];
    }
    return result;
}

void broadcast_message(string message) 
{
    for(auto client : client_sockets) send(client.first, message.c_str(), message.size(), 0);
}

void handle_client(int client_socket) 
{
    int client_id = client_id_counter++;
    client_sockets.push_back({client_socket, client_id});

    cout << "New client connected with ID [" << client_id << "].\n";

    char buffer[1024];
    while(int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) 
    {
        buffer[bytes_read] = '\0';
        string expression(buffer);

        if(expression.back() == '\n') 
        {
            expression.pop_back();
            int result = calculate_expression(expression);
            string response = "Broadcast from Client [" + to_string(client_id) + "]: " + expression + " = " + to_string(result) + "\n";
            broadcast_message(response);
        }
    }

    for(auto it = client_sockets.begin(); it != client_sockets.end(); it++)
    if(it->first == client_socket) {client_sockets.erase(it); break;}
    close(client_socket);
    cout << "Client [" << client_id << "] disconnected.\n";
}

int main() 
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0), opt=1;
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        cerr << "Unable to set socket option: " << strerror(errno) << endl;
        close(server_fd);
        exit(1);
    }
    if(::bind(server_fd, (sockaddr*)&address, sizeof(address)) == -1 || listen(server_fd, 10) == -1) 
    {
        cerr << "Server setup failed: " << strerror(errno) << endl;
        close(server_fd);
        exit(1);
    }

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