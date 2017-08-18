#include "data.h"
#include "error_handling.h"
#include "utils.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>
/*This file contains functions which handle the data list*/

/*pointer to the first node*/
static Data_pt data_head = NULL;


/*This function takes a string as a parameter, and splits it into tokens of whole numbers.
if the string recieved is null, it will add an error. otherwise, it will loop through the string's
tokens and send each one to the insert_single_number function*/
int insert_numbers_to_data(char *token, int line_number, int *DC) {
    /*if the token is null*/
    if (!(token = strtok(NULL, " ,\t\n"))) {
        /*add error and return*/
        handle_error(DATA_DIRECTIVE_WITH_NO_DATA_ERROR, line_number);
        return BAD_EXIT_STATUS;
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
    (*DC)++;
}


/*This function adds a string to the data list*/
int insert_string_to_data(char *string, int line_number, int *DC) {
    char c;
    /* check if string is empty */
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
    }
    /*add null terminator*/
    insert_new_data(DC, STRING_NULL_TERMINATOR);
    return GOOD_EXIT_STATUS;
    /*increment data count*/
}

/*This functions counts the number of nodes in the data list*/
int count_all_data() {
    /*Temp counter variable*/
    int counter = 0;
    /*pointer to data_head*/
    Data_pt ptr = data_head;
    /*if data_head is null, return counter (0)*/
    if (!ptr)
        return counter;

    /*go through the list and count each node*/
    while (ptr) {
        counter++;
        ptr = ptr->next;
    }
    /*Return the number of nodes*/
    return counter;
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
        ptr->address = IC;
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
}

/*return pointer to data_head. used when exporting files*/
Data_pt get_data_head() {
    Data_pt tmp = data_head;
    return tmp;
}
