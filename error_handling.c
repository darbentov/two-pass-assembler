/*
Error handling of errors occurred during introspection of .as file
*/
#include <stdio.h>
#include "error_handling.h"

int err_count;

void handle_error(error_code err, int line_number) {
    err_count++;
    switch (err) {
        case SYNTAX_ERROR:
            printf("line %d: Syntax error. \n", line_number);
            break;
        case LABEL_WITH_NO_DATA_ERROR:
            printf("Line %d: line should contain more than a label.\n", line_number);
            break;
        case LABEL_ALREADY_EXISTS_ERROR:
            printf("Line %d: label already exists.\n", line_number);
            break;
        case LABEL_TOO_LONG_ERROR:
            printf("line %d: label contains more than 30 characters.\n", line_number);
            break;
        case LABEL_CONTAINS_INVALID_CHARACTER_ERROR:
            printf("Line %d: label contains invalid characters.\n", line_number);
            break;
        case LABEL_FIRST_CHAR_IS_NOT_ALPHA_ERROR:
            printf("Line %d: First character of the label must be a letter.\n", line_number);
            break;
        case LABEL_IS_OPCODE_ERROR:
            printf("Line %d: Label cannot be an opcode.\n", line_number);
            break;
        case LABEL_IS_DIRECTIVE_ERROR:
            printf("Line %d: Label cannot be a directive.\n", line_number);
            break;
        case LABEL_IS_REGISTER_ERROR:
            printf("Line %d: Label cannot be a register.\n", line_number);
            break;
        case EMPTY_DIRECTIVE_ERROR:
            printf("Line %d: directive is empty.\n", line_number);
        case NOT_EXISTS_DIRECTIVE_ERROR:
            printf("Line %d: Unrecognized directive.\n", line_number);
            break;
        case LABEL_IS_ALREADY_EXTERN:
            printf("Line %d: label already defined as external.\n", line_number);
            break;
        case LABEL_IS_ALREADY_ENTRY:
            printf("Line %d: label already defined as entry.\n", line_number);
            break;
        case DATA_DIRECTIVE_WITH_NO_DATA_ERROR:
            printf("Line %d: .data directive got no data to insert.\n", line_number);
            break;
        case INVALID_NUMBER_ERROR:
            printf("Line %d: invalid number.\n", line_number);
            break;
        case NUMBER_OUT_OF_RANGE_ERROR:
            printf("Line %d: number is out of range.\n", line_number);
            break;
        case NO_STRING_ERROR:
            printf("Line %d: .string directive got no string to insert.\n", line_number);
            break;
        case STRING_MUST_BE_ENCLOSED_BY_QUOTES_ERROR:
            printf("Line %d: string must be enclosed by '\"'.\n", line_number);
            break;
        case ENTRY_OPERAND_LABEL_DOES_NOT_EXIST_ERROR:
            printf("Line %d: entry points to a label that does not exist in the file.\n", line_number);
            break;
        case 40:
            printf("line %d: label cannot be a directive. \n", line_number);
            break;
        case 41:
            printf("line %d: Previously defined label. \n", line_number);
            break;
        case 62:

            break;
        case 74:
            printf("line %d: unrecognized insruction. \n", line_number);
            break;

        case 50:
            printf("line %d: To many characters for one line. The number of characters per line must be less than 80.\n",
                   line_number);
            break;
        case 101:
            printf("line %d: bad argument[s]\n", line_number);
            break;
        case 111:
            printf("line %d: label cannot be an instruction.\n", line_number);
            break;
        case 112:
            printf("Line %d: Illegal addressing, check operands. \n", line_number);
            break;
        case 131:
            printf("Line %d: label cannot be a register \n", line_number);
            break;
        case 114:
            printf("Line %d:no character[s] between the quotation marks ", line_number);
            break;

        case 116:

        case 117:
            printf("Line %d: cannot define label as external. ", line_number);
            printf("label already defined as internal.\n");
            break;
        case 118:
            printf("Line %d: Error: expecting argument...\n", line_number);
            break;
    }

}


/*---------------- write_error2 ------------------------
write_error2: This function uses the global variable
             "error" to determine whether an error
	         should be written to the screen.
--------------------------------------------------------
*/
int write_error2(int line_number, char *line) {

    switch (error) {
        case 1:
            printf("Line %d: unrecognized label \n", line_number);
            break;
        case 2:
            printf("Line %d: Error: expecting argument... \n", line_number);
            break;
        case 3:
            printf("Line %d: To few actual argument[s] \n", line_number);
            break;
        case 4:
            printf("Line %d: misseing quotation[s] character on .string directive", line_number);
            break;
        case 5:
            line = skipspace(line);
            line = nextfield(line);
            if (line == NULL)
                return 0;
            printf("Line %d: file has no internal label name \"%s\" ", line_number, line);
            break;

    }
    return -13; /* late */
}

