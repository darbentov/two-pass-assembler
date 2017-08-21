#include "entry_table.h"
#include "constants.h"
#include <stdlib.h>
#include <string.h>

entry_pt entry_head = NULL;
static short int entry_count;

/* Insert entry to linked list */
void insert_entry(char *label, int address) {
    entry_pt tmp;
    tmp = malloc(sizeof(Entry));
    check_allocation(tmp);
    tmp->label = strdup(label);
    tmp->address = address;
    tmp->next = NULL;

    if (!entry_head) {
        /* If entry linked list is empty, make the new node as the head of the linked list*/
        entry_head = tmp;
    }
    else {
        /* point to head of list, iterating through the list until the last node found */
        entry_pt p = entry_head;

        while (p->next)
            p = p->next;

        /* after the next node found, assign the new node to the next pointer */
        p->next = tmp;
    }

    /* increment entry count, so we can get the count in O(1) */
    entry_count++;
}

/*** Clean entry table ***
 *
 * After the process of one file, the entry table needs to be cleaned for the next file processing
 *
 * */
void clean_entry_table() {
    entry_pt ptr, tmp;
    ptr = entry_head;

    /* point to head of list, iterating through the list until the last node found
     * free the memory taken by the current node in loop.
     * */
    while (ptr) {
        tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }

    entry_head = NULL;
    entry_count = 0;
}

/*** Write Entry File
 *
 * */

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
