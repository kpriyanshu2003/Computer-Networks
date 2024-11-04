// Write a program where more than two clients will send bank customer details (Account Number, Customer ID, Balance, Phone Number) and
// server will display customer names whose balance is more than INR 2000.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection failed\n");
        return -1;
    }

    // Input customer details
    char account_number[20], customer_id[20], phone_number[15];
    float balance;
    printf("Enter Account Number, Customer ID, Balance, Phone Number: ");
    scanf("%s %s %f %s", account_number, customer_id, &balance, phone_number);

    // Send customer details to server
    sprintf(buffer, "%s %s %.2f %s", account_number, customer_id, balance, phone_number);
    send(sock, buffer, strlen(buffer), 0);

    close(sock);
    return 0;
}