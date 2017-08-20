#include "file_processing.h"
#include "keywords.h"
#include "data.h"
#include "first_pass.h"
#include "second_pass.h"
#include "extern_table.h"
#include "entry_table.h"


extern int err_count;


void generate_output_files(char *filename);
FILE * open_file(char* filename, char * mode, char * extension);

void write_ob_file(char *filename);

/* 11. Create and save the output files: .ob file, .ext file (if needed), .ent file (if needed). */

void process_file(char *filename) {
    FILE *fp;
    fp = open_file(filename, "r", ".as");
    if (!fp){
        return;
    }
    printf("Starting first pass.\n");
    first_pass(fp);
    printf("Done first pass. err_count: %d\n\n\n\n\n", err_count);
    if (err_count)
        return;
    fseek(fp, 0, 0);
    printf("Starting second pass.\n");
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
}

void write_ob_file(char *filename) {
    FILE *fp;
    fp = open_file(filename, "w", ".ob");
    write_code_to_ob_file(fp);
    fputs("Data:\n", fp);
    write_data_to_ob_file(fp);
    fclose(fp);

}

FILE * open_file(char* filename, char * mode, char * extension)
{
    /*Temp file to be returned*/
    FILE * fp = NULL;
    /*Full filename string*/
    char *full_filename;
    full_filename = (char *)(malloc(strlen(filename) + 3));
    check_allocation(full_filename);
    /*Copy the filename from argv to the full filename string*/
    strcpy(full_filename, filename);
    /*Add the .as extension to the full filename.*/
    strcat(full_filename, extension);
    /*Open the file. if fopen return null, exit the function and print error to screen*/
    if (!(fp = fopen(full_filename, mode)))
    {
        printf("Can't open file %s.\n", full_filename);
        return NULL;
    }
    /*Return pointer to the file*/
    return fp;
}
