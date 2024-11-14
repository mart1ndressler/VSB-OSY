#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <semaphore.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/stat.h>

using namespace std;

struct ImageData 
{
    sem_t semaphore;
    char *img;
    int size;
};

const char* seasons[] = {"jaro", "leto", "podzim", "zima", "error"};
const char* files[] = {"images/jaro.png", "images/leto.png", "images/podzim.png", "images/zima.png", "images/error.png"};
ImageData images[5];

void load_images() 
{
    for(int i = 0; i < 5; i++) 
    {
        FILE* file = fopen(files[i], "rb");
        if(file) 
        {
            fseek(file, 0, SEEK_END);
            int size = ftell(file);
            fseek(file, 0, SEEK_SET);
            char *buffer = new char[size];
            fread(buffer, 1, size, file);
            fclose(file);

            sem_init(&images[i].semaphore, 0, 1);
            images[i].img = buffer;
            images[i].size = size;
        }
    }
}

void send_image(int client_socket, char season[]) 
{
    int index = 4;
    for(int i = 0; i < 4; i++) 
    {
        if(strcmp(seasons[i], season) == 0) 
        {
            index = i;
            break;
        }
    }

    ImageData &img_data = images[index];
    sem_wait(&img_data.semaphore);

    int buffer_size = 1024;
    for(int i = 0; i < img_data.size; i += buffer_size) 
    {
        int chunk_size = (buffer_size < (img_data.size - i)) ? buffer_size : (img_data.size - i);
        send(client_socket, img_data.img + i, chunk_size, 0);
        usleep(5000000 / (img_data.size / buffer_size));
    }

    sem_post(&img_data.semaphore);
    close(client_socket);
}

void handle_client(int client_socket) 
{
    char buffer[128];
    int len = read(client_socket, buffer, sizeof(buffer) - 1);
    if(len > 0) 
    {
        buffer[len] = '\0';
        if(strncmp(buffer, "#img ", 5) == 0)
        {
            char* season = buffer + 5;
            season[strcspn(season, "\r\n ")] = '\0';
            send_image(client_socket, season);
        }
    }
    close(client_socket);
}


int main() 
{
    load_images();

    int server_socket = socket(AF_INET, SOCK_STREAM, 0), opt = 1;
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    cout << "Server is running..." << endl;
    while(1) 
    {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);
        thread client_thread(handle_client, client_socket);
        client_thread.detach();
    }

    for(int i = 0; i < 5; i++) 
    {
        sem_destroy(&images[i].semaphore);
        delete[] images[i].img;
    }

    close(server_socket);
    return 0;
}