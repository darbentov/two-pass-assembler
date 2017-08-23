#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H

#include <stdio.h>

typedef enum bool {
    FALSE, TRUE, FATAL_FALSE
} bool;

void check_allocation(void *ptr);

bool is_comment_or_empty(char *line);

void int_to_bin(int num, char *word_p, int bit_length);

FILE *open_file(char *filename, char *mode, char *extension);

void bin_to_4base(char *src, char *dst, int length);

char *my_strndup(char *src, size_t n);

char *my_strdup(char *src);
#endif
