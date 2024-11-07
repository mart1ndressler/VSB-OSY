#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char* argv[]) 
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

    string animal = argv[1];
    animal += '\n';
    send(sock, animal.c_str(), animal.size(), 0);

    string filename = to_string(getpid()) + ".cl";
    FILE* file = fopen(filename.c_str(), "wb");

    char buffer[1024];
    int bytes_received;
    while((bytes_received = read(sock, buffer, sizeof(buffer))) > 0) 
    {
        fwrite(buffer, 1, bytes_received, file);
    }
    fclose(file);
    close(sock);

    chmod(filename.c_str(), 0755);
    string exec_path = "./" + filename;
    if(fork() == 0) 
    {
        execlp(exec_path.c_str(), filename.c_str(), NULL);
        exit(1);
    }

    wait(NULL);
    return 0;
}