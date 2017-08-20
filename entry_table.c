#include "entry_table.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>

entry_pt entry_head = NULL;
static short int entry_count;

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
    entry_count++;
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
    entry_count = 0;
}

void write_entry_file(char *filename) {
    FILE *fp;
    char address[9], address_4_base[5];
    fp = open_file(filename, WRITE_MODE, ENTRY_EXTENSION);
    entry_pt tmp;
    tmp = entry_head;
    while (tmp) {
        int_to_bin(tmp->address, address, 8);
        bin_to_4base(address, address_4_base, 8);
        fputs(tmp->label, fp);
        fputc(TAB_SEPERATOR, fp);
        fputs(address_4_base, fp);
        fputc(LINE_BREAK, fp);
        tmp = tmp->next;
    }
    fclose(fp);

}

bool is_entry_empty() {
    return (bool) (entry_count == 0);
}
