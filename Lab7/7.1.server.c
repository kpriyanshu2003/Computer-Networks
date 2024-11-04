/*
    Write a program to send an array of n integers from client to server using UDP socket. The server will reply
    to the client with the sum of all odd numbers and the sum of all even numbers separately.

    Example:
                Server                                                                            Client
    -----------------------------------------------                 -----------------------------------------------------
    The server received the array as follows:                       Enter how many numbers to input an array: 5
    {1, 2, 5, 7, 6}                                                 Enter 5 integers for the array: 1 2 5 7 6
    Server sending the result                                       The client received from the server
    (Please press enter)                                            ODD Sum = 13
                                                                    Even Sum = 8
    N.B: As soon as you will press the enter key on the server side, the result will be displayed on the client side.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024

int main()
{
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    int n, len;

    // Create socket file descriptor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        printf("Waiting for client to send data...\n");
        int numbers[MAXLINE / sizeof(int)];
        len = sizeof(cliaddr);

        // Receive the size of the array
        n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        int num_count = atoi(buffer);

        // Receive the array of integers
        n = recvfrom(sockfd, numbers, num_count * sizeof(int), MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        printf("The server received the array as follows: ");
        for (int i = 0; i < num_count; i++)
        {
            printf("%d ", numbers[i]);
        }
        printf("\n");

        // Calculate sum of odd and even numbers
        int odd_sum = 0, even_sum = 0;
        for (int i = 0; i < num_count; i++)
        {
            if (numbers[i] % 2 == 0)
                even_sum += numbers[i];
            else
                odd_sum += numbers[i];
        }

        // Send the sums back to the client
        sprintf(buffer, "ODD Sum = %d\nEven Sum = %d", odd_sum, even_sum);
        sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("Server sending the result (Please press enter)\n");

        getchar(); // Wait for Enter key to be pressed
    }

    close(sockfd);
    return 0;
}