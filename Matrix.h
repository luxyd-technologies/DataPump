#pragma once

#ifndef MATRIX_MATMUL_H
#define MATRIX_MATMUL_H

// Matrix structure and constants
//struct Matrix;
#define MATX_OP_FAILURE 0
#define MATX_OP_INCOMPATIBLE 1
#define MATX_OP_SUCCESS 9

#define MATRIX_TYPE_OPERAND 0
#define MATRIX_TYPE_RESULT 1

#define MAX_OPERATION_COUNT 10
#define MAX_MATRIX_ROW_COUNT 1000
#define MAX_MATRIX_COL_COUNT 1000


struct Matrix {
    unsigned short int matrix_id; // Unique identifier for the matrix
    unsigned short int operand_id; // Identifier for the operand
    unsigned short int rows;
    unsigned short int cols;
    unsigned short int* ushort_data;
    unsigned int* uint_data;
    unsigned int matrix_type; // default to operand type

    //unsigned short int data[rows][cols]; // Using a 2D array for simplicity, but this is not dynamic
};

struct Operation {
    unsigned int operation_id; // Unique identifier for the operation
    Matrix* operand1;   // Pointer to the first operand matrix
    Matrix* operand2;   // Pointer to the second operand matrix
    Matrix* result;     // Pointer to the result matrix
};

struct MatrixHeader {
    unsigned short int matrix_id; // Unique identifier for the matrix
    unsigned short int operand_id; // Identifier for the operand
    unsigned short int rows;
    unsigned short int cols;
    unsigned short int matrix_type; // default to operand type
};

// Function prototypes

Operation*		create_operation(unsigned int _operation_id, Matrix* _matrix_operand1, Matrix* _matrix_operand2, Matrix* _matrix_result);
void            operation_print_info(Operation* op);
int             save_operation_to_file(const char* filename, Operation* _operation);
void            free_operation(Operation* op);

Matrix*         create_matrix(int _matrix_id, int _operand_id, int _rows, int _cols, int _matrix_type);
unsigned short  matrix_get_ushort(const Matrix* m, int row, int col);
unsigned int    matrix_get_uint(const Matrix* m, int row, int col);
void            matrix_set_ushort(Matrix* m, int row, int col, unsigned short int value);
void            matrix_set_uint(Matrix* m, int row, int col, unsigned int value);
void            matrix_print_info(Matrix* m);
int             save_matrix_tofile(FILE* file, Matrix* _matrix, int _operation_id);
void            free_matrix(Matrix* m);

unsigned short	generate_random_ushort(void);
unsigned short	init_matrix_operand1(Matrix* mat);
unsigned short	init_matrix_operand2(Matrix* mat);
unsigned int    multiply_matrices(Matrix* _matrix1, Matrix* _matrix2, Matrix* _matrix_result);

int             print_message(int message_type, char* message);



//Matrix* create_matrix(int _matrix_id, int _operand_id, int _rows, int _cols, int _matrix_type);
//void free_matrix(Matrix* m);
//double matrix_get_ushort(const Matrix* m, int row, int col);
//double matrix_get_uint(const Matrix* m, int row, int col);
//void matrix_set_ushort(Matrix* m, int row, int col, unsigned short int value);
//void matrix_set_uint(Matrix* m, int row, int col, unsigned short int value);
//unsigned int multiply_matrices(Matrix* _matrix1, Matrix* _matrix2, Matrix* _matrix_result);
//
//void print_matrix(const Matrix* mat, int print_type);

#endif