#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

#define PORT 8080
#define BUF_SIZE 1024

void handle_client(int client_sock)
{
    char buffer[BUF_SIZE];
    ssize_t nbytes;

    while (1)
    {
        memset(buffer, 0, BUF_SIZE);
        nbytes = read(client_sock, buffer, BUF_SIZE);
        if (nbytes <= 0)
        {
            perror("Read error");
            break;
        }

        printf("Client: %s\n", buffer);

        // Check if client sent "logout"
        if (strncmp(buffer, "logout", 6) == 0)
        {
            printf("Client requested logout.\n");
            break;
        }

        // Echo the message back to the client
        nbytes = write(client_sock, buffer, strlen(buffer));
        if (nbytes <= 0)
        {
            perror("Write error");
            break;
        }
    }

    close(client_sock);
    exit(0); // Terminate child process
}

int main()
{
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pid_t pid;

    // Create a TCP socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified IP and port
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind error");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) < 0)
    {
        perror("Listen error");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1)
    {
        // Accept a new connection
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
        if (client_sock < 0)
        {
            perror("Accept error");
            continue;
        }

        // Fork a new process to handle the client
        pid = fork();
        if (pid < 0)
        {
            perror("Fork error");
            close(client_sock);
            continue;
        }
        else if (pid == 0)
        {                       // Child process
            close(server_sock); // Child does not need the server socket
            handle_client(client_sock);
        }
        else
        {                       // Parent process
            close(client_sock); // Parent does not need the client socket
        }
    }

    close(server_sock);
    return 0;
}
