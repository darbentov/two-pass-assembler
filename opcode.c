#include "opcode.h"
#include "error_handling.h"
#include "constants.h"
#include "keywords.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

static opcode_pt hash_table[OPCODES_COUNT] = {NULL};
static char *instructions[256];

allowed_addressing_bitfields g1 = {1, 1, 1, 1};
allowed_addressing_bitfields g2 = {0, 1, 1, 1};
allowed_addressing_bitfields g3 = {0, 1, 1, 0};

Opcode mov = {"mov", "0000", &g1, &g2};
Opcode cmp = {"cmp", "0001", &g1, &g1};
Opcode add = {"add", "0010", &g1, &g2};
Opcode sub = {"sub", "0011", &g1, &g2};
Opcode not = {"not", "0100", NULL, &g2};
Opcode clr = {"clr", "0101", NULL, &g2};
Opcode lea = {"lea", "0110", &g3, &g2};
Opcode inc = {"inc", "0111", NULL, &g2};
Opcode dec = {"dec", "1000", NULL, &g2};
Opcode jmp = {"jmp", "1001", NULL, &g2};
Opcode bne = {"bne", "1010", NULL, &g2};
Opcode red = {"red", "1011", NULL, &g2};
Opcode prn = {"prn", "1100", NULL, &g1};
Opcode jsr = {"jsr", "1101", NULL, &g2};
Opcode rts = {"rts", "1110", NULL, NULL};
Opcode stop = {"stop", "1111", NULL, NULL};

void build_instruction_line(opcode_pt cur_opcode, addressing_t source_addressing, addressing_t target_addressing,
                            int IC);

void build_registers_instruction_line(char *source_register, char *target_register, int i);

int
build_operand_instruction_line(addressing_t addressing, char *operand, bool is_target_operand, int IC, int lines_count);

void int_to_bin(int num, char *word_p, int bit_length);

void build_value_instruction_code(int value, coding_type_enum coding_type, int IC);

void build_matrix_instruction_code(char *operand, int IC);

coding_type_enum get_coding_type_from_symbol(sym_pt symbol);

int get_opcode_hash_index(char *name) {
    int sum;
    char c;
    sum = 0;
    while ((c = *(name++))) {
        sum += c;
    }
    return sum % OPCODES_COUNT;
}

void init_opcode_hash_table() {
    insert_opcode_into_hash_table(&mov);
    insert_opcode_into_hash_table(&cmp);
    insert_opcode_into_hash_table(&add);
    insert_opcode_into_hash_table(&sub);
    insert_opcode_into_hash_table(&not);
    insert_opcode_into_hash_table(&clr);
    insert_opcode_into_hash_table(&lea);
    insert_opcode_into_hash_table(&inc);
    insert_opcode_into_hash_table(&dec);
    insert_opcode_into_hash_table(&jmp);
    insert_opcode_into_hash_table(&bne);
    insert_opcode_into_hash_table(&red);
    insert_opcode_into_hash_table(&prn);
    insert_opcode_into_hash_table(&jsr);
    insert_opcode_into_hash_table(&rts);
    insert_opcode_into_hash_table(&stop);
}


void insert_opcode_into_hash_table(opcode_pt node) {
    int index = get_opcode_hash_index(node->name);
    if (!hash_table[index]) {
        hash_table[index] = node;
        node->next = NULL;
    } else {
        opcode_pt temp = hash_table[index];
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = node;
        node->next = NULL;
    }
}


opcode_pt get_opcode(char *op_name) {
    opcode_pt temp = NULL;
    int index = get_opcode_hash_index(op_name);
    if (hash_table[index]) {
        temp = hash_table[index];
        while (strcmp(temp->name, op_name) && temp) {
            temp = temp->next;
        }
    }

    return temp;
}


addressing_t
get_addressing_and_validate(char *operand, allowed_addressing_bitfields *allowed_addressings, int lines_count) {
    addressing_t addressing;
    addressing = get_addressing_by_operand(operand, lines_count);
    if (is_addressing_in_allowed_addressings(addressing, allowed_addressings)) {
        return addressing;
    }
    handle_error(ADDRESSING_NOT_ALLOWED_ERROR, lines_count);
    return NO_ADDRESSING;
}

addressing_t get_addressing_by_operand(char *operand, int lines_count) {
    if (*operand == '#') {
        char *ptr;
        long num;
        num = strtol(operand, &ptr, DECIMAL_BASE);
        if (!*ptr) {
            return IMMEDIATE_ADDRESSING;
        } else {
            handle_error(INVALID_NUMBER_ERROR, lines_count);
            return NO_ADDRESSING;
        }
    } else if (isalpha(*operand)) {
        if (is_register(operand)) {
            return REGISTER_ADDRESSING;
        } else if (is_matrix(operand)) {
            return MATRIX_ADRESSING;
        } else if (label_is_valid(operand, strlen(operand), lines_count)) {
            return DIRECT_ADDRESSING;
        } else {
            return NO_ADDRESSING;
        }

    } else {
        handle_error(INVALID_INSTRUCTION_OPERAND_ERROR, lines_count);
        return NO_ADDRESSING;
    }
}

int get_words_count_by_single_addressing(addressing_t addressing) {
    if (addressing == NO_ADDRESSING) {
        return 0;
    } else if (addressing == MATRIX_ADRESSING) {
        return 2;
    } else {
        return 1;
    }
}

int get_words_count_by_both_addressings(addressing_t source_addressing, addressing_t target_addressing) {
    if (source_addressing == target_addressing == REGISTER_ADDRESSING) {
        return 1;
    } else {
        return get_words_count_by_single_addressing(source_addressing) +
               get_words_count_by_single_addressing(target_addressing);
    }
}

void build_code_lines(opcode_pt cur_opcode, addressing_t source_addressing, char *source_operand,
                      addressing_t target_addressing, char *target_operand, int lines_count, int IC) {
    int words_count;
    words_count = 1;
    build_instruction_line(cur_opcode, source_addressing, target_addressing, IC);
    if (source_addressing == target_addressing == REGISTER_ADDRESSING) {
        build_registers_instruction_line(source_operand, target_operand, IC + words_count);
    } else {
        words_count += build_operand_instruction_line(source_addressing, source_operand, FALSE, IC + words_count,
                                                      lines_count);
        build_operand_instruction_line(target_addressing, target_operand, TRUE, IC + words_count, lines_count);
    }

}

int build_operand_instruction_line(addressing_t addressing, char *operand, bool is_target_operand, int IC,
                                   int lines_count) {
    int value;
    sym_pt symbol;
    coding_type_enum coding_type;
    coding_type = ABSOLUTE_CODING_TYPE;
    char *ptr;
    if (addressing == NO_ADDRESSING) {
        return 0;
    } else if (addressing == MATRIX_ADRESSING) {
        build_matrix_instruction_code(operand, IC);
        return 2;
    } else if (addressing == REGISTER_ADDRESSING) {
        if (is_target_operand) {
            build_registers_instruction_line(NULL, operand, IC);
        } else {
            build_registers_instruction_line(operand, NULL, IC);
        }

        return 1;
    } else {
        if (addressing == IMMEDIATE_ADDRESSING) {
            operand++;
            value = (int) strtol(operand, &ptr, DECIMAL_BASE);
        } else {
            symbol = search_symbol_by_label(operand);
            if (!symbol) {
                handle_error(LABEL_DOES_NOT_EXIST_ERROR, lines_count);
                value = 0;
            } else {
                value = symbol->address;
                coding_type = get_coding_type_from_symbol(symbol);
            }


        }
        build_value_instruction_code(value, coding_type, IC);
        return 1;
    }
}

coding_type_enum get_coding_type_from_symbol(sym_pt symbol) {
    coding_type_enum coding_type;
    if (symbol->is_external) {
        coding_type = EXTERNAL_CODING_TYPE;
    } else {
        coding_type = RELOCATABLE_CODING_TYPE;
    }
    return coding_type;
}

void build_matrix_instruction_code(char *operand, int IC) {
    char *mat_name;
    char *row_register;
    char *col_register;
    sscanf(operand, "%s[%s][%s]", mat_name, row_register, col_register);
    sym_pt symbol;
    symbol = search_symbol_by_label(mat_name);
    coding_type_enum coding_type;
    coding_type = get_coding_type_from_symbol(symbol);
    build_value_instruction_code(symbol->address, coding_type, IC);
    build_registers_instruction_line(row_register, col_register, IC + 1);
}

void build_value_instruction_code(int value, coding_type_enum coding_type, int IC) {
    char word[11];
    char *word_p;
    word_p = word;
    int_to_bin(value, word_p, 8);
    word_p += 8;
    int_to_bin(coding_type, word_p, 2);
    word_p += 2;
    *word_p = '\0';
    instructions[IC] = strdup(word);

}

void build_registers_instruction_line(char *source_register, char *target_register, int IC) {
    char word[11];
    char *word_p;
    word_p = word;
    int source_register_number, target_register_number;
    source_register_number = source_register ? (short int) (source_register[1] - '0') : (short int) 0;
    target_register_number = target_register ? (short int) (target_register[1] - '0') : (short int) 0;
    int_to_bin(source_register_number, word_p, 4);
    word_p += 4;
    int_to_bin(target_register_number, word_p, 4);
    word_p += 4;
    int_to_bin(ABSOLUTE_CODING_TYPE, word_p, 2);
    word_p += 2;
    *word_p = '\0';
    instructions[IC] = strdup(word);

}

void int_to_bin(int num, char *word_p, int bit_length) {
    unsigned int mask;
    int i;
    mask = 1;
    for (i = 0; i < bit_length; i++, word_p++) {
        *word_p = (char) (((num >> (bit_length - (i + 1))) & mask) + '0');
    }
}

void build_instruction_line(opcode_pt cur_opcode, addressing_t source_addressing, addressing_t target_addressing,
                            int IC) {
    char word[11];
    char *word_p;
    word_p = word;
    strcpy(word_p, cur_opcode->code);
    word_p += 4;
    int_to_bin(source_addressing, word_p, 2);
    word_p += 2;
    int_to_bin(target_addressing, word_p, 2);
    word_p += 2;
    int_to_bin(ABSOLUTE_CODING_TYPE, word_p, 2);
    word_p += 2;
    *word_p = '\0';
    instructions[IC] = strdup(word);

}
