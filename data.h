#ifndef ASSEMBLER_DATA_H
#define ASSEMBLER_DATA_H
#endif //ASSEMBLER_DATA_H

#define MIN_NUMBER_DATA -16384
#define MAX_NUMBER_DATA 16383
#define STRING_NULL_TERMINATOR '\\0'
#define BAD_EXIT_STATUS 1
#define GOOD_EXIT_STATUS 0

#define QUOTATION_MARK '\"'
typedef struct Data *Data_pt;

typedef struct Data
{

    int address;
    short int value;
    Data_pt next;
} Data;


void insert_new_data(int *DC, short num);
void insert_single_number(char *number, int line_number, int *DC);
int insert_numbers_to_data(char *token, int line_number, int *DC);
int insert_string_to_data(char *string, int line_number, int *DC);
int insert_matrix_to_data(char *token, int line_number, int *DC);
void increment_data_addresses_by_ic(int DC);
void clean_data();