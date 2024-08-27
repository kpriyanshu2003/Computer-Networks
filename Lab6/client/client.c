#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[])
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char filename[100];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    read(sock, buffer, BUFFER_SIZE);
    printf("Files available on the server:\n%s", buffer);

    printf("Enter the filename to download: ");
    scanf("%s", filename);

    send(sock, filename, strlen(filename), 0);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return -1;
    }

    int bytes_received;
    while ((bytes_received = read(sock, buffer, BUFFER_SIZE)) > 0)
    {
        fwrite(buffer, 1, bytes_received, file);
    }

    printf("File downloaded successfully\n");

    fclose(file);
    close(sock);

    return 0;
}