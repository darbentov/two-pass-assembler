#include <stdio.h>
#include "file_processing.h"
#include "opcode.h"
#include "keywords.h"

#define UASGE_MESSAGE "Usage: assemble an assembly code. by files provided in the terminal."

int main(int argc, char **argv) {
    if (argc == 1) {
        printf(UASGE_MESSAGE);
    }
    init_opcode_hash_table();
    init_keywords_list();
    while (--argc) {
        process_file(*++argv);

    }
}


