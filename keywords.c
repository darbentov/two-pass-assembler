#include "keywords.h"

static keyword_p registers_head = NULL;
static keyword_p opcodes_head = NULL;
static keyword_p directives_head = NULL;

void init_keywords_list() {

    insert_keyword("mov", &opcodes_head);
    insert_keyword("cmp", &opcodes_head);
    insert_keyword("add", &opcodes_head);
    insert_keyword("sub", &opcodes_head);
    insert_keyword("mul", &opcodes_head);
    insert_keyword("div", &opcodes_head);
    insert_keyword("lea", &opcodes_head);
    insert_keyword("inc", &opcodes_head);
    insert_keyword("dec", &opcodes_head);
    insert_keyword("jnz", &opcodes_head);
    insert_keyword("jnc", &opcodes_head);
    insert_keyword("shl", &opcodes_head);
    insert_keyword("prn", &opcodes_head);
    insert_keyword("jsr", &opcodes_head);
    insert_keyword("rts", &opcodes_head);
    insert_keyword("hlt", &opcodes_head);


    insert_keyword("data", &directives_head);
    insert_keyword("string", &directives_head);
    insert_keyword("mat", &directives_head);
    insert_keyword("entry", &directives_head);
    insert_keyword("extern", &directives_head);


    insert_keyword("r0", &registers_head);
    insert_keyword("r1", &registers_head);
    insert_keyword("r2", &registers_head);
    insert_keyword("r3", &registers_head);
    insert_keyword("r4", &registers_head);
    insert_keyword("r5", &registers_head);
    insert_keyword("r6", &registers_head);
    insert_keyword("r7", &registers_head);
}

void insert_keyword(char *value, keyword_p *head) {
    int strcmp_result;
    if (!*head) {
        *head = (keyword_p) malloc(sizeof(Keyword));
        (*head)->value = my_strdup(value);
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

short int is_register(char *word) {
    keyword_p key;
    key = search(word, registers_head);
    return key != NULL;
}

short int is_opcode(char *word) {
    return search(word, opcodes_head) != NULL;
}

short int is_directive(char *word) {
    return search(word, directives_head) != NULL;
}

keyword_p search(char *word, keyword_p node) {
    int strcmp_result;
    if (node) {
        strcmp_result = strcmp(word, node->value);
        if (strcmp_result == 0) {
            return node;
        } else if (strcmp_result > 0) {
            return search(word, node->right);
        } else {
            return search(word, node->left);
        }
    } else
        return NULL;
}
