#ifndef ASSEMBLER_DATA_H
#define ASSEMBLER_DATA_H

#include <stdio.h>
#include "utils.h"

#define MIN_NUMBER_DATA -512
#define MAX_NUMBER_DATA 511

#define QUOTATION_MARK '\"'
#define OPENING_BRACKET '['
#define CLOSING_BRACKET ']'
#define SIZE_OF_BINARY_INT 8
#define SIZE_OF_4_BASE_INT 4
#define WORD_SIZE_4_BASE 5
typedef struct Data *Data_pt;

typedef struct Data {

    unsigned int address;
    int value;
    Data_pt next;
} Data;


void insert_new_data(int *DC, short num);

void insert_single_number(char *number, int line_number, int *DC);

void insert_numbers_to_data(char *operands_line, int line_number, int *DC);

void insert_string_to_data(char *string, int line_number, int *DC);

void insert_matrix_to_data(char *operands_line, int line_number, int *DC);

void increment_data_addresses_by_ic(int DC);

void clean_data();

bool is_data_empty();

void write_data_to_ob_file(FILE *fp);

short get_data_count();

int get_count_from_matrix_declaration(char **matrix_declaration, int line_number);

#endif
