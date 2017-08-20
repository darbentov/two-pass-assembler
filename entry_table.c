#include "entry_table.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

entry_pt entry_head = NULL;

void insert_entry(char *label, int address) {
    entry_pt tmp;
    tmp = malloc(sizeof(Entry));
    check_allocation(tmp);

    tmp->label = strdup(label);
    tmp->address = address;
    tmp->next = NULL;

    if (!entry_head) {
        /*make data_head point to temp*/
        entry_head = tmp;
    }
        /*if list is not empty*/
    else {
        /*pointer to data_head*/
        entry_pt p = entry_head;
        /*make p point to last node*/
        while (p->next)
            p = p->next;
        /*make tmp the last node*/
        p->next = tmp;
    }
}

void clean_entry_table() {
    entry_pt ptr, tmp;
    ptr = entry_head;
    if (!ptr)
        return;

    while (ptr) {
        tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
    entry_head = NULL;
}
