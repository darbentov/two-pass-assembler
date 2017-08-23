#include <stdio.h>
#include "file_processing.h"
#include "opcode.h"
#include "keywords.h"

#define UASGE_MESSAGE "assembler [FILE_1_WITHOUT_AS_EXTENSION] [FILE_2_WITHOUT_AS_EXTENSION]\nUsage: assemble an assembly code. by files provided in the terminal.\n"


/*
 *
 * Two Pass Assembler
 *
 * 20/08/2017
 *
 * Written by:
 *  1. Dar Ben-Tov.
 *  2. Amit Maimon.
 *
 * Algorithm is described in the file: 'file_processing.c'
 *
 * */
int main(int argc, char **argv) {
    if (argc == 1) {
        /*if argc == 1, it means that there is no input. therefore, print usage message. */
        printf(UASGE_MESSAGE);
    }
    /* initiating internal lists of keywords and opcode */
    init_opcode_hash_table();
    init_keywords_list();

    while (--argc) {
        process_file(*++argv);
        printf("\n");
    }
    return 0;
}


