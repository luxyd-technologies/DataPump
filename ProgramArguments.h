#include <iostream>
#include "common.h"

#ifndef PROGRAM_ARGUMENTS_H
#define PROGRAM_ARGUMENTS_H


struct ProgramArguments {

    int     generate_matrix_flag = 0;            // generate matrix test data file
    int     load_file_flag = 0;            // load and read matrix test data file
    int     n_flag = 0;
    int     operation_count = 0;   //default_operation to generate

    int     r1_flag = 0;           // matrix 1 row
    int     c1_flag = 0;           // matrix 1 column 

    int     mat1_row_count = 0;    // row count
    int     mat1_col_count = 0;    // column count

    int     r2_flag = 0;           // matrix 2 row
    int     c2_flag = 0;           // matrix 1 column

    int     mat2_row_count = 0;    // row count
    int     mat2_col_count = 0;    // column count

    int     f_flag = 0;            // Filename flag
    char* filename = nullptr;    // Filename

};


// functions
ProgramArguments* initialize_program_arguments(void);
int parse_arguments(int argc, char* argv[], ProgramArguments* _arguments);


// test functions
int test_initialize_program_arguments();
int test_parse_arguments();

#endif 