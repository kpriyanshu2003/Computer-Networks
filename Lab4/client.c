/*
    Write a connection - oriented client and server program in C using command line arguments.At the server side, pass the port
    number (to whom the server will bind to) in the command line.At the client side, pass the IP address and the port number of the server
    (to whom the client will connect to) as command line argument and carry out the following tasks.

    ◦ After establishment of connection print the IP Address and port number of the client to whom the server is connected now.
    ◦ Then exchange messages.
    ◦ After message exchange is over then the client sends a “close” message to the server to tear down the connection
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <IP address> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    char *ip_address = argv[1];
    int port = atoi(argv[2]);

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address/Address not supported");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Connection failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", ip_address, port);

    while (1)
    {
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        if (send(sock_fd, buffer, strlen(buffer), 0) == -1)
        {
            perror("send failed");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }

        if (strcmp(buffer, "close") == 0)
        {
            printf("Closing connection to the server\n");
            break;
        }

        N if (bytes_received < 0)
        {
            perror("recv failed");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }
        else if (bytes_received == 0)
        {
            printf("Connection closed by server\n");
            break;
        }

        printf("Server: %s\n", buffer);
    }

    close(sock_fd);
    return 0;
}