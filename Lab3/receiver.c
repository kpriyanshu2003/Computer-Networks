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
        fprintf(stderr, "Usage: %s <Sender IP> <Sender Port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *sender_ip = argv[1];
    int sender_port = atoi(argv[2]);

    int sockfd;
    struct sockaddr_in sender_addr, my_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(sender_port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&my_addr, sizeof(my_addr)) < 0)
    {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    addr_len = sizeof(sender_addr);

    while (1)
    {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&sender_addr, &addr_len);
        buffer[n] = '\0';
        printf("Sender: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
            break;

        printf("Receiver: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&sender_addr, addr_len);

        if (strcmp(buffer, "exit") == 0)
            break;
    }

    close(sockfd);
    return 0;
}