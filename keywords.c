//
// Created by Dar Bentov on 12/8/17.
//

#include "keywords.h"


static keyword_p keywords_head = NULL;

void init_keywords_list() {
    insert_keyword("mov", &keywords_head);
    insert_keyword("cmp", &keywords_head);
    insert_keyword("add", &keywords_head);
    insert_keyword("sub", &keywords_head);
    insert_keyword("mul", &keywords_head);
    insert_keyword("div", &keywords_head);
    insert_keyword("lea", &keywords_head);
    insert_keyword("inc", &keywords_head);
    insert_keyword("dec", &keywords_head);
    insert_keyword("jnz", &keywords_head);
    insert_keyword("jnc", &keywords_head);
    insert_keyword("shl", &keywords_head);
    insert_keyword("prn", &keywords_head);
    insert_keyword("jsr", &keywords_head);
    insert_keyword("rts", &keywords_head);
    insert_keyword("hlt", &keywords_head);
    insert_keyword("data", &keywords_head);
    insert_keyword("string", &keywords_head);
    insert_keyword("mat", &keywords_head);
    insert_keyword("entry", &keywords_head);
    insert_keyword("extern", &keywords_head);
    insert_keyword("r0", &keywords_head);
    insert_keyword("r1", &keywords_head);
    insert_keyword("r2", &keywords_head);
    insert_keyword("r3", &keywords_head);
    insert_keyword("r4", &keywords_head);
    insert_keyword("r5", &keywords_head);
    insert_keyword("r6", &keywords_head);
    insert_keyword("r7", &keywords_head);
}

void insert_keyword(char *value, keyword_p *head) {
    int strcmp_result;
    if (!*head) {
        *head = (keyword_p) malloc(sizeof(keyword));
        (*head)->value = strdup(value);
        /* initialize the children to null */
        (*head)->left = 0;
        (*head)->right = 0;
    } else if ((strcmp_result = strcmp(value, (*head)->value)) > 0) {
        insert_keyword(value, &(*head)->right);
    } else if (strcmp_result < 0) {
        insert_keyword(value, &(*head)->left);
    } else {
        return;
    }
}

int is_keyword(char *word) {
    return search(word, keywords_head);

}

int search(char *word, keyword_p node) {
    int strcmp_result;
    if (node != 0) {
        if ((strcmp_result = strcmp(word, node->value)) == 0) {
            return 1;
        } else if (strcmp_result > 1) {
            return search(word, node->right);
        } else {
            return search(word, node->left);
        }
    } else return 0;
}