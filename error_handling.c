/*
Error handling of errors occurred during introspection of .as file
*/
#include <stdio.h>
#include "error_handling.h"

int err_count;

void handle_error(error_code err, int line_number) {
    err_count++;
    printf("\tError found in line %d: ", line_number);
    switch (err) {
        case SYNTAX_ERROR:
            printf("Syntax error");
            break;
        case LABEL_WITH_NO_DATA_ERROR:
            printf("line should contain more than a label");
            break;
        case LABEL_ALREADY_EXISTS_ERROR:
            printf("label already exists");
            break;
        case LABEL_TOO_LONG_ERROR:
            printf("label contains more than 30 characters");
            break;
        case LABEL_CONTAINS_INVALID_CHARACTER_ERROR:
            printf("label contains invalid characters");
            break;
        case LABEL_FIRST_CHAR_IS_NOT_ALPHA_ERROR:
            printf("First character of the label must be a letter");
            break;
        case LABEL_IS_OPCODE_ERROR:
            printf("Label cannot be an opcode");
            break;
        case LABEL_IS_DIRECTIVE_ERROR:
            printf("Label cannot be a directive");
            break;
        case LABEL_IS_REGISTER_ERROR:
            printf("Label cannot be a register");
            break;
        case EMPTY_DIRECTIVE_ERROR:
            printf("directive is empty");
        case NOT_EXISTS_DIRECTIVE_ERROR:
            printf("Unrecognized directive");
            break;
        case LABEL_IS_ALREADY_EXTERN:
            printf("label already defined as external");
            break;
        case LABEL_IS_ALREADY_ENTRY:
            printf("label already defined as entry");
            break;
        case DATA_DIRECTIVE_WITH_NO_DATA_ERROR:
            printf(".data directive got no data to insert");
            break;
        case INVALID_NUMBER_ERROR:
            printf("invalid number");
            break;
        case NUMBER_OUT_OF_RANGE_ERROR:
            printf("number is out of range");
            break;
        case NO_STRING_ERROR:
            printf(".string directive got no string to insert");
            break;
        case STRING_MUST_BE_ENCLOSED_BY_QUOTES_ERROR:
            printf("string must be enclosed by '\"'");
            break;
        case ENTRY_OPERAND_LABEL_DOES_NOT_EXIST_ERROR:
            printf("entry points to a label that does not exist in the file");
            break;
        case LABEL_DOES_NOT_EXIST_ERROR:
            printf("label does not exist");
            break;
        case ADDRESSING_NOT_ALLOWED_ERROR:
            printf("Illegal addressing");
            break;
        case OPCODE_NOT_FOUND_ERROR:
            printf("opcode not found");
            break;
        case TOO_FEW_OPERANDS_ERROR:
            printf("too few operands for given opcode");
            break;
        case TOO_MANY_OPERANDS:
            printf("too many operands for given opcode");
            break;
        case INVALID_INSTRUCTION_OPERAND_ERROR:
            printf("invalid operand for instruction");
            break;
        case MATRIX_INDEX_MUST_BE_REGISTERS_ERROR:
            printf("matrix indices must be registers");
            break;
        case INVALID_MATRIX_DECLARATION:
            printf("invalid matrix declaration");
            break;
        case TOO_MANY_NUMBERS_FOR_MAT:
            printf("too many numbers in matrix declaration");
            break;
        case NEGATIVE_INDEX_MATRIX_DECLARATION_ERROR:
            printf("matrix indicies must be a positive number");
            break;
        case EXTRA_CHARACTERS_AFTER_STRING_DECLARARTION_ERROR:
            printf("extra characters after string declaration");
            break;
        case EXTRA_OPERANDS_AFTER_EXTERN_DIRECITVE:
            printf("redundant operands after .extern directive");
            break;
        case MISSING_COLUMN_DIMENSION:
            printf("missing matrix column dimension.");
            break;
    }
    printf("\n");

}
