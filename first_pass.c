#include <ctype.h>
#include "first_pass.h"
#include "keywords.h"
#include "data.h"
#include "directives.h"
#include "constants.h"
#include "error_handling.h"
#include "extern_table.h"

extern int err_count;
int lines_count_first_pass = 0;
char *label;


/*
 * First Pass Algorithm
 * 1.  IC -> 0, DC -> 0.
 * 2.  Read line, if EOF: move to step 16.
 * 3.  Check if first field of line is a label, if yes: move to step 4, if not: move to step 5.
 * 4.  turn on 'is_label' flag.
 *
 *
 ****** Dealing with a directive ***********
 *
 * 5.  Check if current field is a directive for storing data (.data / .mat / .string)?
 *     if yes: move to step 6, if not: move to step 8.
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
 * 11. If 'is_label' flag is turned on, insert label to Symbol table as a instruction Symbol
 *     and insert the current value of IC as the address.
 * 12. Look for the instruction name in the Instructions table.
 * 13. Check operands of the instruction and calculate the words count.
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
        token = strtok(line_p, BLANK_CHARACTER_SEPARATOR);
        if (!token || is_comment_or_empty(token))
            continue;
        process_line_first_pass(token, &DC, &IC);
    }
    increment_data_addresses_by_ic(IC);
    increment_symbol_addresses_by_ic(IC);
}

void process_line_first_pass(char *token, int *DC, int *IC) {
    {
        bool is_label;
        is_label = FALSE;
        if (!token)
            return;

        /*___________________________________*/


        /* first character can either be a dot or a letter. */
        if (!(isalpha(*token) || *token == '.')) {
            handle_error(SYNTAX_ERROR, lines_count_first_pass);
            return;
        }
        /*************** Dealing with Label *******************/


        label = get_label(token, lines_count_first_pass);
        if (label) {
            /* go to the next token in line */
            token = strtok(NULL, BLANK_CHARACTER_SEPARATOR);

            /* if label already exists, it is an error */
            if (search_symbol_by_label(label)) {
                handle_error(LABEL_ALREADY_EXISTS_ERROR, lines_count_first_pass);
                is_label = FALSE;
            } else {
                is_label = TRUE;
                /* check if there is something after the label, if not it is an error */
                if (!token) {
                    handle_error(LABEL_WITH_NO_DATA_ERROR, lines_count_first_pass);
                    return;
                }
            }
        }

        if (*token == '.') { /* means that this is a direcive */
            token++; /* skip '.' */
            process_directive_first_pass(token, is_label, DC);
        } else { /* if not a directive, then it is instruction */
            process_instruction_first_pass(token, is_label, IC);
        }
    }
}


/* Processing a directive line for the first pass
 *
 * (Steps 5-9)
 * 5.  Check if current field is a directive for storing data (.data / .mat / .string)?
 *     if yes: move to step 6, if not: move to step 8.
 * 6.  If 'is_label' flag is turned on, insert label to Symbol table as a data Symbol
 *     and insert the current value of DC as the address.
 * 7.  verify type of data, insert into data memory and update DC by the words count. Go back to step 2.
 * 8.  Is this a .extern or .entry?
 *     If yes: move to step 9, if not: move to step 11.
 * 9.  Is this a .extern?
 *     if yes, insert the label (operand of .extern directive) to the Symbol table with flag 'is_external'.
 *
 * */

void process_directive_first_pass(char *token, int is_label, int *DC) {
    int directive_type;
    enum bool is_action = FALSE;
    enum bool is_external = FALSE;

    /* if the directive is empty, raise an error */
    if (!token) {
        handle_error(EMPTY_DIRECTIVE_ERROR, lines_count_first_pass);
    }

    directive_type = find_directive_type(token);

    /* if directive type was not found, it means that the given directive does not exist, and this is an error */
    if (directive_type == NOT_EXISTS_DIRECTIVE_TYPE) {
        handle_error(NOT_EXISTS_DIRECTIVE_ERROR, lines_count_first_pass);
    }

    /* If direcitve type is .entry, skip it because it is the first pass */
    if (directive_type == ENTRY_DIRECTIVE_TYPE) {
        return;
    }
        /* If directive type is .extern, the operand of the directive is the label for the extern */
    else if (directive_type == EXTERN_DIRECTIVE_TYPE) {
        token = strtok(NULL, BLANK_CHARACTER_SEPARATOR); /* get next token of line */
        insert_extern_to_symbol_table(token, lines_count_first_pass);
    }
        /* If the directive type is one of the data directives, process it */
    else {
        /* If is_label flag is turned on, insert it to the Symbol tree with the current DC */
        if (is_label) {
            insert_symbol(label, *DC, is_external, is_action);
        }

        if (directive_type == DATA_DIRECTIVE_TYPE) {
            token = strtok(NULL, COMMA_AND_BLANK_SPACE_DELIMITER); /* get the next token of line */
            insert_numbers_to_data(token, lines_count_first_pass, DC);
        } else if (directive_type == STRING_DIRECTIVE_TYPE) {
            token = strtok(NULL, EMPTY_DELIMITER);
            /* string may contain comma or any blank character,
             * therefore we only skipping to next token with no delimiter,
             * and we skip the blank characters at the start of the token*/
            while (token && isspace(*token)) {
                token++;
            }
            insert_string_to_data(token, lines_count_first_pass, DC);
        } else if (directive_type == MAT_DIRECTIVE_TYPE) {
            token = strtok(NULL, COMMA_AND_BLANK_SPACE_DELIMITER); /* get the next token of line */
            insert_matrix_to_data(token, lines_count_first_pass, DC);
        }
    }


}


/* Processing an instruction line for the first pass
 *
 * (Steps 11-14)
 *
 * 11. If 'is_label' flag is turned on, insert label to Symbol table as an instruction Symbol
 *     and insert the current value of IC as the address.
 * 12. Look for the instruction name in the Instructions table.
 * 13. Check operands of the instruction and calculate the words count:
 *     13.1. assign the value '1' to words_count, as the instruction line of the opcode will cost 1 word.
 *     13.2. Check if the instruction permits a target operand,
 *           if yes: go to next sub step, if not: go to step 13.6.
 *     13.3. Check if the instruction permits a source operand,
 *           if yes: go to next sub step, if not: go to step 13.5.
 *     13.4. get the next operand of the instruction, get the addressing type by the operand,
 *           and check if the found addressing type is valid for this instruction, for the source operand.
 *     13.5. get the next operand of the instruction, get the addressing type by the operand,
 *           and check if the found addressing type is valid for this instruction, for the target operand.
 *     13.6. calculate the words count that the operands will cost.
 *
 * 14. update IC by the words count.
 *
 *
 * */
void process_instruction_first_pass(char *field, int is_label, int *IC) {
    opcode_pt cur_opcode;
    char *operand;
    bool is_action, is_external; /* for more readable code, flag variables are used to create the symbol */
    addressing_t source_addressing, target_addressing;
    int words_count;

    /* Assigning 'no_addressing' type for both addressings,
     * so the function to calculate the cost of words count can handle empty operands
     * */
    source_addressing = target_addressing = NO_ADDRESSING;

    /* If is_label is turned on, a Symbol should be created with the flags 'is_external' turned off
     * and 'is_action' turned on
     * */
    is_action = TRUE;
    is_external = FALSE;


    /*
     *
     * Step 11
     *
     * */
    if (is_label) {
        insert_symbol(label, *IC, is_external, is_action);
    }

    /*
     *
     * Step 12
     *
     * */
    cur_opcode = get_opcode(field);
    if (!cur_opcode) {
        handle_error(OPCODE_NOT_FOUND_ERROR, lines_count_first_pass);
        return;
    }


    /*
     *
     * Step 13
     *
     * */
    words_count = 1; /* 13.1. */

    if (cur_opcode->target_addressing_types) { /* 13.2. */
        if (cur_opcode->source_addressing_types) { /* 13.3 */
            /*
             *
             * Step 13.4.
             *
             * */
            operand = strtok(NULL, COMMA_AND_BLANK_SPACE_DELIMITER);
            if (!operand) {
                handle_error(TOO_FEW_OPERANDS_ERROR, lines_count_first_pass);
                return;
            }
            source_addressing = get_addressing_and_validate(operand, cur_opcode->source_addressing_types,
                                                            lines_count_first_pass);
        }
        /*
         *
         * Step 13.5.
         *
         * */
        operand = strtok(NULL, COMMA_AND_BLANK_SPACE_DELIMITER);
        if (!operand) {
            handle_error(TOO_FEW_OPERANDS_ERROR, lines_count_first_pass);
            return;
        }
        target_addressing = get_addressing_and_validate(operand, cur_opcode->target_addressing_types,
                                                        lines_count_first_pass);
    }

    /*
     *
     * Step 13.6.
     *
     * */
    words_count += get_words_count_by_both_addressings(source_addressing, target_addressing);

    /* validate that there are no more operands, if there are, this is an error */
    operand = strtok(NULL, COMMA_AND_BLANK_SPACE_DELIMITER);
    if (operand) {
        handle_error(TOO_MANY_OPERANDS, lines_count_first_pass);
        return;
    }

    /*
     *
     * Step 14
     *
     * */
    *IC += words_count;


}



