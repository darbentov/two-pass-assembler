cmake_minimum_required(VERSION 3.7)
project(assembler)

set(CMAKE_C_STANDARD 90)

set(SOURCE_FILES main.c file_processing.c symbol_tree.c opcode.c error_handling.c entry_table.c extern_table.c utils.h keywords.c keywords.h directives.c directives.h)
add_executable(assembler ${SOURCE_FILES})