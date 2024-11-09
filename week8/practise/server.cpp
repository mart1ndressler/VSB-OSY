#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <string>
#include <semaphore.h>
#include <sys/socket.h>
#include <cstring>

using namespace std;

struct PlayerSlot 
{
    int socket;
    sem_t* semaphore;
    string sem_name;
};
PlayerSlot players[5];
char current_word[128] = "a";

int next_player(int current) 
{
    int next = (current + 1) % 5;
    while(players[next].socket == -1) 
    {
        next = (next + 1) % 5;
        if(next == current) break;
    }
    return next;
}

void player_game(int pos) 
{
    char received_word[128];
    while(1) 
    {
        sem_wait(players[pos].semaphore);
        string message = "Enter a word starting with '" + string(1, current_word[strlen(current_word) - 1]) + "': ";
        send(players[pos].socket, message.c_str(), message.size(), 0);

        int len = recv(players[pos].socket, received_word, 128, 0);
        if(len <= 0 || received_word[0] == '*') 
        {
            cout << "Position " << pos + 1 << ". Player Disconnected!" << endl;
            close(players[pos].socket);
            players[pos].socket = -1;
            int next = next_player(pos);
            if(next != pos) sem_post(players[next].semaphore);
            break;
        }

        received_word[len] = '\0';
        if(received_word[0] != current_word[strlen(current_word) - 1])
        {
            string invalid_message = "Invalid word!";
            send(players[pos].socket, invalid_message.c_str(), invalid_message.size(), 0);
            sem_post(players[pos].semaphore);
            continue;
        }

        strncpy(current_word, received_word, 128);
        int next = next_player(pos);
        sem_post(players[next].semaphore);
    }
}

int main() 
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0), opt = 1;
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    ::bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    for(int i=0; i < 5; i++) 
    {
        players[i].socket = -1;
        players[i].sem_name = "/sem_player_" + to_string(i);
        players[i].semaphore = sem_open(players[i].sem_name.c_str(), O_CREAT, 0644, 0);
    }

    cout << "Server is running..." << endl;
    int first_connected_player = -1;
    while(1) 
    {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len), position = -1;
        for(int i=0; i < 5; i++) 
        {
            if(players[i].socket == -1) 
            {
                position = i;
                players[i].socket = client_socket;
                break;
            }
        }

        if(position == -1) 
        {
            string full_message = "All slots are full!";
            send(client_socket, full_message.c_str(), full_message.size(), 0);
            close(client_socket);
            cout << "All slots are full!" << endl;
            continue;
        }

        cout << "Position " << position + 1 << ". New Player Connected!" << endl;
        thread client_thread(player_game, position);
        client_thread.detach();
        if(first_connected_player == -1) 
        {
            first_connected_player = position;
            sem_post(players[first_connected_player].semaphore);
        }
    }

    for(int i=0; i < 5; i++) 
    {
        sem_close(players[i].semaphore);
        sem_unlink(players[i].sem_name.c_str());
    }   
    close(server_socket);
    return 0;
}