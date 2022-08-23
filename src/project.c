#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student
{
    char id[7];
    char *firstname;
    char *lastname;
    int points[6];
};

int init_student(struct student *p_student, char *p_id, char *p_firstname, char *p_lastname)
{
    int numvalue = atoi(p_id);
    if (numvalue > 999999 || numvalue < 0)
    {
        return 0;
    }

    for (int i = 0; i < 7; i++)
    {
        p_student->id[i] = p_id[i];
    }

    // TODO: Virheentarkistus etunimelle ja sukunimelle
    p_student->firstname = malloc(strlen(p_firstname) + 1);
    strcpy(p_student->firstname, p_firstname);

    p_student->lastname = malloc(strlen(p_lastname) + 1);
    strcpy(p_student->lastname, p_lastname);

    for (int i=0; i<6; i++)
    {
        p_student->points[i] = 0;
    }
    
    return 1;
}

struct student *add_student_to_collection(struct student *collection, unsigned int size, struct student new_student)
{
    struct student *new_collection = new_collection = (struct student*) realloc(collection, (size+1)*sizeof(struct student));
    new_collection[size] = new_student;

    return new_collection;
}

void print_student(struct student s)
{
    printf("%s %s %s\n", s.id, s.firstname, s.lastname);
}

void print_students(struct student *collection, unsigned int n)
{
    struct student s;
    for (unsigned int i=0; i < n; i++)
    {
        s = collection[i];
        printf("%s %s %s ", s.id, s.firstname, s.lastname);
        for (int j=0; j<6; j++)
        {
            printf("%d ", s.points[j]);
        }
        printf("\n");
    }
}

int main(void)
{
    struct student *student_collection = NULL;
    unsigned int size = 0;
    char buffer[1000];
    char command;
    int keep_going = 1;
    while (keep_going)
    {
        fgets(buffer, 1000, stdin);
        command = buffer[0];

        switch (command)
        {
        case 'A':;
            char id[7];
            char firstname[20];
            char lastname[20];
            sscanf(buffer, "A %s %s %s", id, firstname, lastname);
            struct student new_student;
            int retval = init_student(&new_student, id, firstname, lastname);
            if (retval == 0)
            {
                printf("Error!\n");
                break;
            }
            student_collection = add_student_to_collection(student_collection, size, new_student);
            size++;
            printf("SUCCESS\n");
            break;
        case 'L':
            ;
            print_students(student_collection, size);
            break;
        case 'Q':
            keep_going = 0;
            break;
        default:
            printf("Invalid command %c\n", command);
            break;
        }
    }
}