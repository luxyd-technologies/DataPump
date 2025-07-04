#pragma once

#ifndef MATRIX_PERSISTENCE_H
#define MATRIX_PERSISTENCE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "common.h"
#include "Matrix.h"

// State definitions
#define STATE_OUT_OF_OPERATION      0
#define STATE_INSIDE_OPERATION      1
#define STATE_IN_OPERAND_MATRIX     2
#define STATE_NEXT_OPERAND          3
#define STATE_IN_RESULT_MATRIX      4

// Matrix reading states
#define READ_OPERAND_MATRIX 10
#define READ_RESULT_MATRIX  11

// Constants
#define MATRIX_COUNT_PER_OPERATION 3
#define MATRIX_HEADER_SIZE 5
#define MAX_LINE 10000
#define DEBUG_1 1

// Function declarations
void debug_print(const char* _message_format, const char* _message);
unsigned short* extract_operand_matrix_row(char* _line, unsigned short _col_count, unsigned short* _values);
unsigned int* extract_result_matrix_row(char* _line, unsigned short _col_count, unsigned int* _values);
Matrix* extract_matrix_operand(char* _line, int _matrix_id, int _operand_id, int _row_count, int _col_count, int _matrix_type);
FILE* open_matrix_file(const char* filename, const char* option);
char* read_rawline(FILE* file, char* buffer, int size);
int test_persistence(char* _filename, Operation* _operations);

#endif // MATRIX_PERSISTENCE_H