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

// Structure to hold customer details
struct Customer
{
    char account_number[20];
    char customer_id[20];
    float balance;
    char phone_number[15];
};

// Function to display customers with balance more than INR 2000
void displayCustomersWithHighBalance(struct Customer customers[], int count)
{
    printf("\nCustomers with balance more than INR 2000:\n");
    for (int i = 0; i < count; i++)
    {
        if (customers[i].balance > 2000.0)
        {
            printf("Customer ID: %s, Account Number: %s, Balance: %.2f, Phone Number: %s\n",
                   customers[i].customer_id, customers[i].account_number, customers[i].balance, customers[i].phone_number);
        }
    }
}

int main()
{
    int server_fd, new_socket, client_socket[MAX_CLIENTS], max_sd;
    struct sockaddr_in address;
    int opt = 1, addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    struct Customer customers[MAX_CLIENTS];
    int customer_count = 0;

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
                    // Process the customer's data
                    buffer[valread] = '\0';
                    struct Customer customer;
                    sscanf(buffer, "%s %s %f %s", customer.account_number, customer.customer_id, &customer.balance, customer.phone_number);
                    customers[customer_count++] = customer;

                    // Display customers with balance more than INR 2000
                    displayCustomersWithHighBalance(customers, customer_count);

                    // Clear buffer
                    memset(buffer, 0, BUFFER_SIZE);
                }
            }
        }
    }

    return 0;
}