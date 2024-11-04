// Create a calculator, the client will send all operators and operands.Server will perform operations with provided operators and display results.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define MAX 1024

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[MAX] = {0};
    char operation[MAX];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid address/ Address not supported\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection Failed\n");
        return -1;
    }

    printf("Enter operation (e.g., 12+5): ");
    scanf("%s", operation);

    send(sock, operation, strlen(operation), 0);

    read(sock, buffer, MAX);
    printf("%s\n", buffer);

    close(sock);

    return 0;
}
