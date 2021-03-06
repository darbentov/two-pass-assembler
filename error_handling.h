#ifndef ASSEMBLER_ERROR_HANDLING_H
#define ASSEMBLER_ERROR_HANDLING_H


typedef enum error_code {
    SYNTAX_ERROR,
    LABEL_WITH_NO_DATA_ERROR,
    LABEL_ALREADY_EXISTS_ERROR,
    LABEL_TOO_LONG_ERROR,
    LABEL_CONTAINS_INVALID_CHARACTER_ERROR,
    LABEL_FIRST_CHAR_IS_NOT_ALPHA_ERROR,
    LABEL_IS_OPCODE_ERROR,
    LABEL_IS_DIRECTIVE_ERROR,
    LABEL_IS_REGISTER_ERROR,
    EMPTY_DIRECTIVE_ERROR,
    NOT_EXISTS_DIRECTIVE_ERROR,
    LABEL_IS_ALREADY_EXTERN,
    LABEL_IS_ALREADY_ENTRY,
    DATA_DIRECTIVE_WITH_NO_DATA_ERROR,
    INVALID_NUMBER_ERROR,
    NUMBER_OUT_OF_RANGE_ERROR,
    NO_STRING_ERROR,
    STRING_MUST_BE_ENCLOSED_BY_QUOTES_ERROR,
    ENTRY_OPERAND_LABEL_DOES_NOT_EXIST_ERROR,
    LABEL_DOES_NOT_EXIST_ERROR,
    ADDRESSING_NOT_ALLOWED_ERROR,
    OPCODE_NOT_FOUND_ERROR,
    TOO_FEW_OPERANDS_ERROR,
    TOO_MANY_OPERANDS,
    INVALID_INSTRUCTION_OPERAND_ERROR,
    MATRIX_INDEX_MUST_BE_REGISTERS_ERROR,
    INVALID_MATRIX_DECLARATION,
    TOO_MANY_NUMBERS_FOR_MAT,
    NEGATIVE_INDEX_MATRIX_DECLARATION_ERROR,
    EXTRA_CHARACTERS_AFTER_STRING_DECLARARTION_ERROR,
    EXTRA_OPERANDS_AFTER_EXTERN_DIRECITVE,
    MISSING_COLUMN_DIMENSION

} error_code;

void handle_error(error_code err, int line_number);

#endif /* ASSEMBLER_ERROR_HANDLING_H */
