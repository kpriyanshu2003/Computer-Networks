// Write a program where more than two clients will send student details (SID, Name, Age, CGPA) and server will sort student details in 
// ascending order of CGPA and display in the server.

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

    // Input student details
    int sid, age;
    char name[50];
    float cgpa;
    printf("Enter Student ID, Name, Age, CGPA: ");
    scanf("%d %s %d %f", &sid, name, &age, &cgpa);

    // Send student details to server
    sprintf(buffer, "%d %s %d %.2f", sid, name, age, cgpa);
    send(sock, buffer, strlen(buffer), 0);

    close(sock);
    return 0;
}