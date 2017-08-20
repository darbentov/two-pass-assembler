#include "extern_table.h"
#include <stdlib.h>
#include <string.h>

static extern_pt extern_head = NULL;


void insert_extern(char *extern_label, int address) {
    extern_pt new, tmp;
    new = (extern_pt) (malloc(sizeof(ExternObj)));
    new->label = strdup(extern_label);
    new->address = address;
    if (!extern_head) {
        extern_head = new;
    } else {
        tmp = extern_head;
        while (tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = new;
    }
}

void clean_extern_table() {
    extern_pt ptr, tmp;
    ptr = extern_head;
    if (!ptr)
        return;

    while (ptr) {
        tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
    extern_head = NULL;
}
