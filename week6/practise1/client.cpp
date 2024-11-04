#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) 
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[1024];

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

    string season = argv[1], filename = to_string(getpid()) + ".img";
    season += '\n';
    send(sock, season.c_str(), season.size(), 0);

    FILE* image_file = fopen(filename.c_str(), "wb");
    int bytes_received;

    while((bytes_received = read(sock, buffer, sizeof(buffer))) > 0) 
    {
        fwrite(buffer, 1, bytes_received, image_file);
    }
    fclose(image_file);
    close(sock);

    if(fork() == 0) 
    {
        execlp("feh", "feh", filename.c_str(), NULL);
        exit(0);
    }
    wait(NULL);
    return 0;
}