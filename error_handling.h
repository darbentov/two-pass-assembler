#ifndef ASSEMBLER_ERROR_HANDLING_H
#define ASSEMBLER_ERROR_HANDLING_H

#endif /* ASSEMBLER_ERROR_HANDLING_H */
typedef enum error_code {
    EMPTY_ERROR, /* for better use, the enum will start from 1, so we put here an empty error.*/
    SYNTAX_ERROR,
    LABEL_WITH_NO_DATA_ERROR,
    LABEL_ALREADY_EXISTS_ERROR,
    LABEL_TOO_LONG_ERROR,
    LABEL_CONTAINS_INVALID_CHARACTER_ERROR,
    LABEL_FIRST_CHAR_IS_NOT_ALPHA_ERROR,
    LABEL_IS_KEYWORD_ERROR,
    NOT_EXISTS_DIRECTIVE_ERROR,
    LABEL_IS_ALREADY_EXTERN,
    LABEL_IS_ALREADY_ENTRY

} error_code;

void print_error(error_code err, int line_number);