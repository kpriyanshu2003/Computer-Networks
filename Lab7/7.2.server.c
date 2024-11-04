/*
    Write a program where client will send an array of alphabets, and server will sort it.
    i) Server will display the sorted alphabets in ascending order.
    ii) Client will display the sorted alphabets in descending order .
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAXLEN 1024

// Function to sort characters in ascending order
void sort_ascending(char *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] > arr[j])
            {
                char temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
}

int main()
{
    int sockfd, n;
    char buf[MAXLEN];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

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

        len = sizeof(cliaddr);
        // Receive the array of alphabets from client
        n = recvfrom(sockfd, buf, MAXLEN, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buf[n] = '\0';

        printf("Received array from client: %s\n", buf);

        // Sort the array in ascending order
        sort_ascending(buf, n);

        printf("Sorted array in ascending order: %s\n", buf);

        // Send the sorted array back to the client
        sendto(sockfd, buf, n, MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
        printf("Sorted array sent back to client.\n");
    }

    close(sockfd);
    return 0;
}