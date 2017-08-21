#include "first_pass.h"
#include "extern_table.h"
#include "constants.h"
#include "symbols.h"
#include <stdlib.h>
#include <string.h>

static extern_pt extern_head = NULL;
static short int extern_count;


void insert_extern(char *extern_label, int address) {
    extern_pt new, tmp;
    new = (extern_pt) (malloc(sizeof(ExternObj)));
    new->label = strdup(extern_label);
    new->address = address;
    new->next = NULL;
    if (!extern_head) {
        extern_head = new;
    } else {
        tmp = extern_head;
        while (tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = new;
    }
    extern_count++;
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
    extern_count = 0;
}

bool is_extern_empty() {
    return (bool) (extern_count == 0);
}

void write_extern_file(char *filename) {
    FILE *fp;
    char address[9], address_4_base[5];
    fp = open_file(filename, WRITE_MODE, EXTERN_EXTENSION);
    extern_pt tmp;
    tmp = extern_head;
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

void insert_extern_to_symbol_table(char *label, int line_number) {
    if (label_is_valid(label, strlen(label), line_number)) {
        insert_symbol(label, 0, TRUE, FALSE);
    }
}