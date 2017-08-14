#include "file_processing.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "keywords.h"
#include "opcode.h"
#include "directives.h"


typedef struct flags {
    unsigned is_label:1;
    unsigned in_data:1;
    unsigned in_mat:1;
};


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




void process_file(char *filename) {
    char actions[MAX_CODE_LINE][WORD_SIZE];
    FILE *fp = fopen(filename, "r");
    first_pass(fp);
    if (err_count)
        return;
    second_pass(fp, symbol_head);
    if (err_count)
        return;
    generate_output_files(symbol_head);
    clean_symbol_table();
    clean_externs_table();
    clean_entries_table();
    clean_code();
    clean_data();
}

void first_pass(FILE *fp) {

    char line[MAX_CODE_LINE];
    char *line_p;
    line_p = line;
    while (fgets(line, MAX_CODE_LINE, fp)) {
        lines_count++;
        line_p = strip_blank_chars(line_p);
        if (is_comment_or_empty(line_p))
            continue;
        process_line_first_pass(line_p);
    }
}


char* go_to_next_field(char *line) {
    while (!isspace(*line))
        line++;
    while (isspace(*line)){
        line++;
    }
    return line;
}


int process_directive(char *line, int is_label) {
    char * directive_name;
    int directive_type;
    int i;
    i = 0;
    while (!isspace(*line++))
        i++;
    directive_name = strndup(line, i);
    directive_type = find_directive_type(directive_name);
    if (directive_type == NOT_EXISTS_DIRECTIVE_TYPE){
        print_error(NOT_EXISTS_DIRECTIVE_ERROR, lines_count);
        return 0;
    }

    if (directive_type < ENTRY_DIRECTIVE_TYPE){

    }

}

int process_line_first_pass(char *line) {
    {

        /*char *p=line  ;*/
        int temp = 0;
        int is_label = 0;


        /*___________________________________*/


        /* first character can either be a dot or a letter. */
        if (!(isalpha(*line) || *line == '.')) {
            err_count++;
            print_error(SYNTAX_ERROR, lines_count);
            return 0;
        }
        /******** Dealing with Label *******************/


        is_label = find_label(line);
        if (is_label) {
            line = go_to_next_field(line);
        }

        if (line == NULL) {
            err_count++;
            print_error(LABEL_WITH_NO_DATA_ERROR, lines_count);
            return 0;
        }

        /* Looking for directive...-------------*/
        if (*line == '.') {
            return process_directive(++line, is_label);
        }
            /*--------------------------------------*/

        else {
            return process_instruction(line, is_label);
        }
    }
}

bool find_label(char *line) {
    char *p = line;
    bool is_label_found;
    int i, c;
    i = 0;
    while (*p && *p != LABEL_SEPARATOR) {
        p++;
        i++;
    }
    is_label_found = (bool) (*p == LABEL_SEPARATOR);
    if (is_label_found) {
        strncpy(label, line, i);
        if (i > MAX_LABEL) {
            print_error(LABEL_TOO_LONG_ERROR, lines_count);
            return FALSE;
        }
        for (c = 0; c < i; c++) { /* checking label characters.*/
            if (!isdigit(line[c]) && !isalpha(line[c])) {
                print_error(LABEL_CONTAINS_INVALID_CHARACTER_ERROR, lines_count);
                return FALSE;
            }
        }
        if (!isalpha(*line)) { /* label must start with an alpha char */
            print_error(LABEL_FIRST_CHAR_IS_NOT_ALPHA_ERROR, lines_count);
            return FALSE;
        }
        if (is_keyword(label)) {
            print_error(LABEL_IS_KEYWORD_ERROR, lines_count);
            return FALSE;
        }
        if (search_extern(label)){
            print_error(LABEL_IS_ALREADY_EXTERN, lines_count);
            return FALSE;
        }
        if (search_entry(label)){
            print_error(LABEL_IS_ALREADY_ENTRY, lines_count);
            return FALSE;
        }
    }
    return is_label_found;
}

short int is_comment_or_empty(char *line) {
    char c;
    c = *line;
    if (!c || c == COMMENT_CHAR)
        return 1;
    return 0;
}

char *strip_blank_chars(char *line) {
    char c;
    while ((c = *line) && isspace(c))
        line++;
    return line;
}