#include <stdio.h>
#include "utils.h"
#include "symbols.h"

#ifndef ASSEMBLER_OPCODE_H
#define ASSEMBLER_OPCODE_H


#define OPCODES_COUNT 16
#define MAX_OPCODE_NAME 10
#define MAX_OPCODE_BITS 4
#define MAX_IMMIDIATE_NUM 127
#define MIN_IMMIDIATE_NUM -128
#define MAX_INSTRUCTIONS_LINES 256

#define SCANF_MATRIX_PATTERN "[%[^]]][ %[^]]]"
#define MATRIX_ADDRESSING_WORDS_COUNT 2
#define BOTH_REGISTERS_ADDRESSING_WORDS_COUNT 1
#define IMMIDIATE_NUMBER_INDICATOR '#'

typedef enum {
    IMMEDIATE_ADDRESSING, DIRECT_ADDRESSING, MATRIX_ADRESSING, REGISTER_ADDRESSING, NO_ADDRESSING
} addressing_t;

typedef enum {
    ABSOLUTE_CODING_TYPE, EXTERNAL_CODING_TYPE, RELOCATABLE_CODING_TYPE
} coding_type_enum;

typedef struct addressing_types {
    unsigned int immediate_addressing:1;
    unsigned int direct_addressing:1;
    unsigned int matrix_addressing:1;
    unsigned int register_addressing:1;
} allowed_addressing_bitfields;


typedef struct Opcode *opcode_pt;

typedef struct Opcode {             /* Opcode node. it will be in an hash list, and opcodes with the same hash will be in a linked list */
    char name[MAX_OPCODE_NAME];
    char code[MAX_OPCODE_BITS];
    allowed_addressing_bitfields *source_addressing_types;
    allowed_addressing_bitfields *target_addressing_types;
    opcode_pt next;

} Opcode;

opcode_pt get_opcode(char *op_name);

int get_opcode_hash_index(char *name);

void init_opcode_hash_table();

void insert_opcode_into_hash_table(Opcode *Node);

addressing_t
get_addressing_and_validate(char *operand, allowed_addressing_bitfields *allowed_addressings, int lines_count);

addressing_t get_addressing_by_operand(char *operand, int lines_count);

bool is_addressing_in_allowed_addressings(addressing_t addressing, allowed_addressing_bitfields *allowed_addressings);

int get_words_count_by_both_addressings(addressing_t source_addressing, addressing_t target_addressing);

int build_code_lines(opcode_pt cur_opcode, addressing_t source_addressing, char *source_operand,
                     addressing_t target_addressing, char *target_operand, int lines_count, int IC);


void clean_code();

bool is_code_empty();

void write_code_to_ob_file(FILE *fp);

short int get_insrtuctions_count();

void build_instruction_line(opcode_pt cur_opcode, addressing_t source_addressing, addressing_t target_addressing,
                            int IC);

void build_registers_instruction_line(char *source_register, char *target_register, int i);

int
build_operand_instruction_line(addressing_t addressing, char *operand, bool is_target_operand, int IC, int lines_count);

void build_value_instruction_code(int value, coding_type_enum coding_type, int IC);

void build_matrix_instruction_code(char *operand, int IC, int line_number);

coding_type_enum get_coding_type_from_symbol(sym_pt symbol);

void insert_instruction(int IC, const char *word);

short int get_register_number(char *register_);

#endif /* ASSEMBLER_OPCODE_H */
