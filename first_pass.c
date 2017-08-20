#include <ctype.h>
#include "file_processing.h"
#include "first_pass.h"
#include "keywords.h"
#include "data.h"
#include "directives.h"
#include "constants.h"
#include "error_handling.h"

extern int err_count;
int lines_count_first_pass = 0;
char *label;


addressing_t get_addressing_and_validate(char *operand, allowed_addressing_bitfields *allowed_addressings, int lines_count);

/*
 * First Pass Algorithm
 * 1.  IC -> 0, DC -> 0.
 * 2.  Read line, if EOF: move to step 16.
 * 3.  Check if first field of line is a label, if yes: move to step 4, if not: move to step 5.
 * 4.  turn on 'is_label' flag.
 * 5.  Check if current field is a directive for storing data (.data / .mat / .string)?
 *     if yes: move to step 6, if not: move to step 8.
 *
 *
 ****** Dealing with a directive ***********
 *
 *
 * 6.  If 'is_label' flag is turned on, insert label to Symbol table as a data Symbol
 *     and insert the current value of DC as the address.
 * 7.  verify type of data, insert into data memory and update DC by the words count. Go back to step 2.
 * 8.  Is this a .extern or .entry? If yes: move to step 9, if not: move to step 11.
 * 9.  Is this a .extern? if yes, insert the label (operand of .extern directive) to the Symbol table with flag 'is_external'.
 * 10. Go back to step 2
 *
 *
 ****** Dealing with an instruction ***********
 *
 *
 * 11. If 'is_label' flag is turned on, insert label to Symbol table as a data Symbol
 *     and insert the current value of IC as the address.
 * 12. Look for the instruction name in the Instructions table.
 * 13. Check operands of the instruction and calculate the words count.
 *     build the code of the first word (The instruction itself).
 * 14. update IC by the words count.
 * 15. Go back to step 2.
 *
 *
 *************** Wrapping up *****************
 *
 *
 * 16. If errors were found, stop the processing of the file.
 * 17. Update the addresses of the Symbol which refers to data, by adding the value of IC.
 * 18. Start second pass.
 *
 * Done!
 */



void first_pass(FILE *fp) {

    char line[MAX_CODE_LINE];
    char *line_p;
    char *token;
    int IC, DC;
    IC = DC = 0;
    lines_count_first_pass = 0;
    while (fgets(line, MAX_CODE_LINE, fp)) {
        lines_count_first_pass++;
        line_p = line;
        while (isspace(*line_p)) {
            line_p++;
        }
        token = strtok(line_p, BLANK_CHARACTER_SEPERATOR);
        if (!token || is_comment_or_empty(token))
            continue;
        process_line_first_pass(token, &DC, &IC);
    }
    increment_data_addresses_by_ic(IC);
    increment_symbol_addresses_by_ic(IC);
}

void process_line_first_pass(char *field, int *DC, int *IC) {
    {
        bool is_label;
        is_label = FALSE;
        if (!field)
            return;

        /*___________________________________*/


        /* first character can either be a dot or a letter. */
        if (!(isalpha(*field) || *field == '.')) {
            handle_error(SYNTAX_ERROR, lines_count_first_pass);
            return;
        }
        /*************** Dealing with Label *******************/


        label = get_label(field, lines_count_first_pass);
        if (label) {
            field = strtok(NULL, BLANK_CHARACTER_SEPERATOR);
            if (search_symbol_by_label(label)){
                handle_error(LABEL_ALREADY_EXISTS_ERROR, lines_count_first_pass);
                is_label = FALSE;
            }
            else {
                is_label = TRUE;
                if (!field) {
                    handle_error(LABEL_WITH_NO_DATA_ERROR, lines_count_first_pass);
                    return;
                }
            }
        }

        if (*field == '.') { /* means that this is a direcive */
            field++; /* skip '.' */
            process_directive_first_pass(field, is_label, DC);
        } else { /* if not a directive, then it is instruction */
            process_instruction_first_pass(field, is_label, IC);
        }
    }
}

void insert_extern_to_symbol_table(char *label){
    if (label_is_valid(label, strlen(label), lines_count_first_pass)){
        sym_pt symbol_node = create_symbol_node(label, 0, TRUE, FALSE);
        insert_symbol(symbol_node);
    }
}

void process_directive_first_pass(char *field, int is_label, int *DC) {
    int directive_type;
    if (!field) {
        handle_error(EMPTY_DIRECTIVE_ERROR, lines_count_first_pass);
    }
    directive_type = find_directive_type(field);

    if (directive_type == NOT_EXISTS_DIRECTIVE_TYPE) {
        handle_error(NOT_EXISTS_DIRECTIVE_ERROR, lines_count_first_pass);
    }

    if (directive_type == ENTRY_DIRECTIVE_TYPE) {
        return;
    } else if (directive_type == EXTERN_DIRECTIVE_TYPE) {
        field = strtok(NULL, BLANK_CHARACTER_SEPERATOR);
        insert_extern_to_symbol_table(field);
    } else {
        if (is_label) {
            sym_pt symbol_node = create_symbol_node(label, *DC, FALSE, FALSE);
            insert_symbol(symbol_node);
        }

        if (directive_type == DATA_DIRECTIVE_TYPE) {
            field = strtok(NULL, ", \n\r\t");
            insert_numbers_to_data(field, lines_count_first_pass, DC);
        } else if (directive_type == STRING_DIRECTIVE_TYPE) {
            field = strtok(NULL, "");
            insert_string_to_data(field, lines_count_first_pass, DC);
        } else if (directive_type == MAT_DIRECTIVE_TYPE) {
            field = strtok(NULL, ", \n\r\t");
            insert_matrix_to_data(field, lines_count_first_pass, DC);
        }
    }


}



void process_instruction_first_pass(char *field, int is_label, int *IC) {
    opcode_pt cur_opcode;
    char *operand;
    enum bool is_action = TRUE;
    enum bool is_external = FALSE;
    addressing_t source_addressing, target_addressing;
    int words_count;

    source_addressing = target_addressing = NO_ADDRESSING;
    cur_opcode = get_opcode(field);
    if (!cur_opcode) {
        handle_error(OPCODE_NOT_FOUND_ERROR, lines_count_first_pass);
        return;
    }
    if (is_label) {
        sym_pt symbol_node = create_symbol_node(label, *IC, is_external, is_action);
        insert_symbol(symbol_node);
    }
    words_count = 1;
    if (cur_opcode->target_addressing_types) {
        if (cur_opcode->source_addressing_types) {
            operand = strtok(NULL, " \n,");
            if (!operand){
                handle_error(TOO_FEW_OPERANDS_ERROR, lines_count_first_pass);
                return;
            }
            source_addressing = get_addressing_and_validate(operand, cur_opcode->source_addressing_types, lines_count_first_pass);
        }
        operand = strtok(NULL, " \n,");
        if (!operand){
            handle_error(TOO_FEW_OPERANDS_ERROR, lines_count_first_pass);
            return;
        }
        target_addressing = get_addressing_and_validate(operand, cur_opcode->target_addressing_types, lines_count_first_pass);
    }

    words_count += get_words_count_by_both_addressings(source_addressing, target_addressing);

    operand = strtok(NULL, " \n,");
    if (operand) {
        handle_error(TOO_MANY_OPERANDS, lines_count_first_pass);
        return;
    }
    *IC += words_count;


}



