/*  Write a C Program to enter a number and store the number across the following structure and print the content of each member of the structure.
    Then aggregate each member of the structure to form the original number and print the same.
    struct pkt{
        char ch1;
        char ch2[2];
        char ch3;
    };
*/

#include <stdio.h>

struct pkt
{
    char ch1;
    char ch2[2];
    char ch3;
};

int main()
{
    struct pkt packet;
    int number;

    printf("Enter a number: ");
    scanf("%d", &number);

    packet.ch1 = (char)(number % 256);
    packet.ch2[0] = (char)((number >> 8) % 256);
    packet.ch2[1] = (char)((number >> 16) % 256);
    packet.ch3 = (char)(number >> 24);

    printf("Content of each member of structure:\n");
    printf("ch1: %d\n", packet.ch1);
    printf("ch2[0]: %d\n", packet.ch2[0]);
    printf("ch2[1]: %d\n", packet.ch2[1]);
    printf("ch3: %d\n", packet.ch3);

    int reconstruct = (packet.ch3 << 24) | (packet.ch2[1] << 16) | (packet.ch2[0] << 8) | packet.ch1;
    printf("Reconstructed number: %d\n", reconstruct);

    return 0;
}