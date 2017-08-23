#ifndef ASSEMBLER_DIRECTIVES_H
#define ASSEMBLER_DIRECTIVES_H


#define NOT_EXISTS_DIRECTIVE_TYPE -1
#define DATA_DIRECTIVE_TYPE 1
#define MAT_DIRECTIVE_TYPE 2
#define STRING_DIRECTIVE_TYPE 3
#define ENTRY_DIRECTIVE_TYPE 4
#define EXTERN_DIRECTIVE_TYPE 5

typedef struct directive {
    char *name;
    int type;
} directive;


int find_directive_type(char *directive_name);

#endif
