#pragma once

#ifndef MATRIX_H
#define MATRIX_H

#include "common.h"

struct MatrixData {
    //unsigned short int  rows; // Number of rows in the matrix
    //unsigned short int  cols; // Number of columns in the matrix
    short               total_allocated_memory_bytes; // Total allocated memory in bytes
    short               variable_type = VAR_TYPE_MATRIX_DATA;
    short               data_type;
    unsigned short*     ushort_data = nullptr;
    unsigned int*       uint_data = nullptr;

    // total memory footprint for matrix data 16 + 16 + 32 + 32+ 32 + 32 = 160 bytes
};

struct Matrix {
    int                 variable_type = VAR_TYPE_MATRIX;
    unsigned short int  matrix_id; // Unique identifier for the matrix
    unsigned short int  operation_id; // Identifier for the operand
    unsigned short int  rows;
    unsigned short int  cols;
    unsigned int        matrix_type; // default to operand type
    unsigned int        total_allocated_memory_bytes; // Total allocated memory in bytes
    MatrixData*         data; // Pointer to the matrix data
};

struct Operation {
    int                 variable_type = VAR_TYPE_OPERATION;
    unsigned int        operation_id; // Unique identifier for the operation
    unsigned int        total_allocated_memory_bytes; // Total allocated memory in bytes
    Matrix* operand1;   // Pointer to the first operand matrix
    Matrix* operand2;   // Pointer to the second operand matrix
    Matrix* result;     // Pointer to the result matrix
    Operation*          next = nullptr;
};

// member access functions
unsigned short          matrix_get_ushort(const Matrix* m, int row, int col);
unsigned int            matrix_get_uint(const Matrix* m, int row, int col);

void                    matrix_set_ushort(Matrix* m, int row, int col, unsigned short int value);
void                    matrix_set_uint(Matrix* m, int row, int col, unsigned int value);

// I/O functions
int                     matrix_print_info(Matrix* m);
int                     operation_print_info(Operation* op);

// initialization and creation functions
Matrix*                 init_matrix_random(Matrix* _matrix);
Matrix*                 init_matrix_zeroes(Matrix* _matrix);

// calculation and other transformation functions


MatrixData*             create_matrix_data(int _data_type, int _rows, int _cols);
Matrix*                 create_matrix(short _mode, unsigned short _operation_id, unsigned short _matrix_id,
                            unsigned short int _rows, unsigned short int _cols, int _matrix_type);
Operation*              create_operation(int _operation_id, Matrix* _operand1, Matrix* _operand2, Matrix* _result_matrix);
Operation*              create_operation_list();

Matrix*                 calc_matrix_multiplication(Matrix* _operand1, Matrix* _operand2, Matrix* _result);
Operation*              generate_matrix_test_data(/*ProgramArguments* _arguments,  Operation* _operations, */
                            int _operation_count,
                            int _matrix1_row_count, int _matrix1_col_count,
	                        int _matrix2_row_count, int _matrix2_col_count);

//Matrix* create_matrix(int _matrix_id, int _operation_id, int _rows, int _cols, int _matrix_type);


//test functions


#endif // MATRIX_H


