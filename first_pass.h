//
// Created by Dar Bentov on 15/8/17.
//

#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#endif //ASSEMBLER_FIRST_PASS_H

#include <stdio.h>
#include "utils.h"
#include "opcode.h"


void first_pass(FILE *fp);

void process_line_first_pass(char *field, int *DC, int *IC);

void process_directive_first_pass(char *field, int is_label, int *DC);

void process_instruction_first_pass(char *field, int is_label, int *IC);

void insert_extern(char *field);
