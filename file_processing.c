#include "file_processing.h"
#include "keywords.h"
#include "data.h"
#include "first_pass.h"
#include "second_pass.h"
#include "extern_table.h"
#include "entry_table.h"
#include "constants.h"


/* declaring an extern variable of errors count */

extern int err_count;


void generate_output_files(char *filename);

void write_ob_file(char *filename);

void write_counts_to_ob_file(FILE *fp);

/* 11. Create and save the output files: .ob file, .ext file (if needed), .ent file (if needed). */

void process_file(char *filename) {
    FILE *fp;
    /* open file for read with .as extension */
    fp = open_file(filename, READ_MODE, AS_EXTENSION);
    if (!fp) {
        return;
    }
    printf("Processing file: %s%s\n", filename, AS_EXTENSION);
    first_pass(fp);
    /* point to head of file, so second pass will run from the start of the file */
    fseek(fp, 0, 0);
    /* If errors found, don't do the second pass */
    if (!err_count) {
        second_pass(fp);
    }
    fclose(fp);
    if (!err_count) {
        generate_output_files(filename);
    }

    /* Cleaning The data for the next file */
    clean_symbol_table();
    clean_extern_table();
    clean_entry_table();
    clean_code();
    clean_data();
    err_count = 0;
}

void generate_output_files(char *filename) {
    if (!is_code_empty() || !is_data_empty()) {
        write_ob_file(filename);
    }
    if (!is_extern_empty()) {
        write_extern_file(filename);
    }
    if (!is_entry_empty()) {
        write_entry_file(filename);
    }
}

void write_ob_file(char *filename) {
    FILE *fp;
    fp = open_file(filename, WRITE_MODE, OB_EXTENSION);
    write_counts_to_ob_file(fp);
    write_code_to_ob_file(fp);
    write_data_to_ob_file(fp);
    fclose(fp);

}

void write_counts_to_ob_file(FILE *fp) {
    char instructions_count[BITS_COUNT_OF_AN_INT + 1];
    char data_count[BITS_COUNT_OF_AN_INT + 1];
    char instructions_count_4base[(BITS_COUNT_OF_AN_INT / 2) + 1];
    char data_count_4_base[(BITS_COUNT_OF_AN_INT / 2) + 1];

    /* convert data count (int) to 4-odd-base string */
    int_to_bin(get_data_count(), data_count, BITS_COUNT_OF_AN_INT);
    bin_to_4base(data_count, data_count_4_base, BITS_COUNT_OF_AN_INT);

    /* convert instructions count (int) to 4-odd-base string */
    int_to_bin(get_insrtuctions_count(), instructions_count, BITS_COUNT_OF_AN_INT);
    bin_to_4base(instructions_count, instructions_count_4base, BITS_COUNT_OF_AN_INT);

    /* writing instructions count to first row of file */
    fputs(instructions_count_4base, fp);
    /* put a tab seperator */
    fputc(TAB_SEPERATOR, fp);
    /* writing data count to first row of file */
    fputs(data_count_4_base, fp);
    /* put line break char */
    fputc(LINE_BREAK, fp);
}
