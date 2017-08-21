#include "directives.h"
#include <string.h>

/*** Find the directive type by given directive name ***
 *
 *   If directive not found, return non-exists directive flag
 *
 */
int find_directive_type(char *directive_name) {
    int i;
    for (i = 0; i < directive_count; i++) {
        if (strcmp(directives[i].name, directive_name) == 0) {
            return directives[i].type;
        }
    }
    return NOT_EXISTS_DIRECTIVE_TYPE;
}
