#include "file_processing.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "keywords.h"
#include "opcode.h"
#include "directives.h"
#include "data.h"
#include "first_pass.h"


int IC, DC, lines_count;
extern int err_count;



void process_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    first_pass(fp);
    if (err_count)
        return;
    fseek(fp, 0, 0);
    second_pass(fp);
    if (err_count)
        return;
    generate_output_files();
    clean_symbol_table();
    clean_externs_table();
    clean_entries_table();
    clean_code();
    clean_data();
    err_count = 0;
}
