/* Write a C program to assign values to each member of the following structure.Pass the populated structure to a function.
    Using call - by - value and another function using call - by - address and print the value of each member of the structure.
    struct student_info
    {
        int roll_no;
        char name[50];
        float CGPA;
        struct dob age;
    };
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dob
{
    int day, month, year;
};

struct student_info
{
    int roll_no;
    char name[50];
    float CGPA;
    struct dob age;
};

void print_value(struct student_info s)
{
    printf("\nCall by Value\n");
    printf("Roll number: %d\n", s.roll_no);
    printf("Name: %s\n", s.name);
    printf("CGPA: %f\n", s.CGPA);
    printf("DoB: %d-%d-%d\n", s.age.day, s.age.month, s.age.year);
}

void print_address(struct student_info *s)
{
    printf("\nCall by Address\n");
    printf("Roll number: %d\n", s->roll_no);
    printf("Name: %s\n", s->name);
    printf("CGPA: %f\n", s->CGPA);
    printf("DoB: %d-%d-%d\n", s->age.day, s->age.month, s->age.year);
}

int main()
{
    struct student_info student1;

    student1.roll_no = 101;
    strcpy(student1.name, "Priyanshu");
    student1.CGPA = 9.00;
    student1.age.day = 21;
    student1.age.month = 11;
    student1.age.year = 2003;

    print_value(student1);
    printf("\n");
    print_address(&student1);

    return 0;
}