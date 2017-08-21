#include "first_pass.h"
#include <ctype.h>
#include "constants.h"
#include "keywords.h"
#include "error_handling.h"

static sym_pt search_symbol_by_label_from_given_node(char *label, sym_pt head);

static sym_pt symbol_head = NULL;

sym_pt create_symbol_node(char *label, int address, bool is_external, bool is_action) {
    sym_pt new_node;
    new_node = (sym_pt) malloc(sizeof(Symbol));
    check_allocation(new_node);
    new_node->label = strdup(label);
    new_node->address = address;
    new_node->is_external = is_external;
    new_node->is_action = is_action;
    new_node->right = NULL;
    new_node->left = NULL;
    return new_node;
}

void insert_symbol(char *label, int address, enum bool is_external, enum bool is_action) {
    sym_pt new_node;
    new_node = create_symbol_node(label, address, is_external, is_action);
    insert_symbol_after_node(new_node, &symbol_head);
}

void insert_symbol_after_node(sym_pt new_symbol, sym_pt *node) {
    int strcmp_result;
    if (!(*node)) {
        *node = new_symbol;
    } else if ((strcmp_result = strcmp(new_symbol->label, (*node)->label)) > 0) {
        insert_symbol_after_node(new_symbol, &((*node)->right));
    } else if (strcmp_result < 0) {
        insert_symbol_after_node(new_symbol, &((*node)->left));
    }
}


sym_pt search_symbol_by_label(char *label) {
    return search_symbol_by_label_from_given_node(label, symbol_head);
}

sym_pt search_symbol_by_label_from_given_node(char *label, sym_pt node) {
    int strcmp_result;
    if (node) {
        if ((strcmp_result = strcmp(label, node->label)) == 0) {
            return node;
        } else if (strcmp_result > 0) {
            return search_symbol_by_label_from_given_node(label, node->right);
        } else {
            return search_symbol_by_label_from_given_node(label, node->left);
        }
    } else
        return NULL;
}

char *get_label(char *line, int lines_count) {
    char *p = line;
    size_t label_length;
    label_length = 0;
    while (*p && *p != LABEL_SEPARATOR) {
        p++;
        label_length++;
    }
    if (*p != LABEL_SEPARATOR) {
        return NULL;
    }

    line[label_length] = STRING_NULL_TERMINATOR;
    if (label_is_valid(line, label_length, lines_count)) {
        return line;
    }

    return NULL;


}

bool label_is_valid(char *label, size_t label_length, int lines_count) {
    char c;
    if (label_length > MAX_LABEL) {
        handle_error(LABEL_TOO_LONG_ERROR, lines_count);
        return FALSE;
    }
    if (!isalpha(*label)) { /* label must start with an alpha char */
        handle_error(LABEL_FIRST_CHAR_IS_NOT_ALPHA_ERROR, lines_count);
        return FALSE;
    }

    while ((c = *label++)) {
        if (!isalnum(c)) {
            handle_error(LABEL_CONTAINS_INVALID_CHARACTER_ERROR, lines_count);
            return FALSE;
        }
    }

    if (is_opcode(label)) {
        handle_error(LABEL_IS_OPCODE_ERROR, lines_count);
        return FALSE;
    } else if (is_directive(label)) {
        handle_error(LABEL_IS_DIRECTIVE_ERROR, lines_count);
        return FALSE;
    } else if (is_register(label)) {
        handle_error(LABEL_IS_REGISTER_ERROR, lines_count);
        return FALSE;
    }

    return TRUE;
}

void increment_symbol_addresses_by_ic(int IC) {
    increment_symbol_addresses_by_ic_from_given_node(symbol_head, IC);
}

void increment_symbol_addresses_by_ic_from_given_node(sym_pt symbol, int IC) {
    if (symbol) {
        if (!symbol->is_action && !symbol->is_external) {
            symbol->address += IC + START_IC;
        } else if (symbol->is_action) {
            symbol->address += START_IC;
        }
        increment_symbol_addresses_by_ic_from_given_node(symbol->right, IC);
        increment_symbol_addresses_by_ic_from_given_node(symbol->left, IC);

    }
}


void clean_symbol_table() {
    clean_symbol_table_from_given_node(symbol_head);
    symbol_head = NULL;
}

void clean_symbol_table_from_given_node(sym_pt symbol) {
    if (symbol) {
        clean_symbol_table_from_given_node(symbol->right);
        clean_symbol_table_from_given_node(symbol->left);
        symbol->right = NULL;
        symbol->left = NULL;
        free(symbol);
    }
}
