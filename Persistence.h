#pragma once

#ifndef MATRIX_PERSISTENCE_H
#define MATRIX_PERSISTENCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "common.h"
#include "Matrix.h"

//// State definitions
//#define STATE_OUT_OF_OPERATION      0
//#define STATE_INSIDE_OPERATION      1
//#define STATE_IN_OPERAND_MATRIX     
//#define STATE_NEXT_OPERAND          3
//#define STATE_IN_RESULT_MATRIX      4
//
//// Matrix reading states
//#define READ_OPERAND_MATRIX 10
//#define READ_RESULT_MATRIX  11

// Constants
//#define MATRIX_COUNT_PER_OPERATION 3
//#define MATRIX_HEADER_SIZE 5
//#define MAX_LINE 10000
//#define DEBUG_1 1

struct OperandHeader {
	int matrix_id;
	int operation_id;
	int row_count;
	int col_count;
	int matrix_type; // 0 for operand, 1 for result
};

// Function declarations

int				extract_operand_header(char* _line, OperandHeader* _operand_header);

unsigned short*	extract_operand_matrix_row(char* _line, unsigned short _col_count, unsigned short* _values);
unsigned int*	extract_result_matrix_row(char* _line, unsigned short _col_count, unsigned int* _values);
//Matrix*		extract_matrix_operand(char* _line, int _matrix_id, int _operand_id, int _row_count, int _col_count, 
//					int _matrix_type);

int				save_matrix_tofile(FILE* file, int _operation_id, int operand_no, Matrix* _matrix);
FILE*			open_matrix_file(const char* filename, const char* option);
int 			close_matrix_file(FILE* _file);
char*			read_rawline(FILE* file, char* buffer, int size);


Operation*		load_matrix_operations();
int				save_matrix_operations(Operation* _operation_list, FILE* _file_to_write_to, const char* _filename);

// test functions
int				test_load_matrix_operations(); 
int				test_save_matrix_operations(); 
Operation*		load_matrix_test_data(FILE* _file, Operation* _operation_list);

#endif // MATRIX_PERSISTENCE_H