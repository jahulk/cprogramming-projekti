#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Initializes student and reserves memory for fields
 * 
 * @param p_student Student to initialize
 * @param p_id Id
 * @param p_firstname Firstname
 * @param p_lastname Lastname
 * @return int Returns 1 if operation successful
 */
int init_student(struct student *p_student, char *p_id, char *p_firstname, char *p_lastname)
{
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

/**
 * @brief Adds student to collection
 * 
 * @param collection Student collection
 * @param size Current size
 * @param new_student Student to add
 * @return struct student* Returns pointer to new collection
 */
struct student *add_student_to_collection(struct student *collection, unsigned int size, struct student new_student)
{
    struct student *new_collection = realloc(collection, (size + 1) * sizeof(struct student));
    new_collection[size] = new_student;

    return new_collection;
}

/**
 * @brief Calculates total exercise points for student
 * 
 * @param s Student
 * @return int Total points
 */
int calculate_total_points(struct student s)
{
    int sum = 0;
    for (int i = 0; i < 6; i++)
    {
        sum += s.points[i];
    }

    return sum;
}

/**
 * @brief Prints student information to stream
 * 
 * @param collection Student collection
 * @param n Number of students in collection
 * @param stream Output stream
 */
void print_students(struct student *collection, unsigned int n, FILE *stream)
{
    struct student s;
    for (unsigned int i = 0; i < n; i++)
    {
        s = collection[i];
        fprintf(stream, "%s %s %s ", s.id, s.firstname, s.lastname);
        for (int j = 0; j < 6; j++)
        {
            fprintf(stream, "%d ", s.points[j]);
        }
        fprintf(stream, "%d\n", calculate_total_points(s));
    }
}

/**
 * @brief Finds student by id
 * 
 * @param id Id of student
 * @param collection Student collection
 * @param n Number of students in collection
 * @return struct student* Returns pointer to student if found, otherwise returns NULL
 */
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

/**
 * @brief Check's if student is already in collection
 * 
 * @param id Student's Id
 * @param collection Student collection
 * @param n Number of students
 * @return int Returns 1 if student already in collection, otherwise returns 0
 */
int student_already_in_collection(char *id, struct student *collection, unsigned int n)
{
    if (get_student_by_id(id, collection, n) != NULL)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief Frees memory reserved  for firstname and lastname fields
 *
 * @param s Student to delete
 */
void delete_student(struct student *s)
{
    free(s->firstname);
    free(s->lastname);
}

/**
 * @brief Frees memory reserved for collection
 *
 * @param collection Collection to delete
 * @param n Size of collection
 */
void delete_collection(struct student *collection, unsigned int n)
{
    for (unsigned int i = 0; i < n; i++)
    {
        delete_student(&collection[i]);
    }

    free(collection);
}

/**
 * @brief Compares 2 student's total points
 *
 * @param a First student
 * @param b Second student
 * @return int Integer
 */
int compare(const void *a, const void *b)
{
    struct student *eka = (struct student *)a;
    struct student *toka = (struct student *)b;

    return (-1) * (calculate_total_points(*eka) - calculate_total_points(*toka));
}

int main(void)
{
    struct student *student_collection = NULL;
    unsigned int size = 0;
    FILE *f = NULL;
    char buffer[1000];
    char command = 'X';

    while (command != 'Q')
    {
        fgets(buffer, 1000, stdin);
        command = buffer[0];
        // Number of succesfully read args
        int args;
        char id[100];
        char firstname[100];
        char lastname[100];
        char filename[100];

        switch (command)
        {
        case 'A':
        {
            args = sscanf(buffer, "A %s %s %s", id, firstname, lastname);

            if (args != 3)
            {
                printf("A should be followed by exactly 3 arguments.\n");
                break;
            }

            if (student_already_in_collection(id, student_collection, size))
            {
                printf("Student %s is already in the database.\n", id);
                break;
            }

            if (strlen(id) > 6)
            {
                printf("Student number %s cannot be more than 6 digits.\n", id);
                return 0;
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
            // Sort students based on total points in descending order
            qsort(student_collection, size, sizeof(struct student), compare);
            print_students(student_collection, size, stdout);
            printf("SUCCESS\n");
            break;
        case 'W':
        {
            args = sscanf(buffer, "W %s", filename);

            if (args != 1)
            {
                printf("U should be followed by exactly 1 argument.\n");
                break;
            }

            f = fopen(filename, "w");

            if (!f)
            {
                printf("Cannot open file %s for writing.\n", filename);
                break;
            }

            qsort(student_collection, size, sizeof(struct student), compare);
            print_students(student_collection, size, f);
            printf("SUCCESS\n");
            fclose(f);
            break;
        }
        case 'O':
        {
            args = sscanf(buffer, "O %s", filename);

            if (args != 1)
            {
                printf("O should be followed by exactly 1 argument.\n");
                break;
            }

            f = fopen(filename, "r");

            if (!f)
            {
                printf("Cannot open file %s for reading.\n", filename);
                break;
            }

            // Delete old collection
            delete_collection(student_collection, size);
            student_collection = NULL;
            size = 0;

            int p1, p2, p3, p4, p5, p6;

            while (fgets(buffer, 1000, f) != NULL)
            {
                args = sscanf(buffer, "%s %s %s %d %d %d %d %d %d", id, firstname, lastname, &p1, &p2, &p3, &p4, &p5, &p6);
                struct student new_student;
                init_student(&new_student, id, firstname, lastname);
                new_student.points[0] = p1;
                new_student.points[1] = p2;
                new_student.points[2] = p3;
                new_student.points[3] = p4;
                new_student.points[4] = p5;
                new_student.points[5] = p6;
                student_collection = add_student_to_collection(student_collection, size, new_student);
                size++;
            }

            printf("SUCCESS\n");
            fclose(f);
            break;
        }
        case 'Q':
            printf("SUCCESS\n");
            break;
        default:
            printf("Invalid command %c\n", command);
            break;
        }
    }

    delete_collection(student_collection, size);
}