// Create a calculator, the client will send all operators and operands.Server will perform operations with provided operators and display results.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define MAX 1024

int calculate(int num1, int num2, char operator)
{
    switch (operator)
    {
    case '+':
        return num1 + num2;
    case '-':
        return num1 - num2;
    case '*':
        return num1 * num2;
    case '/':
        return num1 / num2;
    default:
        printf("Invalid operator\n");
        return 0;
    }
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX] = {0};
    char response[MAX];

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

    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", PORT);

    while (1)
    {

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        read(new_socket, buffer, MAX);
        printf("Received operation: %s\n", buffer);

        int num1, num2;
        char operator;
        sscanf(buffer, "%d%c%d", &num1, &operator, & num2);

        int result = calculate(num1, num2, operator);

        sprintf(response, "Result: %d", result);
        send(new_socket, response, strlen(response), 0);

        close(new_socket);
        memset(buffer, 0, MAX);
    }

    return 0;
}
