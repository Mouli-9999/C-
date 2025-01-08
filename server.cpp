#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>

#define PORT 9999
#define BUFFER_SIZE 1024
using namespace std;

int main() 
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) 
	{
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) 
	{
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    cout << "Server is Ready and waiting for a connection...\n";

    // Accept a connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) 
	{
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    cout << "Client connected. Start chatting!\n";

    while (1) 
	{
        // Receive message from client
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0) break;

        cout << "Client: " << buffer << "\n";

        // Send message to client
        cout << "You: ";
        string message;
        getline(cin, message);
        send(new_socket, message.c_str(), message.size(), 0);
    }

    close(new_socket);
    close(server_fd);
    cout << "Connection closed.\n";
    return 0;
}
