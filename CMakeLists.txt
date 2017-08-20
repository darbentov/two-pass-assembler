cmake_minimum_required(VERSION 3.7)
project(assembler)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES main.c file_processing.c symbols.c opcode.c error_handling.c entry_table.c extern_table.c utils.h keywords.c keywords.h directives.c directives.h data.c data.h utils.c first_pass.c first_pass.h second_pass.c second_pass.h constants.h entry_table.h extern_table.h)
add_executable(assembler ${SOURCE_FILES})