// Write a C program to check whether the Host machine is in Little Endian or Big Endian. Enter a number, print the content of each byte
// location and Convert the Endianness of the same i.e. Little to Big Endian and vice-versa.

#include <stdio.h>

int main()
{
    unsigned int num = 1;
    char *ptr = (char *)&num;

    if (*ptr == 1)
        printf("Host machine is Little Endian\n");
    else
        printf("Host machine is Big Endian\n");

    printf("Content of each byte location:\n");
    for (int i = 0; i < sizeof(num); i++)
        printf("Byte %d: %d\n", i + 1, *(ptr + i));

    printf("Converting Endianness...\n");
    for (int i = 0; i < sizeof(num) / 2; i++)
    {
        char temp = *(ptr + i);
        *(ptr + i) = *(ptr + sizeof(num) - i - 1);
        *(ptr + sizeof(num) - i - 1) = temp;
    }

    printf("Content of each byte location after conversion:\n");
    for (int i = 0; i < sizeof(num); i++)
        printf("Byte %d: %d\n", i + 1, *(ptr + i));

    return 0;
}