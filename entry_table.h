#ifndef ASSEMBLER_ENTRY_TABLE_H
#define ASSEMBLER_ENTRY_TABLE_H

typedef struct entry *entry_pt;

typedef struct entry {
    char *label;
    int address;
    entry_pt next;
} Entry;

void insert_entry(char *label, int address);

void clean_entry_table();

#endif
