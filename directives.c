#include "directives.h"
#include <string.h>

int find_directive_type(char *directive_name) {
    int i;
    for (i = 0; i < directive_count; i++) {
        if (strcmp(directives[i].name, directive_name) == 0) {
            return directives[i].type;
        }
    }
    return NOT_EXISTS_DIRECTIVE_TYPE;
}
