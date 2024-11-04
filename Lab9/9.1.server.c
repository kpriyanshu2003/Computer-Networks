#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main()
{
    int server_fd, client1_fd, client2_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int num1, num2;
    char buffer[MAX] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 2) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    if ((client1_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Accept failed for client 1");
        exit(EXIT_FAILURE);
    }
    read(client1_fd, buffer, MAX);
    num1 = atoi(buffer);
    printf("Client 1 sent number: %d\n", num1);

    if ((client2_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Accept failed for client 2");
        exit(EXIT_FAILURE);
    }
    read(client2_fd, buffer, MAX);
    num2 = atoi(buffer);
    printf("Client 2 sent number: %d\n", num2);

    int addition = num1 + num2;
    int multiplication = num1 * num2;
    int subtraction = num1 - num2;
    int division = num1 / num2;
    int modulo = num1 % num2;

    sprintf(buffer, "Addition: %d, Multiplication: %d", addition, multiplication);
    send(client1_fd, buffer, strlen(buffer), 0);
    send(client2_fd, buffer, strlen(buffer), 0);
    printf("Sent to both clients: %s\n", buffer);

    sprintf(buffer, "Subtraction: %d, Division: %d", subtraction, division);
    send(client1_fd, buffer, strlen(buffer), 0);
    printf("Sent to Client 1: %s\n", buffer);

    sprintf(buffer, "Modulo: %d", modulo);
    send(client2_fd, buffer, strlen(buffer), 0);
    printf("Sent to Client 2: %s\n", buffer);

    close(client1_fd);
    close(client2_fd);
    close(server_fd);

    return 0;
}
