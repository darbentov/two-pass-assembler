#ifndef ASSEMBLER_OPCODE_H
#define ASSEMBLER_OPCODE_H

#endif /* ASSEMBLER_OPCODE_H */

#include <stdlib.h>

#define OPCODES_COUNT 16
#define MAX_OPCODE_NAME 10
#define MAX_OPCODE_BITS 4

typedef enum {
    OPS_2, OP_1, NO_OP
} op_type;

typedef struct Opcode *opcode_pt;

typedef struct Opcode {             /* Opcode node. it will be in an hash list, and opcodes with the same hash will be in a linked list */
    char name[MAX_OPCODE_NAME];
    char code[MAX_OPCODE_BITS];
    op_type type;
    opcode_pt next;

} Opcode;

Opcode *hash_table[OPCODES_COUNT] = {NULL};

int getHashIndex(char *name);

void init_opcode_hash_table();

void insertOpCodeNode(Opcode *node);

Opcode *getOpcodeNode(char *op);