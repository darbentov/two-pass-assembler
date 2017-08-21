#ifndef ASSEMBLER_CONSTANTS_H
#define ASSEMBLER_CONSTANTS_H


#define MAX_CODE_LINE 81
#define WORD_SIZE 10
#define START_IC 100
#define COMMENT_CHAR ';'
#define LABEL_SEPARATOR ':'
#define BLANK_CHARACTER_SEPARATOR " \t\n\r"
#define DECIMAL_BASE 10

#define OB_EXTENSION ".ob"

#define WRITE_MODE "w"

#define AS_EXTENSION ".as"

#define READ_MODE "r"

#define EXTERN_EXTENSION ".ext"

#define ENTRY_EXTENSION ".ent"

#define LINE_BREAK '\n'

#define TAB_SEPERATOR '\t'

#define EMPTY_DELIMITER ""

#define COMMA_AND_BLANK_SPACE_DELIMITER ", \n\r\t"
#define BITS_COUNT_OF_AN_INT 8
#define OPCODE_BITS_SIZE 4
#define ADDRESSING_BIT_SIZE 2
#define CODING_TYPE_BIT_SIZE 2
#define REGISTER_BIT_SIZE 4
#define REGISTER_NAME_LENGTH 2
#define STRING_NULL_TERMINATOR '\0'
#define SCANF_MATRIX_PATTERN "[%[^]]][%[^]]]"
#endif