#ifndef ASSEMBLER_FILE_PROCESSING_H
#define ASSEMBLER_FILE_PROCESSING_H
#endif ASSEMBLER_FILE_PROCESSING_H


#include <stdio.h>
#include "error_handling.h"
#include "symbol_tree.h"
#include "utils.h"


#define MAX_CODE_LINE 81
#define WORD_SIZE 10
#define COMMENT_CHAR ';'
#define LABEL_SEPARATOR ':'

/*
 * Declaring prototypes
 */

void process_file(char *filename);

void first_pass(FILE *fp);

short int is_comment_or_empty(char *line);

char *strip_blank_chars(char *line);

bool find_label(char *line);

int process_line_first_pass(char *line);

/*
 * Done declaring prototypes.
 */


int IC, DC, err_count, lines_count;
char label[MAX_LABEL];
sym_pt symbol_head = NULL;

