#include "file_processing.h"
#include "keywords.h"
#include "data.h"
#include "first_pass.h"
#include "second_pass.h"
#include "extern_table.h"
#include "entry_table.h"
#include "constants.h"


extern int err_count;


void generate_output_files(char *filename);

void write_ob_file(char *filename);

void write_counts_to_ob_file(FILE *fp);

/* 11. Create and save the output files: .ob file, .ext file (if needed), .ent file (if needed). */

void process_file(char *filename) {
    FILE *fp;
    fp = open_file(filename, READ_MODE, AS_EXTENSION);
    if (!fp){
        return;
    }
    first_pass(fp);
    if (err_count)
        return;
    fseek(fp, 0, 0);
    second_pass(fp);
    if (err_count)
        return;
    fclose(fp);
    generate_output_files(filename);
    clean_symbol_table();
    clean_extern_table();
    clean_entry_table();
    clean_code();
    clean_data();
    err_count = 0;
}

void generate_output_files(char *filename) {
    if (!is_code_empty() || !is_data_empty()){
        write_ob_file(filename);
    }
    if (!is_extern_empty()){
        write_extern_file(filename);
    }
    if (!is_entry_empty()){
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
    char instructions_count[8 + 1];
    char data_count[8 + 1];
    char instructions_count_4base[(8 / 2) + 1];
    char data_count_4_base[(8 / 2) + 1];
    
    int_to_bin(get_data_count(), data_count, 8);
    bin_to_4base(data_count, data_count_4_base, 8);
    int_to_bin(get_insrtuctions_count(), instructions_count, 8);
    bin_to_4base(instructions_count, instructions_count_4base, 8);
    fputs(instructions_count_4base, fp);
    fputc('\t', fp);
    fputs(data_count_4_base, fp);
    fputc('\n', fp);
}

