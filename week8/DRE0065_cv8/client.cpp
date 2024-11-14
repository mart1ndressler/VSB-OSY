#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/wait.h>

using namespace std;

int main(int argc, char *argv[]) 
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

    string request = "#img " + string(argv[1]) + "\n";
    send(sock, request.c_str(), request.size(), 0);

    string filename = to_string(getpid()) + ".img";
    FILE* file = fopen(filename.c_str(), "wb");

    char buffer[1024];
    int bytes_received;
    while((bytes_received = read(sock, buffer, sizeof(buffer))) > 0) 
    {
        fwrite(buffer, 1, bytes_received, file);
    }
    fclose(file);
    close(sock);

    if(fork() == 0) 
    {
        execlp("display", "display", filename.c_str(), NULL);
        exit(1);
    } 
    wait(NULL);
    return 0;
}