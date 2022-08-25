#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _PROJECT__H_
#define _PROJECT__H_

/**
 * @brief A student data structure
 * 
 */
struct student
{
    char id[7];
    char *firstname;
    char *lastname;
    int points[6];
};

int init_student(struct student *p_student, char *p_id, char *p_firstname, char *p_lastname);

struct student *add_student_to_collection(struct student *collection, unsigned int size, struct student new_student);

int calculate_total_points(struct student s);

void print_students(struct student *collection, unsigned int n, FILE *stream);

struct student *get_student_by_id(char *id, struct student *collection, unsigned int n);

int student_already_in_collection(char *id, struct student *collection, unsigned int n);

#endif //! _PROJECT__H_