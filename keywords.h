#ifndef ASSEMBLER_KEYWORDS_H
#define ASSEMBLER_KEYWORDS_H

#include "symbols.h"
#include <stdlib.h>
#include <string.h>


typedef struct keyword *keyword_p;

typedef struct keyword {
    char *value;
    keyword_p left;
    keyword_p right;
} Keyword;


void insert_keyword(char *value, keyword_p *head);

void init_keywords_list();

short int is_directive(char *word);

short int is_register(char *word);

short int is_opcode(char *word);

keyword_p search(char *word, keyword_p node);

#endif
