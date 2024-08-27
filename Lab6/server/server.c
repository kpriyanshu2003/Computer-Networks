#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_file_list(int new_socket)
{
    DIR *d;
    struct dirent *dir;
    struct stat file_stat;
    char buffer[BUFFER_SIZE] = {0};

    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            stat(dir->d_name, &file_stat);  // Use stat() to check if the entry is a regular file
            if (S_ISREG(file_stat.st_mode)) // Reg File
            {
                strcat(buffer, dir->d_name);
                strcat(buffer, "\n");
            }
        }
        closedir(d);
    }
    send(new_socket, buffer, strlen(buffer), 0);
}

void send_file(int new_socket, const char *filename)
{
    FILE *file = fopen(filename, "r");
    char buffer[BUFFER_SIZE] = {0};
    int bytes_read;

    if (file == NULL)
    {
        perror("File not found");
        return;
    }

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0)
    {
        send(new_socket, buffer, bytes_read, 0);
        memset(buffer, 0, BUFFER_SIZE);
    }

    fclose(file);
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("Client connected.\n");
        send_file_list(new_socket);

        read(new_socket, buffer, BUFFER_SIZE);
        send_file(new_socket, buffer);

        printf("File sent. Closing connection with the client.\n");
        close(new_socket);

        memset(buffer, 0, BUFFER_SIZE); // Clear the buffer for the next client
    }
    return 0;
}
