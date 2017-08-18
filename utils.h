#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H

#endif //ASSEMBLER_UTILS_H

typedef enum bool{
    TRUE, FALSE
} bool;

void check_allocation(void *ptr);

short int is_comment_or_empty(char *line);