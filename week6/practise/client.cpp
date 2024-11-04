#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <ctime>
#include <sys/select.h>

using namespace std;

int main() 
{
    srand(time(NULL));
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[1024];

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)); 

    cout << "Enter a mathematical expression: ";
    fflush(stdout);

    fd_set readfds;
    timeval timeout = {15, 0};
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);

    string expression;
    if(select(1, &readfds, NULL, NULL, &timeout) == 0) 
    {
        expression = to_string(rand() % 101) + ((rand() % 2) ? "+" : "-") + to_string(rand() % 101) + '\n';
        cout << "\nSending random expression: " << expression;
    } 
    else 
    {
        getline(cin, expression);
        expression += '\n';
    }

    send(sock, expression.c_str(), expression.size(), 0);
    read(sock, buffer, 1024);
    cout << "Response from server: " << buffer << "\n";
    close(sock);
    return 0;
}