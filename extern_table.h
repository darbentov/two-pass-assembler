#ifndef ASSEMBLER_EXTERN_TABLE_H
#define ASSEMBLER_EXTERN_TABLE_H


typedef struct extern_obj *extern_pt;

typedef struct extern_obj {
    char *label;
    int address;
    extern_pt next;
} ExternObj;

void insert_extern(char *label, int address);

void clean_extern_table();

#endif
