#ifndef ASSEMBLER_UTILS_H
#define ASSEMBLER_UTILS_H

#include <stdio.h>

typedef enum bool {
    FALSE, TRUE
} bool;

void check_allocation(void *ptr);

bool is_comment_or_empty(char *line);

bool is_valid_matrix_for_instruction(char *word, int lines_count);

int get_data_count_from_matrix_declaration(char *word, int line_number);

void int_to_bin(int num, char *word_p, int bit_length);

FILE *open_file(char *filename, char *mode, char *extension);

void bin_to_4base(char *src, char *dst, int length);

#endif
