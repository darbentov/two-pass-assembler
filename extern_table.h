#ifndef ASSEMBLER_EXTERN_TABLE_H
#define ASSEMBLER_EXTERN_TABLE_H
#endif //ASSEMBLER_EXTERN_TABLE_H

typedef struct extern_obj *extern_pt;

typedef struct extern_obj {
    char *label;
    int address;
    extern_pt next;
} ExternObj;

