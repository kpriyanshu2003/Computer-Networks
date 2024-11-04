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
    struct sockaddr_in servaddr;

    // Create socket file descriptor
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // Input the array size and elements from the user
    printf("Enter how many numbers to input in the array: ");
    int num_count;
    scanf("%d", &num_count);

    int numbers[num_count];
    printf("Enter %d integers for the array: ", num_count);
    for (int i = 0; i < num_count; i++)
    {
        scanf("%d", &numbers[i]);
    }

    // Send the size of the array
    sprintf(buffer, "%d", num_count);
    sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    // Send the array of integers
    sendto(sockfd, numbers, num_count * sizeof(int), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Array sent to server. Waiting for response...\n");

    // Receive the result from the server
    int n, len;
    n = recvfrom(sockfd, buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("The client received from the server\n%s\n", buffer);

    close(sockfd);
    return 0;
}