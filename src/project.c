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

    for (int i = 0; i < 6; i++)
    {
        p_student->points[i] = 0;
    }

    return 1;
}

struct student *add_student_to_collection(struct student *collection, unsigned int size, struct student new_student)
{
    struct student *new_collection = new_collection = realloc(collection, (size + 1) * sizeof(struct student));
    new_collection[size] = new_student;

    return new_collection;
}

int calculate_total_points(struct student s)
{
    int sum = 0;
    for (int i = 0; i < 6; i++)
    {
        sum += s.points[i];
    }

    return sum;
}

void print_students(struct student *collection, unsigned int n)
{
    struct student s;
    for (unsigned int i = 0; i < n; i++)
    {
        s = collection[i];
        printf("%s %s %s ", s.id, s.firstname, s.lastname);
        for (int j = 0; j < 6; j++)
        {
            printf("%d ", s.points[j]);
        }
        printf("%d\n", calculate_total_points(s));
    }
}

int student_already_in_collection(char* id, struct student *collection, unsigned int n)
{
    if (n == 0) return 0;

    for (unsigned int i=0; i<n; i++)
    {
        if (strcmp(id, collection[i].id) == 0)
        {
            return 1;
        }
    }

    return 0;
}

void delete_student(struct student *s)
{
    free(s->firstname);
    free(s->lastname);
}

void delete_collection(struct student *collection, unsigned int n)
{
    for (unsigned int i = 0; i<n; i++)
    {
        delete_student(&collection[i]);
    }

    free(collection);
}

int main(void)
{
    struct student *student_collection = NULL;
    unsigned int size = 0;
    char buffer[1000];
    char command = 'X';
    
    while (command != 'Q')
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
                printf("Error! Invalid arguments\n");
                break;
            }

            if (student_already_in_collection(new_student.id, student_collection, size))
            {
                printf("Error! Student already in collection\n");
                delete_student(&new_student);
                break;
            }

            student_collection = add_student_to_collection(student_collection, size, new_student);
            size++;
            printf("SUCCESS\n");
            break;
        case 'L':
            print_students(student_collection, size);
            printf("SUCCESS\n");
            break;
        case 'Q':
            break;
        default:
            printf("Invalid command %c\n", command);
            break;
        }
    }

    delete_collection(student_collection, size);
}