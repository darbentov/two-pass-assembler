#ifndef ASSEMBLER_SYMBOL_TREE_H
#define ASSEMBLER_SYMBOL_TREE_H

#endif /* ASSEMBLER_SYMBOL_TREE_H */

#define MAX_LABEL 51

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

sym_pt create_symbol_node(char *label, int address, bool is_external, bool is_action);

void insert_symbol(sym_pt new_node);

void insert_symbol_after_node(sym_pt new_node, sym_pt *head);

sym_pt search_symbol_by_label(char *label);

static sym_pt search_symbol_by_label_from_given_node(char *label, sym_pt head);

char *get_label(char *line, int lines_count);

bool label_is_valid(char *label, size_t label_length, int lines_count);