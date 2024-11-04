#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <errno.h>

using namespace std;

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

void handle_client(int client_socket) 
{
    char buffer[1024];
    while(int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1)) 
    {
        buffer[bytes_read] = '\0';
        string expression(buffer);

        if(expression.back() == '\n') 
        {
            expression.pop_back();
            int result = calculate_expression(expression);
            string response = expression + " = " + to_string(result) + "\n";
            write(client_socket, response.c_str(), response.size());
        } 
        else 
        {
            string error_message = "Expression must end with '\\n'\n";
            write(client_socket, error_message.c_str(), error_message.size());
        }
    }

    close(client_socket);
    cout << "Client disconnected.\n";
}

int main() 
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
    {
        cerr << "Unable to set socket option: " << strerror(errno) << endl;
        close(server_fd);
        exit(1);
    }
    if(::bind(server_fd, (sockaddr*)&address, sizeof(address)) == -1 || listen(server_fd, 3) == -1) 
    {
        cerr << "Server setup failed: " << strerror(errno) << endl;
        close(server_fd);
        exit(1);
    }
    cout << "Server is running...\n";

    while(1) 
    {
        int client_socket = accept(server_fd, nullptr, nullptr);
        cout << "New client connected.\n";

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