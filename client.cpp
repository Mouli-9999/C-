#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>

#define PORT 9999
#define BUFFER_SIZE 1024
using namespace std;

int main() 
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
        perror("Error in Socket creation");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) 
	{
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connection to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    cout << "Connected to the server. Start chatting!\n";

    while (1) 
	{
        // Send message to server
        cout << "You: ";
        string message;
        getline(cin, message);
        send(sock, message.c_str(), message.size(), 0);

        // Receive message from server
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) break;

        cout << "Server: " << buffer << "\n";
    }

    close(sock);
    cout << "Connection closed.\n";
    return 0;
}
