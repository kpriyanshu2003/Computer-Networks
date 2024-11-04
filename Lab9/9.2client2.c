/*
    Write a program when Client1 will send one number and Client2 will send another number. Server will perform all arithmetic operations.
    i) Server will display the result of Addition and Multiplication
    ii) Client1 will display  the result of Division and Subtraction
    iii) Client2 will display the result of Modulo operation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[MAX] = {0};
    char number[MAX];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("Invalid address\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connection failed\n");
        return -1;
    }

    printf("Enter a number (Client 2): ");
    scanf("%s", number);
    send(sock, number, strlen(number), 0);

    read(sock, buffer, MAX);
    printf("Received from server: %s\n", buffer);

    memset(buffer, 0, MAX);
    read(sock, buffer, MAX);
    printf("Received from server: %s\n", buffer);

    close(sock);

    return 0;
}
