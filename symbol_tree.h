#ifndef ASSEMBLER_SYMBOL_TREE_H
#define ASSEMBLER_SYMBOL_TREE_H

#endif /* ASSEMBLER_SYMBOL_TREE_H */

#define MAX_LABEL 50

#include "utils.h"

typedef struct Symbol *sym_pt;

typedef struct Symbol {
    char *label;
    int address;
    bool is_external;
    bool is_action;
    sym_pt left;
    sym_pt right;
} Symbol;

void insert_label_node(char *label, sym_pt *head);
sym_pt search_label(char *label, sym_pt head);



