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
        case LABEL_DOES_NOT_EXIST_ERROR:
            printf("line %d: label does not exist.\n", line_number);
            break;
        case ADDRESSING_NOT_ALLOWED_ERROR:
            printf("Line %d: Illegal addressing.\n", line_number);
            break;
        case OPCODE_NOT_FOUND_ERROR:
            printf("Line %d: opcode not found.\n", line_number);
            break;
        case TOO_FEW_OPERANDS_ERROR:
            printf("Line %d: too few operands for given opcode.\n", line_number);
            break;
        case TOO_MANY_OPERANDS:
            printf("Line %d: too many operands for given opcode.\n", line_number);
            break;
        case INVALID_INSTRUCTION_OPERAND_ERROR:
            printf("Line %d: invalid operand for instruction.\n", line_number);
            break;
        case MATRIX_INDEX_MUST_BE_REGISTERS_ERROR:
            printf("Line %d: matrix indexes must be registers.\n", line_number);
            break;
        case INVALID_MATRIX_DECLARATION:
            printf("Line %d: invalid matrix declaration.\n", line_number);
            break;
    }

}
