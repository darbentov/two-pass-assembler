#include "data.h"
#include "error_handling.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*This file contains functions which handle the data list*/

/* Matrix scanning values */
#define NUMBER_OPERAND -1

/*pointer to the first node*/
static Data_pt data_head = NULL;
static short int data_count;
char get_matrix_declaration_operand(char **mat_decleration_pt, int *target_operand, bool is_skip_spaces);


/*This functions counts the number of nodes in the data list*/
short get_data_count() {
    return data_count;
}

/*This function updates the data addresses AFTER we have all our operation words*/
void increment_data_addresses_by_ic(int IC) {
    /*pointer to data_head*/
    Data_pt ptr;
    ptr = data_head;
    /*if data_head is null, return*/
    if (!ptr) {
        return;
    }
    /*go through every node and assign it an address*/
    while (ptr) {
        ptr->address += IC + START_IC;
        /*increment the address to be assigned to the next node*/
        IC++;
        ptr = ptr->next;
    }
}

/*Clear the data list*/
void clean_data() {
    /*temp pointer to data_head*/
    Data_pt ptr = data_head;
    /*if data_head is null, return*/
    if (!ptr)
        return;
    /*free each data node*/
    while (ptr) {
        Data_pt tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
    /*make the data_head pointer null*/
    data_head = NULL;
    data_count = 0;
}


bool is_data_empty() {
    return (bool) (data_count == 0);
}


/*
 * Write data to .ob file
 *
 *
 *
 * */
void write_data_to_ob_file(FILE *fp) {
    char word[WORD_SIZE + 1];
    char address[SIZE_OF_BINARY_INT + 1];
    char address_4_base[SIZE_OF_4_BASE_INT + 1];
    char value_4_base[WORD_SIZE_4_BASE + 1];
    Data_pt ptr = data_head;
    while (ptr) {
        int_to_bin(ptr->address, address, SIZE_OF_BINARY_INT);
        int_to_bin(ptr->value, word, WORD_SIZE);
        bin_to_4base(address, address_4_base, SIZE_OF_BINARY_INT);
        bin_to_4base(word, value_4_base, WORD_SIZE);
        fputs(address_4_base, fp);
        fputc(TAB_SEPERATOR, fp);
        fputs(value_4_base, fp);
        fputc(LINE_BREAK, fp);
        ptr = ptr->next;
    }
}


void insert_new_data(int *DC, short num) {/*create new pointer to data, allocate memory*/
    Data_pt new_node;
    new_node = malloc(sizeof(Data));
    check_allocation(new_node);

    new_node->value = num;
    new_node->next = NULL;

    /*if the list is empty*/
    if (!data_head) {
        /*make data_head point to temp*/
        data_head = new_node;
    }

    else {
        /* Search for the last node */
        Data_pt p = data_head;
        while (p->next)
            p = p->next;

        /*make new_node the last node*/
        p->next = new_node;
    }
    data_count++; /* Increment the inner data count */
    (*DC)++; /* Increment the DC for the assembler data count */
}


/*
 ***********************************************
 ***********************************************
 *********** Handle .data directive ************
 ***********************************************
 ***********************************************
 */

/*This function takes a string as a parameter, and splits it into tokens of whole numbers.
if the string recieved is null, it will add an error. otherwise, it will loop through the string's
tokens and send each one to the insert_single_number function*/
void insert_numbers_to_data(char *operands_line, int line_number, int *DC) {
    char *operand;
    /*if the operands_line variable is null*/
    if (!operands_line) {
        handle_error(DATA_DIRECTIVE_WITH_NO_DATA_ERROR, line_number);
    }

    else {
        /*send first operands_line to insert_single_number()*/
        operand = strtok(operands_line, COMMA_AND_BLANK_SPACE_DELIMITER);
        /*send the rest tokens to insert_single_number()*/
        while (operand) {
            insert_single_number(operand, line_number, DC);
            operand = strtok(NULL, COMMA_AND_BLANK_SPACE_DELIMITER);
        }
    }
}

/*This function adds a number to the data list.
It will check if the string is a valid number, and if the number is within range.
It will convert the string to a long int and then store it in the data node*/
void insert_single_number(char *number, int line_number, int *DC) {
    /*pointer for strtol error checking*/
    char *ptr = NULL;
    /*temporary data node*/
    /*temporary holder for the converted number*/
    long int num;

    /*convert the string to a number*/
    num = strtol(number, &ptr, DECIMAL_BASE);
    /*if strtok returned an error*/
    if (*ptr) {
        /*add an error and exit function*/
        handle_error(INVALID_NUMBER_ERROR, line_number);
        return;
    }

    /*if number is out of 10-bit range*/
    if ((num < MIN_NUMBER_DATA) || (num > MAX_NUMBER_DATA)) {
        /*add error and exit function*/
        handle_error(NUMBER_OUT_OF_RANGE_ERROR, line_number);
        return;
    }
    insert_new_data(DC, (short int) num);
}




/*
 ***********************************************
 ***********************************************
 ********* Handle .string directive ************
 ***********************************************
 ***********************************************
 */



/*This function adds a string to the data list*/
void insert_string_to_data(char *string, int line_number, int *DC) {
    char c;

    /* check if string is empty */
    if (!string) {
        handle_error(NO_STRING_ERROR, line_number);
        return;
    }

    if (*string++ != QUOTATION_MARK) {
        handle_error(STRING_MUST_BE_ENCLOSED_BY_QUOTES_ERROR, line_number);
        return;
    }

    /*Loop adding each character to the data list*/
    while ((c = *string++) && c != QUOTATION_MARK) {
        insert_new_data(DC, c);
    }
    if (c != QUOTATION_MARK) {
        handle_error(STRING_MUST_BE_ENCLOSED_BY_QUOTES_ERROR, line_number);
        return;
    } else {
        while ((c = *string++)) {
            if (!isspace(c)) {
                handle_error(EXTRA_CHARACTERS_AFTER_STRING_DECLARARTION_ERROR, line_number);
                return;
            }
        }
    }
    /*add null terminator*/
    insert_new_data(DC, STRING_NULL_TERMINATOR);

}


/*
 ***********************************************
 ***********************************************
 ********* Handle .matrix directive ************
 ***********************************************
 ***********************************************
 */


/* Insert the data of .matrix directive */
void insert_matrix_to_data(char *operands_line, int line_number, int *DC) {
    int matrix_data_count;
    char *operand;

    /* getting count from matrix and skip the matrix declaration */
    matrix_data_count = get_count_from_matrix_declaration(&operands_line, line_number);
    if (matrix_data_count == 0) {
        return;
    }
    /* get first operand of matrix */
    operand = strtok(operands_line, COMMA_AND_BLANK_SPACE_DELIMITER);
    /* Get all the operands for the new matrix  */
    while (matrix_data_count-- > 0) {

        if (!operand) {
            operand = "0";
        }
        insert_single_number(operand, line_number, DC);
        operand = strtok(NULL, COMMA_AND_BLANK_SPACE_DELIMITER);
    }
    /* check if there an additional unexpected operands_line */
    operand = strtok(NULL, COMMA_AND_BLANK_SPACE_DELIMITER);
    if (operand) {
        handle_error(TOO_MANY_NUMBERS_FOR_MAT, line_number);
    }

}


/* Get count from matrix declaration 
 * 
 * Iterating the string of matrix declaration and find the pattern of '[row][col]'
 * 
 * Rules:
 * 1. Blank characters are allowed inside the '[]'.
 * 2. Blank characters are not allowed between both dimensions ('[] []' is not allowed)/
 * 3. Negative numbers are not allowed, but the scanner needs to catch them to handle the correct error.
 * 4. After the second closing bracket, a blank character is expected, so the scanner knows that the job was finished.
 * */
int get_count_from_matrix_declaration(char **matrix_declaration, int line_number) {
    int row, col, operand, status;
    char c;
    bool is_skip_spaces;
    status = BEGINING;
    is_skip_spaces = TRUE; /* is_skip_spaces is set to TRUE, because space is allowed until the first opening bracket */
    col = row = 0;
    /* getting operands in a loop
     *
     * Expected operands:
     * 1. '['
     * 2. ']'
     * 3. a number
     * */
    while ((c = get_matrix_declaration_operand(matrix_declaration, &operand, is_skip_spaces)) && !isspace(c)) {
        switch (c) {
            case OPENING_BRACKET:
                if (status == BEGINING) {
                    /* the scan got the first '[' character */
                    status = AFTER_FIRST_OPENING_BRACKET;
                } else if (status == AFTER_FIRST_CLOSING_BRACKET) {
                    /* the scan got the second '[' character */
                    status = AFTER_SECOND_OPENING_BRACKET;
                    /* inside '[]' space is allowed */
                    is_skip_spaces = TRUE;
                }
                else {
                    handle_error(INVALID_MATRIX_DECLARATION, line_number);
                    return 0;
                }
                break;
            case CLOSING_BRACKET:
                if (status == AFTER_ROW_DIMENSION){
                    status = AFTER_FIRST_CLOSING_BRACKET;
                    /* spaces are not allowed between the row and col declaration */
                    is_skip_spaces = FALSE;
                } else if (status == AFTER_COL_DIMENSION){
                    status = AFTER_SECOND_CLOSING_BRACKET;
                    /* after the scan got both dimensions, set the 'is_skip_spaces' to FALSE,
                     * to complete the scan of the matrix dimensions */
                    is_skip_spaces = FALSE;
                }
                else {
                    /* if closing bracket is not after a number, it is an error */
                    handle_error(INVALID_MATRIX_DECLARATION, line_number);
                    return 0;
                }
                break;
            case NUMBER_OPERAND:
                /* if a number was found */
                if (status == AFTER_FIRST_OPENING_BRACKET){
                    status = AFTER_ROW_DIMENSION;
                    row = operand;
                }
                else if (status == AFTER_SECOND_OPENING_BRACKET){
                    status = AFTER_COL_DIMENSION;
                    col = operand;
                }
                else {
                    /* if a number found not after an opening bracket, is it an error */
                    handle_error(SYNTAX_ERROR, line_number);
                }
                break;
            default:
                /* if the scanner found unexpected characters, is it an error */
                handle_error(SYNTAX_ERROR, line_number);
                return 0;
        }
    }

    /* if the scanner has not got to the second closing bracket, it means that the matrix declaration is not valid */
    if (status != AFTER_SECOND_CLOSING_BRACKET){
        handle_error(MISSING_COLUMN_DIMENSION, line_number);
        return 0;
    }
    /* if the scanner found a negative number, it is an error */
    if (col < 0 || row < 0){
        handle_error(NEGATIVE_INDEX_MATRIX_DECLARATION_ERROR, line_number);
        return 0;
    }
    return col * row;
}

char get_matrix_declaration_operand(char **mat_decleration_pt, int *target_operand, bool is_skip_spaces) {
    char c;
    int multiplier;
    if (is_skip_spaces){
        while (isspace(**mat_decleration_pt)){
            *mat_decleration_pt+= 1;
        }
    }
    /* settings multiplier to 1, if '-' found, the multiplier will change to -1 to get negative number */
    multiplier = 1;
    *target_operand = 0;
    
    c = **mat_decleration_pt; /* get first character */
    
    /* if c is not a number or '-' or '+' */
    if (!isdigit(c) && c != '-' && c != '+'){
        *mat_decleration_pt+= 1;
        return c;
    }

    /* handle multiplier */
    if (c == '-' || c == '+'){
        if (c == '-'){
            multiplier = - 1;
        }
        *mat_decleration_pt+= 1;
        c = **mat_decleration_pt;

    }
    /* find whole number */
    while (isdigit(c)){
        *target_operand = (*target_operand * 10) + (c - '0');
        *mat_decleration_pt+= 1;
        c = **mat_decleration_pt; /* get the next character */
    }
    *target_operand = *target_operand * multiplier; /* multiply the operand with the multiplier to handle '-' sign */
    return NUMBER_OPERAND;

}
