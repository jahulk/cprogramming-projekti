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
    if (strlen(p_id) > 6)
    {
        printf("Student number %s cannot be more than 6 digits.\n", p_id);
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
    struct student *new_collection = realloc(collection, (size + 1) * sizeof(struct student));
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

struct student *get_student_by_id(char *id, struct student *collection, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
    {
        if (strcmp(id, collection[i].id) == 0)
        {
            return &collection[i];
        }
    }

    return NULL;
}

int student_already_in_collection(char *id, struct student *collection, unsigned int n)
{
    if (n == 0)
        return 0;

    if (get_student_by_id(id, collection, n) != NULL)
    {
        return 1;
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
    for (unsigned int i = 0; i < n; i++)
    {
        delete_student(&collection[i]);
    }

    free(collection);
}

int compare(const void *a, const void *b)
{
    struct student *eka = (struct student *)a;
    struct student *toka = (struct student *)b;

    return  (-1)*(calculate_total_points(*eka)-calculate_total_points(*toka));
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
        int args;
        char id[100];
        switch (command)
        {
        case 'A':
        {
            char firstname[100];
            char lastname[100];
            args = sscanf(buffer, "A %s %s %s", id, firstname, lastname);

            if (student_already_in_collection(id, student_collection, size))
            {
                printf("Student %s is already in the database.\n", id);
                break;
            }
            
            if (args != 3)
            {
                printf("A should be followed by exactly 3 arguments.\n");
                break;
            }

            
            struct student new_student;
            int retval = init_student(&new_student, id, firstname, lastname);

            if (retval == 0)
            {
                break;
            }

            student_collection = add_student_to_collection(student_collection, size, new_student);
            size++;
            printf("SUCCESS\n");
            break;
        }
        case 'U':
        {
            int round;
            int new_points;
            args = sscanf(buffer, "U %s %d %d", id, &round, &new_points);
            struct student *s = get_student_by_id(id, student_collection, size);

            if (args != 3)
            {
                printf("U should be followed by exactly 3 arguments.\n");
                break;
            }

            if (s == NULL)
            {
                printf("Student %s is not in the database.\n", id);
                break;
            }

            if (round < 1 || round > 6)
            {
                printf("Round cannot be less than 1 or larger than 6\n");
                break;
            }

            if (new_points < 0)
            {
                printf("Student cannot have negative points.\n");
                break;
            }

            s->points[round - 1] = new_points;
            printf("SUCCESS\n");
            break;
        }
        case 'L':
            // Sort students based on total points in ascending order
            qsort(student_collection, size, sizeof(struct student), compare);
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