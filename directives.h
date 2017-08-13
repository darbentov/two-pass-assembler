//
// Created by Dar Bentov on 12/8/17.
//

#ifndef ASSEMBLER_DIRECTIVES_H
#define ASSEMBLER_DIRECTIVES_H

#endif //ASSEMBLER_DIRECTIVES_H

#include <stdlib.h>
#include <string.h>
#define NOT_EXISTS_DIRECTIVE_TYPE -1

#define DATA_DIRECTIVE_TYPE 1
#define MAT_DIRECTIVE_TYPE 2
#define STRING_DIRECTIVE_TYPE 3
#define ENTRY_DIRECTIVE_TYPE 4
#define EXTERN_DIRECTIVE_TYPE 5
typedef struct directive *directive_p;

typedef struct directive {
    char *name;
    int type;
} directive;

static directive directives[] = {
        {"data",   DATA_DIRECTIVE_TYPE},
        {"mat",    MAT_DIRECTIVE_TYPE},
        {"string", STRING_DIRECTIVE_TYPE},
        {"entry",  ENTRY_DIRECTIVE_TYPE},
        {"extern", EXTERN_DIRECTIVE_TYPE}

};

int find_directive_type(char *directive_name);

