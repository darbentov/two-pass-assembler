#include "symbol_tree.h"
#include <string.h>
#include <stdlib.h>


void insert_label_node(char *label, sym_pt *head) {
    int strcmp_result;
    if (*head == 0) {
        *head = (sym_pt) malloc(sizeof(Symbol));
        (*head)->label = strdup(label);
        /* initialize the children to null */
        (*head)->left = 0;
        (*head)->right = 0;
    } else if ((strcmp_result = strcmp(label, (*head)->label)) > 0) {
        return insert_label_node(label, &(*head)->right);
    } else if (strcmp_result < 0) {
        return insert_label_node(label, &(*head)->left);
    } else {
        return;
    }
}

sym_pt search_label(char *label, sym_pt node) {
    int strcmp_result;
    if (node != 0) {
        if ((strcmp_result = strcmp(label, node->label)) == 0) {
            return node;
        } else if (strcmp_result > 1) {
            return search_label(label, node->right);
        } else {
            return search_label(label, node->left);
        }
    } else
        return 0;
}