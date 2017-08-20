#include "first_pass.h"
#include <ctype.h>
#include "symbols.h"
#include "constants.h"
#include "keywords.h"
#include "error_handling.h"

static sym_pt search_symbol_by_label_from_given_node(char *label, sym_pt head);

static void print_all_symbols_from_given_node(sym_pt symbol);

sym_pt symbol_head = NULL;

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

void insert_symbol(sym_pt new_node) {
    insert_symbol_after_node(new_node, &symbol_head);
}

void insert_symbol_after_node(sym_pt new_node, sym_pt *head) {
    int strcmp_result;
    if (!(*head)) {
        *head = new_node;
    } else if ((strcmp_result = strcmp(new_node->label, (*head)->label)) > 0) {
        insert_symbol_after_node(new_node, &((*head)->right));
    } else if (strcmp_result < 0) {
        insert_symbol_after_node(new_node, &((*head)->left));
    }
}


sym_pt search_symbol_by_label(char *label) {
    return search_symbol_by_label_from_given_node(label, symbol_head);
}

sym_pt search_symbol_by_label_from_given_node(char *label, sym_pt node) {
    int strcmp_result;
    if (node != 0) {
        if ((strcmp_result = strcmp(label, node->label)) == 0) {
            return node;
        } else if (strcmp_result > 0) {
            return search_symbol_by_label_from_given_node(label, node->right);
        } else {
            return search_symbol_by_label_from_given_node(label, node->left);
        }
    } else
        return 0;
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

    line[label_length] = 0;
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
            symbol->address += IC;
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

void print_all_symbols() {
    print_all_symbols_from_given_node(symbol_head);
}

static void print_all_symbols_from_given_node(sym_pt symbol) {
    if (symbol) {
        printf("symbol: %s\n", symbol->label);
        print_all_symbols_from_given_node(symbol->right);
        print_all_symbols_from_given_node(symbol->left);
    }
}
