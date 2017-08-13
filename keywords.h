#ifndef ASSEMBLER_KEYWORDS_H
#define ASSEMBLER_KEYWORDS_H

#endif //ASSEMBLER_KEYWORDS_H

#include "symbol_tree.h"
#include <stdlib.h>
#include <string.h>

typedef struct keyword *keyword_p;

typedef struct keyword {
    char *value;
    keyword_p left;
    keyword_p right;
} keyword;


void insert_keyword(char *value, keyword_p *head);

void init_keywords_list();

int is_keyword(char *word);

int search(char *word, keyword_p node);

