#include <ctype.h>
#include <stdio.h>
#include "data.h"
#include "constants.h"
#include "symbols.h"
#include "keywords.h"

void int_to_bin(int num, char *word_p, int bit_length);

FILE *open_file(char *filename, char *mode, char *extension);

void check_allocation(void *ptr) {
    if (!ptr) {
        printf("Memory error. aborting...\n");
        exit(0);
    }
}

/*checks if line is empty*/
bool is_comment_or_empty(char *line) {
    char c;
    c = *line;
    if (!c || c == COMMENT_CHAR)
        return TRUE;
    return FALSE;
}


/*make a number to represent bits*/
void int_to_bin(int num, char *word_p, int bit_length) {
    unsigned int mask;
    int i;
    mask = 1;
    for (i = 0; i < bit_length; i++, word_p++) {
        *word_p = (char) (((num >> (bit_length - (i + 1))) & mask) + '0');
    }
}

FILE *open_file(char *filename, char *mode, char *extension) {
    /*returns Temp file*/
    FILE *fp = NULL;
    char *full_filename;
    full_filename = (char *) (malloc(strlen(filename) + strlen(extension) + 1));
    check_allocation(full_filename);
    /*Copy name from argv to filename string*/
    strcpy(full_filename, filename);
    /*Add .as extension to file.*/
    strcat(full_filename, extension);

    if (!(fp = fopen(full_filename, mode))) {
        printf("Can't open file %s.\n", full_filename);
        return NULL;
    }
    /*Returns pointer to file*/
    return fp;
}

/*converts to abcd base*/
void bin_to_4base(char *src, char *dst, int length) {
    int i;
    char zero;
    char cur[3];
    zero = '0';
    for (i = 0; i < length; i += 2) {
        cur[0] = (length >= (i + 2)) ? src[length - (i + 2)] : zero;
        cur[1] = src[length - (i + 1)];
        cur[2] = 0;
        if (strcmp(cur, "00") == 0) {
            dst[((length - i) / 2) - 1] = 'a';
        } else if (strcmp(cur, "01") == 0) {
            dst[((length - i) / 2) - 1] = 'b';
        } else if (strcmp(cur, "10") == 0) {
            dst[((length - i) / 2) - 1] = 'c';
        } else {
            dst[((length - i) / 2) - 1] = 'd';
        }
    }
    dst[length / 2] = 0;
}

/*duplicates string from 0 until n length */
char *my_strndup(char *src, size_t n) {
    char *new, *new_p;
    new = (char *) (malloc(sizeof(char) * n + 1));
    new_p = new;
    while (src && n--) {
        *new_p++ = *src++;
    }
    return new;
}

/*duplicates string from 0 until n length */
char *my_strdup(char *src) {
    size_t n;
    n = strlen(src);
    return my_strndup(src, n);
}
