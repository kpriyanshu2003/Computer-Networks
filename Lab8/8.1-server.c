// Write a program where more than two clients will send student details (SID, Name, Age, CGPA) and server will sort student details in
// ascending order of CGPA and display in the server.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

#define PORT 8080
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

// Structure to hold student details
struct Student
{
    int sid;
    char name[50];
    int age;
    float cgpa;
};

// Function to compare students by CGPA
int compareByCGPA(const void *a, const void *b)
{
    struct Student *studentA = (struct Student *)a;
    struct Student *studentB = (struct Student *)b;
    return (studentA->cgpa > studentB->cgpa) - (studentA->cgpa < studentB->cgpa);
}

int main()
{
    int server_fd, new_socket, client_socket[MAX_CLIENTS], max_sd;
    struct sockaddr_in address;
    int opt = 1, addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    struct Student students[MAX_CLIENTS];
    int student_count = 0;

    fd_set readfds;

    // Initialize client sockets to 0
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        client_socket[i] = 0;
    }

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the address
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        // Clear the socket set and add server socket to set
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Add child sockets to set
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (client_socket[i] > 0)
            {
                FD_SET(client_socket[i], &readfds);
            }

            if (client_socket[i] > max_sd)
            {
                max_sd = client_socket[i];
            }
        }

        // Wait for an activity on one of the sockets
        int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            perror("select error");
        }

        // If something happened on the master socket, it's an incoming connection
        if (FD_ISSET(server_fd, &readfds))
        {
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("New connection from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            // Add new socket to array of sockets
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets at index %d\n", i);
                    break;
                }
            }
        }

        // Handle input from clients
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            int sd = client_socket[i];

            if (FD_ISSET(sd, &readfds))
            {
                int valread = read(sd, buffer, BUFFER_SIZE);
                if (valread == 0)
                {
                    // Client disconnected
                    getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
                    printf("Client disconnected, ip %s, port %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
                    close(sd);
                    client_socket[i] = 0;
                }
                else
                {
                    // Process the student's data
                    buffer[valread] = '\0';
                    struct Student student;
                    sscanf(buffer, "%d %s %d %f", &student.sid, student.name, &student.age, &student.cgpa);
                    students[student_count++] = student;

                    // Sort students by CGPA
                    qsort(students, student_count, sizeof(struct Student), compareByCGPA);

                    // Display sorted students
                    printf("Sorted Student List by CGPA:\n");
                    for (int j = 0; j < student_count; j++)
                    {
                        printf("SID: %d, Name: %s, Age: %d, CGPA: %.2f\n",
                               students[j].sid, students[j].name, students[j].age, students[j].cgpa);
                    }

                    // Clear buffer
                    memset(buffer, 0, BUFFER_SIZE);
                }
            }
        }
    }

    return 0;
}