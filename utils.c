#include "constants.h"
#include "symbols.h"
#include "keywords.h"
#include "error_handling.h"
#include <stdio.h>
#include <stdlib.h>

void int_to_bin(int num, char *word_p, int bit_length);

void check_allocation(void *ptr)
{
    if (!ptr) {
        printf("Memory error. aborting...\n");
        exit(0);
    }
}

bool is_comment_or_empty(char *line) {
    char c;
    c = *line;
    if (!c || c == COMMENT_CHAR)
        return TRUE;
    return FALSE;
}


int get_data_count_from_matrix_declaration(char *word, int line_number){
    while (*word && *word != '['){
        word++;
    }
    if (!*word || *word != '['){
        return FALSE;
    }
    int res, col, row;
    res = sscanf(word, "[%d][%d]", &col, &row);
    printf("Res: %d\n", res);
    printf("Row: %d\n", row);
    printf("Col: %d\n", col);
    if (res != 2){
        handle_error(INVALID_MATRIX_DECLARATION, line_number);
        return 0;
    }
    else {
        return col * row;
    }
}

bool is_valid_matrix_for_instruction(char *word, int lines_count) {
    while (*word && *word != '['){
        word++;
    }
    if (!*word || *word != '['){
        return FALSE;
    }
    int res;
    char row_register[40];
    char col_register[40];
    printf("Word: %s\n", word);
    res = sscanf(word, "[%[^]]][%[^]]]", row_register, col_register);

    if (res == 2){
        if (is_register(row_register) && is_register(col_register)){
            return TRUE;
        }
        else {
            handle_error(MATRIX_INDEX_MUST_BE_REGISTERS_ERROR, lines_count);
            return FALSE;
        }

    }
    return FALSE;
}

void int_to_bin(int num, char *word_p, int bit_length) {
    unsigned int mask;
    int i;
    mask = 1;
    for (i = 0; i < bit_length; i++, word_p++) {
        *word_p = (char) (((num >> (bit_length - (i + 1))) & mask) + '0');
    }
}