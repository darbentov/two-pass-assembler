#include "data.h"
#include "constants.h"
#include "symbols.h"
#include "keywords.h"
#include "error_handling.h"
#include <stdio.h>
#include <stdlib.h>

void int_to_bin(int num, char *word_p, int bit_length);

FILE * open_file(char* filename, char * mode, char * extension);

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
    if (res != 2){
        handle_error(INVALID_MATRIX_DECLARATION, line_number);
        return 0;
    }
    if (row <= 0 || col <=0){
        handle_error(NEGTIVE_INDEX_MATRIX_DECLARATION_ERROR, line_number);
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

FILE * open_file(char* filename, char * mode, char * extension)
{
    /*Temp file to be returned*/
    FILE * fp = NULL;
    /*Full filename string*/
    char *full_filename;
    full_filename = (char *)(malloc(strlen(filename) + strlen(extension) + 1));
    check_allocation(full_filename);
    /*Copy the filename from argv to the full filename string*/
    strcpy(full_filename, filename);
    /*Add the .as extension to the full filename.*/
    strcat(full_filename, extension);
    /*Open the file. if fopen return null, exit the function and print error to screen*/
    if (!(fp = fopen(full_filename, mode)))
    {
        printf("Can't open file %s.\n", full_filename);
        return NULL;
    }
    /*Return pointer to the file*/
    return fp;
}

void bin_to_4base(char *src, char *dst, int length){
    int i;
    char zero;
    char cur[3];
    zero = '0';
    for (i=0; i < length; i+=2){
        cur[0] = (length >= (i + 2)) ? src[length - (i + 2)] : zero;
        cur[1] = src[length - (i + 1)];
        cur[2] = 0;
        if (strcmp(cur, "00") == 0){
            dst[((length - i) / 2) - 1] = 'a';
        }
        else if (strcmp(cur, "01") == 0){
            dst[((length - i) / 2) - 1] = 'b';
        }
        else if (strcmp(cur, "10") == 0){
            dst[((length - i) / 2) - 1] = 'c';
        }
        else {
            dst[((length - i) / 2) - 1] = 'd';
        }
    }
    dst[length / 2] = 0;
}