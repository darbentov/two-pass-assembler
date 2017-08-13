#ifndef ASSEMBLER_OPCODE_H
#define ASSEMBLER_OPCODE_H

#endif /* ASSEMBLER_OPCODE_H */

#define OPCODES_COUNT 16
#define MAX_OPCODE_NAME 10
#define MAX_OPCODE_BITS 4

enum {
    OPS_2, OP_1, NO_OP
};

typedef struct Opcode {             /* Opcode node. it will be in an hash list, and opcodes with the same hash will be in a linked list */
    char name[MAX_OPCODE_NAME];
    char code[MAX_OPCODE_BITS];
    enum op_type;
    Opcode *next;

} Opcode;

Opcode *hash_table[OPCODES_COUNT] = {NULL};

int getHashIndex(char *name);

void init_opcode_hash_table();

void insertOpCodeNode(Opcode *node, int index);

void insertIntoHashMap(Opcode *Node);

Opcode *getOpcodeNode(char *op);