#include "data.h"
#include "constants.h"
#include "symbols.h"
#include "keywords.h"
#include "error_handling.h"

void int_to_bin(int num, char *word_p, int bit_length);

FILE *open_file(char *filename, char *mode, char *extension);

void check_allocation(void *ptr) {
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


int get_data_count_from_matrix_declaration(char *word, int line_number) {
    int res, col, row;
    while (*word && *word != '[') {
        word++;
    }
    if (!*word || *word != '[') {
        return FALSE;
    }
    res = sscanf(word, "[%d][%d]", &col, &row);
    if (res != 2) {
        handle_error(INVALID_MATRIX_DECLARATION, line_number);
        return 0;
    }
    if (row <= 0 || col <= 0) {
        handle_error(NEGTIVE_INDEX_MATRIX_DECLARATION_ERROR, line_number);
        return 0;
    } else {
        return col * row;
    }
}

bool is_valid_matrix_for_instruction(char *word, int lines_count) {
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

void int_to_bin(int num, char *word_p, int bit_length) {
    unsigned int mask;
    int i;
    mask = 1;
    for (i = 0; i < bit_length; i++, word_p++) {
        *word_p = (char) (((num >> (bit_length - (i + 1))) & mask) + '0');
    }
}

FILE *open_file(char *filename, char *mode, char *extension) {
    /*returns Temp file*/
    FILE *fp = NULL;
    char *full_filename;
    full_filename = (char *) (malloc(strlen(filename) + strlen(extension) + 1));
    check_allocation(full_filename);
    /*Copy name from argv to filename string*/
    strcpy(full_filename, filename);
    /*Add .as extension to file.*/
    strcat(full_filename, extension);

    if (!(fp = fopen(full_filename, mode))) {
        printf("Can't open file %s.\n", full_filename);
        return NULL;
    }
    /*Returns pointer to file*/
    return fp;
}

void bin_to_4base(char *src, char *dst, int length) {
    int i;
    char zero;
    char cur[3];
    zero = '0';
    for (i = 0; i < length; i += 2) {
        cur[0] = (length >= (i + 2)) ? src[length - (i + 2)] : zero;
        cur[1] = src[length - (i + 1)];
        cur[2] = 0;
        if (strcmp(cur, "00") == 0) {
            dst[((length - i) / 2) - 1] = 'a';
        } else if (strcmp(cur, "01") == 0) {
            dst[((length - i) / 2) - 1] = 'b';
        } else if (strcmp(cur,"10") == 0) {
            dst[((length - i)/2) - 1] = 'c';
        } else {
            dst[((length - i)/2) - 1] = 'd';
        }
    }
    dst[length / 2] = 0;
}

char *my_strndup(char *src, size_t n){
    char *new, *new_p;
    new = (char *)(malloc(sizeof(char) * n + 1));
    new_p = new;
    while (src && n--){
        *new_p++ = *src++;
    }
    return new;
}
