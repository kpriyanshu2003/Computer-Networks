/*
    Write a sender and receiver program in C by passing the IP address and the port number of each other through the command line arguments using
    connection less socket.Both of them will exchange messages with each other continuously.If any one of them will receive the “exit” message from
    the other end then both of them will close the connection.(Assume both the client and server are running with in the same host)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <Receiver IP> <Receiver Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *receiver_ip = argv[1];
    int receiver_port = atoi(argv[2]);

    int sockfd;
    struct sockaddr_in receiver_addr;

    char buffer[BUFFER_SIZE];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(receiver_port);
    receiver_addr.sin_addr.s_addr = inet_addr(receiver_ip);

    while (1)
    {
        printf("Sender: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&receiver_addr, sizeof(receiver_addr));

        if (strcmp(buffer, "exit") == 0)
            break;

        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, NULL, NULL);
        buffer[n] = '\0';
        printf("Receiver: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
            break;
    }

    close(sockfd);
    return 0;
}