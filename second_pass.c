#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "second_pass.h"
#include "symbols.h"
#include "constants.h"
#include "directives.h"
#include "error_handling.h"
#include "entry_table.h"
#include "opcode.h"

extern int err_count;
int lines_count;

void process_line_second_pass(char *token, int *IC_pt);

void process_directive_second_pass(char *token);

void process_instruction_second_pass(char *token, int *IC_pt);

char label[MAX_LABEL];

/*
 * Second Pass Algorithm
 * 1.  IC -> 0
 * 2.  Read line, if EOF: move to step 10.
 * 3.  Check if first field of line is a label, if yes: skip it.
 * 4.  Check if current field is a one of the following directives: .data / .mat / .string / .extern.
 *     if yes: go back to step 2.
 *     if not: move to step 5
 * 5.  Check if current field is .entry.
 *     if yes: move to step 6.
 *     if not: move to step 7.
 * 6.  check for label in the symbol table that match the operand of the .entry directive.
 *     Assign the address of the symbol to the entry.
 *     Go back to step 2.
 * 7.  This is an instruction line -> Complete the coding of the operands of the instruction.
 *     And calculate the words length (L).
 * 8.  IC = IC + L.
 * 9.  Go back to step 2.
 * 10. If errors found, stop.
 *
 * Done!
 */

void second_pass(FILE *fp){

    char line[MAX_CODE_LINE];
    char *line_p;
    char *token;
    int IC;
    IC = 0;
    while (fgets(line, MAX_CODE_LINE, fp)) {
        lines_count++;
        line_p = line;
        printf("Working on line: %s", line_p);
        while (isspace(*line_p)) {
            line_p++;
        }
        token = strtok(line_p, BLANK_CHARACTER_SEPERATOR);
        if (!token || is_comment_or_empty(token))
            continue;
        process_line_second_pass(token, &IC);
    }
}

void process_line_second_pass(char *token, int *IC_pt) {
    if (get_label(token,lines_count)){
        token = strtok(NULL, BLANK_CHARACTER_SEPERATOR);
    }
    if (*token == '.'){
        token++;
        process_directive_second_pass(token);
    }
    else {
        printf("Second pass: found instruction. current IC: %d\n", *IC_pt);
        process_instruction_second_pass(token, IC_pt);
    }
}

void process_instruction_second_pass(char *token, int *IC_pt) {

    opcode_pt cur_opcode;
    int words_count;
    char *target_operand;
    char *source_operand;
    addressing_t source_addressing, target_addressing;
    cur_opcode = get_opcode(token);
    source_addressing = NO_ADDRESSING;
    source_operand = NULL;
    words_count = 1;
    if (cur_opcode->target_addressing_types) {
        if (cur_opcode->source_addressing_types) {
            source_operand = strtok(NULL, " \n,");
            source_addressing = get_addressing_and_validate(source_operand, cur_opcode->source_addressing_types, lines_count);

        }
        target_operand = strtok(NULL, " \n,");
        target_addressing = get_addressing_and_validate(target_operand, cur_opcode->target_addressing_types, lines_count);
        printf("source addressing: %d\n", source_addressing);
        printf("target addressing: %d\n", target_addressing);
        build_code_lines(cur_opcode, source_addressing, source_operand, target_addressing, target_operand, lines_count, *IC_pt);
    }

    *IC_pt += words_count;

}

void process_directive_second_pass(char *token) {
    int directive_type;
    sym_pt symbol;
    directive_type = find_directive_type(token);
    token = strtok(NULL, BLANK_CHARACTER_SEPERATOR);

    if (directive_type != ENTRY_DIRECTIVE_TYPE) {
        return;
    }
    symbol = search_symbol_by_label(token);
    if (!symbol){
        handle_error(ENTRY_OPERAND_LABEL_DOES_NOT_EXIST_ERROR, lines_count);
        return;
    }

    insert_entry(symbol->label, symbol->address);

}
