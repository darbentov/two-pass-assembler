#include "data.h"
#include "opcode.h"
#include "error_handling.h"
#include "constants.h"
#include "keywords.h"
#include "extern_table.h"
#include <ctype.h>


#define DIMENSION_FOUND -1

bool is_matrix_declaration_valid_for_instruction(char *matrix_declaration, int line_number);

char find_dimension_or_bracket(char **matrix_declaration);


static opcode_pt hash_table[OPCODES_COUNT] = {NULL};
static char *instructions[MAX_INSTRUCTIONS_LINES];
static short int instructions_count;

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
        while (temp && strcmp(temp->name, op_name)) {
            temp = temp->next;
        }
    }

    return temp;
}


addressing_t get_addressing_and_validate(char *operand, allowed_addressing_bitfields *allowed_addressings,
                                         int lines_count) {
    addressing_t addressing;
    addressing = get_addressing_by_operand(operand, lines_count);
    if (is_addressing_in_allowed_addressings(addressing, allowed_addressings)) {
        return addressing;
    }
    if (addressing != NO_ADDRESSING) {
        handle_error(ADDRESSING_NOT_ALLOWED_ERROR, lines_count);
    }
    return NO_ADDRESSING;
}

addressing_t get_addressing_by_operand(char *operand, int lines_count) {
    bool operand_check_result;
    if (*operand == IMMIDIATE_NUMBER_INDICATOR) {
        char *ptr;
        long num;
        operand++;
        num = strtol(operand, &ptr, DECIMAL_BASE);
        if (*ptr) {
            handle_error(INVALID_NUMBER_ERROR, lines_count);
            return NO_ADDRESSING;
        } else if (num > MAX_IMMIDIATE_NUM || num < MIN_IMMIDIATE_NUM) {
            handle_error(NUMBER_OUT_OF_RANGE_ERROR, lines_count);
            return NO_ADDRESSING;
        } else {
            return IMMEDIATE_ADDRESSING;
        }
    } else if (isalpha(*operand)) {
        if (is_register(operand)) {
            return REGISTER_ADDRESSING;
        } else if ((operand_check_result = is_matrix_declaration_valid_for_instruction(operand, lines_count))) {
            if (operand_check_result == FATAL_FALSE){
                return NO_ADDRESSING;
            }
            if (are_matrix_dimensions_registers(operand, lines_count)) {
                return NO_ADDRESSING;
            }
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
        return MATRIX_ADDRESSING_WORDS_COUNT;
    } else {
        return 1;
    }
}

int get_words_count_by_both_addressings(addressing_t source_addressing, addressing_t target_addressing) {
    if (source_addressing == target_addressing && source_addressing == REGISTER_ADDRESSING) {
        return BOTH_REGISTERS_ADDRESSING_WORDS_COUNT;
    } else {
        return get_words_count_by_single_addressing(source_addressing) +
               get_words_count_by_single_addressing(target_addressing);
    }
}

int build_code_lines(opcode_pt cur_opcode, addressing_t source_addressing, char *source_operand,
                     addressing_t target_addressing, char *target_operand, int lines_count, int IC) {
    int words_count;
    words_count = 1;
    build_instruction_line(cur_opcode, source_addressing, target_addressing, IC);
    if (source_addressing == target_addressing && source_addressing == REGISTER_ADDRESSING) {
        build_registers_instruction_line(source_operand, target_operand, IC + words_count);
    } else {
        words_count += build_operand_instruction_line(source_addressing, source_operand, FALSE, IC + words_count,
                                                      lines_count);
        words_count += build_operand_instruction_line(target_addressing, target_operand, TRUE, IC + words_count,
                                                      lines_count);
    }
    return words_count;

}

int build_operand_instruction_line(addressing_t addressing, char *operand, bool is_target_operand, int IC,
                                   int lines_count) {
    sym_pt symbol;
    int value;
    char *ptr;

    coding_type_enum coding_type;
    coding_type = ABSOLUTE_CODING_TYPE;

    if (addressing == NO_ADDRESSING) {
        return 0;
    } else if (addressing == MATRIX_ADRESSING) {
        build_matrix_instruction_code(operand, IC, lines_count);
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
                if (coding_type == EXTERNAL_CODING_TYPE) {
                    insert_extern(symbol->label, IC + START_IC);
                }
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

void build_matrix_instruction_code(char *operand, int IC, int line_number) {
    char *word_p, *mat_name;
    char row_register[REGISTER_NAME_LENGTH + 1];
    char col_register[REGISTER_NAME_LENGTH + 1];
    sym_pt symbol;
    size_t i;
    word_p = operand;
    i = 0;
    while (*word_p && *word_p != '[') {
        i++;
        word_p++;
    }

    mat_name = my_strndup(operand, i);
    operand += i;
    sscanf(operand, SCANF_MATRIX_PATTERN, row_register, col_register);
    symbol = search_symbol_by_label(mat_name);
    if (!symbol) {
        handle_error(LABEL_DOES_NOT_EXIST_ERROR, line_number);
        return;
    }
    coding_type_enum coding_type;
    coding_type = get_coding_type_from_symbol(symbol);
    build_value_instruction_code(symbol->address, coding_type, IC);
    build_registers_instruction_line(row_register, col_register, IC + 1);
}

void build_value_instruction_code(int value, coding_type_enum coding_type, int IC) {
    char word[WORD_SIZE + 1];
    char *word_p;
    word_p = word;
    int_to_bin(value, word_p, 8);
    word_p += 8;
    int_to_bin(coding_type, word_p, 2);
    word_p += 2;
    *word_p = '\0';
    insert_instruction(IC, word);

}

void insert_instruction(int IC, const char *word) {
    instructions[IC] = strdup(word);
    instructions_count++;
}

void build_registers_instruction_line(char *source_register, char *target_register, int IC) {
    char word[WORD_SIZE + 1];
    char *word_p;
    int source_register_number, target_register_number;
    word_p = word;
    source_register_number = get_register_number(source_register);
    target_register_number = get_register_number(target_register);
    int_to_bin(source_register_number, word_p, REGISTER_BIT_SIZE);
    word_p += REGISTER_BIT_SIZE;
    int_to_bin(target_register_number, word_p, REGISTER_BIT_SIZE);
    word_p += REGISTER_BIT_SIZE;
    int_to_bin(ABSOLUTE_CODING_TYPE, word_p, 2);
    word_p += CODING_TYPE_BIT_SIZE;
    *word_p = '\0';
    insert_instruction(IC, word);

}

short int get_register_number(char *register_) {
    return register_ ? (short int) (register_[1] - '0') : (short int) 0;
}

void build_instruction_line(opcode_pt cur_opcode, addressing_t source_addressing, addressing_t target_addressing,
                            int IC) {
    char word[WORD_SIZE + 1];
    char *word_p;
    word_p = word;
    strcpy(word_p, cur_opcode->code);
    word_p += OPCODE_BITS_SIZE;
    int_to_bin(source_addressing, word_p, ADDRESSING_BIT_SIZE);
    word_p += ADDRESSING_BIT_SIZE;
    int_to_bin(target_addressing, word_p, ADDRESSING_BIT_SIZE);
    word_p += ADDRESSING_BIT_SIZE;
    int_to_bin(ABSOLUTE_CODING_TYPE, word_p, CODING_TYPE_BIT_SIZE);
    word_p += CODING_TYPE_BIT_SIZE;
    *word_p = '\0';
    insert_instruction(IC, word);

}

void clean_code() {
    int i = 0;
    char *word;
    for (i = 0; i < MAX_INSTRUCTIONS_LINES; i++) {
        word = instructions[i];
        free(word);
        instructions[i] = NULL;
    }
    instructions_count = 0;
}

bool is_addressing_in_allowed_addressings(addressing_t addressing, allowed_addressing_bitfields *allowed_addressings) {
    return (bool) ((addressing == IMMEDIATE_ADDRESSING && allowed_addressings->immediate_addressing) ||
                   (addressing == DIRECT_ADDRESSING && allowed_addressings->direct_addressing) ||
                   (addressing == REGISTER_ADDRESSING && allowed_addressings->register_addressing) ||
                   (addressing == MATRIX_ADRESSING && allowed_addressings->matrix_addressing));
}

bool is_code_empty() {
    return (bool) (instructions_count == 0);
}

void write_code_to_ob_file(FILE *fp) {
    int i;
    char address[BITS_COUNT_OF_AN_INT + 1];
    char address_4_base[(BITS_COUNT_OF_AN_INT / 2) + 1];
    char value_4_base[(WORD_SIZE / 2) + 1];
    for (i = 0; i < MAX_INSTRUCTIONS_LINES; i++) {

        if (!instructions[i]) {
            break;
        }
        int_to_bin(i + START_IC, address, BITS_COUNT_OF_AN_INT);
        bin_to_4base(address, address_4_base, BITS_COUNT_OF_AN_INT);
        bin_to_4base(instructions[i], value_4_base, WORD_SIZE);
        fputs(address_4_base, fp);
        fputc(TAB_SEPERATOR, fp);
        fputs(value_4_base, fp);
        fputc(LINE_BREAK, fp);
    }
}

short int get_insrtuctions_count() {
    return instructions_count;
}

/*checks if matrix is valid*/
bool are_matrix_dimensions_registers(char *word, int lines_count) {
    int res;
    char row_register[REGISTER_NAME_LENGTH + 1];
    char col_register[REGISTER_NAME_LENGTH + 1];
    while (*word && *word != '[') {
        word++;
    }
    if (!*word || *word != '[') {
        return FALSE;
    }
    res = sscanf(word, SCANF_MATRIX_PATTERN, row_register, col_register);

    if (res == 2) {
        if (is_register(row_register) && is_register(col_register)) {
            return TRUE;
        } else {
            handle_error(MATRIX_INDEX_MUST_BE_REGISTERS_ERROR, lines_count);
            return FALSE;
        }

    }
    return FALSE;
}


/* Get count from matrix declaration
 *
 * Iterating the string of matrix declaration and find the pattern of '[row][col]'
 *
 * Rules:
 * 1. Only alnum character are allowed between brackets.
 * 2. No spaces are allowed
 * */
bool is_matrix_declaration_valid_for_instruction(char *matrix_declaration, int line_number) {
    int status;
    char c;
    status = BEGINING;

    while (*matrix_declaration && *matrix_declaration != '[') {
        matrix_declaration++;
    }
    if (!*matrix_declaration || *matrix_declaration != '[') {
        return FALSE;
    }
    /* getting operands in a loop
     *
     * Expected operands:
     * 1. '['
     * 2. ']'
     * 3. a register
     * */
    while ((c = find_dimension_or_bracket(&matrix_declaration))) {
        switch (c) {
            case OPENING_BRACKET:
                if (status == BEGINING) {
                    /* the scan got the first '[' character */
                    status = AFTER_FIRST_OPENING_BRACKET;
                } else if (status == AFTER_FIRST_CLOSING_BRACKET) {
                    /* the scan got the second '[' character */
                    status = AFTER_SECOND_OPENING_BRACKET;

                } else {
                    handle_error(INVALID_MATRIX_DECLARATION, line_number);
                    return FATAL_FALSE;
                }
                break;
            case CLOSING_BRACKET:
                if (status == AFTER_ROW_DIMENSION) {
                    status = AFTER_FIRST_CLOSING_BRACKET;
                    /* spaces are not allowed between the row and col declaration */
                } else if (status == AFTER_COL_DIMENSION) {
                    status = AFTER_SECOND_CLOSING_BRACKET;
                    /* after the scan got both dimensions, set the 'is_skip_spaces' to FALSE,
                     * to complete the scan of the matrix dimensions */
                } else {
                    /* if closing bracket is not after a number, it is an error */
                    handle_error(INVALID_MATRIX_DECLARATION, line_number);
                    return FATAL_FALSE;
                }
                break;
            case DIMENSION_FOUND:
                /* if a number was found */
                if (status == AFTER_FIRST_OPENING_BRACKET) {
                    status = AFTER_ROW_DIMENSION;
                } else if (status == AFTER_SECOND_OPENING_BRACKET) {
                    status = AFTER_COL_DIMENSION;
                } else {
                    /* if a number found not after an opening bracket, is it an error */
                    handle_error(SYNTAX_ERROR, line_number);
                    return FATAL_FALSE;
                }
                break;
            default:
                /* if the scanner found unexpected characters, is it an error */
                handle_error(SYNTAX_ERROR, line_number);
                return FATAL_FALSE;
        }
    }
    if (status == AFTER_ROW_DIMENSION || status == AFTER_COL_DIMENSION){
        handle_error(SYNTAX_ERROR, line_number);
        return FATAL_FALSE;
    }
    /* if the scanner has not got to the second closing bracket, it means that the matrix declaration is not valid */
    if (status != AFTER_SECOND_CLOSING_BRACKET) {
        handle_error(MISSING_COLUMN_DIMENSION, line_number);
        return FATAL_FALSE;
    }
    return TRUE;
}

char find_dimension_or_bracket(char **matrix_declaration) {
    char c;
    c = **matrix_declaration; /* get first character */
    *matrix_declaration+=1;

    /* if c is not an alphanum character, it is sure not the beginning of a register name */
    if (!isalnum(c)) {
        return c;
    }
    /* go back to the last character, so we can scan the whole dimension */
    *matrix_declaration-=1;

    /* skipping the whole dimension */
    while (isalnum(c)){
        *matrix_declaration+=1;
        c = **matrix_declaration;

    }
    return DIMENSION_FOUND;

}
