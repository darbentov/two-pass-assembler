#include "data.h"
#include "error_handling.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*This file contains functions which handle the data list*/

/*pointer to the first node*/
static Data_pt data_head = NULL;
static short int data_count;


/*This function takes a string as a parameter, and splits it into tokens of whole numbers.
if the string recieved is null, it will add an error. otherwise, it will loop through the string's
tokens and send each one to the insert_single_number function*/
void insert_numbers_to_data(char *token, int line_number, int *DC) {
    /*if the token is null*/
    if (!token) {
        /*add error and return*/
        handle_error(DATA_DIRECTIVE_WITH_NO_DATA_ERROR, line_number);
    }
        /*if token is not null*/
    else {
        /*send first token to insert_single_number()*/
        insert_single_number(token, line_number, DC);
        /*send the rest tokens to insert_single_number()*/
        while ((token = strtok(NULL, " ,\t\n"))) {
            insert_single_number(token, line_number, DC);
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

    /*if number is out of 15-bit range*/
    if ((num < MIN_NUMBER_DATA) || (num > MAX_NUMBER_DATA)) {
        /*add error and exit function*/
        handle_error(NUMBER_OUT_OF_RANGE_ERROR, line_number);
        return;
    }
    insert_new_data(DC, (short int) num);
}

void insert_new_data(int *DC, short num) {/*create new pointer to data, allocate memory*/
    Data_pt tmp;
    tmp = malloc(sizeof(Data));
    check_allocation(tmp);
    /*Cast tmpNum(long) to a short int type, and assign to tmp*/
    tmp->value = num;
    tmp->next = NULL;
    /*if the list is empty*/
    if (!data_head) {
        /*make data_head point to temp*/
        data_head = tmp;
    }
        /*if list is not empty*/
    else {
        /*pointer to data_head*/
        Data_pt p = data_head;
        /*make p point to last node*/
        while (p->next)
            p = p->next;
        /*make tmp the last node*/
        p->next = tmp;
    }
    data_count++;
    (*DC)++;
}


/*This function adds a string to the data list*/
int insert_string_to_data(char *string, int line_number, int *DC) {
    char c;
    /* check if string is empty */
    while (string && isspace(*string)) {
        string++;
    }
    if (!string) {
        handle_error(NO_STRING_ERROR, line_number);
        return BAD_EXIT_STATUS;
    }

    if (*string++ != QUOTATION_MARK) {
        handle_error(STRING_MUST_BE_ENCLOSED_BY_QUOTES_ERROR, line_number);
        return BAD_EXIT_STATUS;
    }

    /*Loop adding each character to the data list*/
    while ((c = *string++) && c != QUOTATION_MARK) {
        insert_new_data(DC, c);
    }
    if (c != QUOTATION_MARK) {
        handle_error(STRING_MUST_BE_ENCLOSED_BY_QUOTES_ERROR, line_number);
        return BAD_EXIT_STATUS;
    } else {
        while ((c = *string++)) {
            if (!isspace(c)) {
                handle_error(EXTRA_CHARACTERS_AFTER_STRING_DECLARARTION_ERROR, line_number);
                return BAD_EXIT_STATUS;
            }
        }
    }
    /*add null terminator*/
    insert_new_data(DC, STRING_NULL_TERMINATOR);
    return GOOD_EXIT_STATUS;
    /*increment data count*/
}

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


int insert_matrix_to_data(char *token, int line_number, int *DC) {
    int matrix_data_count;
    matrix_data_count = get_data_count_from_matrix_declaration(token, line_number);
    if (matrix_data_count == 0) {
        return 1;
    }
    while (matrix_data_count-- > 0) {
        token = strtok(NULL, " ,\t\n");
        if (!token) {
            token = "0";
        }
        insert_single_number(token, line_number, DC);
    }
    token = strtok(NULL, " ,\t\n");
    if (token) {
        handle_error(TOO_MANY_NUMBERS_FOR_MAT, line_number);
    }
    return 1;
}

bool is_data_empty() {
    return (bool) (data_count == 0);
}

void write_data_to_ob_file(FILE *fp) {
    char word[WORD_SIZE];
    char address[9];
    char address_4_base[5];
    char value_4_base[6];
    Data_pt ptr = data_head;
    while (ptr) {
        int_to_bin(ptr->address, address, 8);
        int_to_bin(ptr->value, word, WORD_SIZE - 1);
        bin_to_4base(address, address_4_base, 8);
        bin_to_4base(word, value_4_base, 10);
        fputs(address_4_base, fp);
        fputc(TAB_SEPERATOR, fp);
        fputs(value_4_base, fp);
        fputc(LINE_BREAK, fp);
        ptr = ptr->next;
    }
}
