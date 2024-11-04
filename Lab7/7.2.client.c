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

// Function to sort characters in descending order
void sort_descending(char *arr, int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] < arr[j])
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
    struct sockaddr_in servaddr;
    socklen_t len;

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

    // Input the array of alphabets from the user
    printf("Enter a string of alphabets to sort: ");
    scanf("%[^\n]%*c", buf);

    int len_input = strlen(buf);

    // Send the array to the server
    sendto(sockfd, buf, len_input, MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("Array sent to server. Waiting for response...\n");

    // Receive the sorted array from the server
    n = recvfrom(sockfd, buf, MAXLEN, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buf[n] = '\0';

    // Sort the array in descending order
    sort_descending(buf, n);
    printf("Sorted array in descending order: %s\n", buf);

    close(sockfd);
    return 0;
}