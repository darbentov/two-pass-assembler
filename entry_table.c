#include "entry_table.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

entry_pt head = NULL;

void insert_entry(char *label, int address) {
    entry_pt tmp;
    tmp = malloc(sizeof(Entry));
    check_allocation(tmp);

    tmp->label = strdup(label);
    tmp->address = address;
    tmp->next = NULL;

    if (!head) {
        /*make data_head point to temp*/
        head = tmp;
    }
        /*if list is not empty*/
    else {
        /*pointer to data_head*/
        entry_pt p = head;
        /*make p point to last node*/
        while (p->next)
            p = p->next;
        /*make tmp the last node*/
        p->next = tmp;
    }
}