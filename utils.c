#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

void check_allocation(void *ptr)
{
    if (!ptr) {
        printf("Memory error. aborting...\n");
        exit(0);
    }
}

short int is_comment_or_empty(char *line) {
    char c;
    c = *line;
    if (!c || c == COMMENT_CHAR)
        return 1;
    return 0;
}

short int is_matrix(char *word){

}