// MatMul1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>   // For malloc, calloc, free
#include <stdlib.h>
#include <stdexcept> // For std::out_of_range
#include "Matrix.h"
#include "common.h"

//using namespace std;

Operation* create_operation(unsigned int _operation_id, Matrix* _matrix_operand1, Matrix* _matrix_operand2, Matrix* _matrix_result) {
    Operation* op = static_cast<Operation*>(malloc(sizeof(Operation)));
    if (!op) return NULL; // Memory allocation failed
    op->operation_id = _operation_id;
    op->operand1 = _matrix_operand1;
    op->operand2 = _matrix_operand2;
    op->result = _matrix_result;
    return op;
}

void operation_print_info(Operation* op) {
    if (op) {
        printf(" ---- \nOperation ID: %u", op->operation_id);
        printf("\nOperand 1:Matrix");
        matrix_print_info(op->operand1);
        printf("\nOperand 2 Matrix:");
        matrix_print_info(op->operand2);
        printf("\nResult Matrix:");
        matrix_print_info(op->result);
        printf(" ---- \n");
    }
    else {
        printf("Operation is NULL\n");
    }
}

int save_operation_to_file(const char* filename, Operation* _operation) {
    if (!_operation || !filename) return -1; // Invalid operation or filename
    FILE* file = fopen(filename, "a");
    if (!file) return -1; // File opening failed
	fwrite("[\n", sizeof(char), strlen("[\n"), file);       // Write operation start marker 
	save_matrix_tofile(file, _operation->operand1);         // write operand1 matrix to file    
	save_matrix_tofile(file, _operation->operand2);         // write operand2 matrix to file
	save_matrix_tofile(file, _operation->result);           // write result matrix to file
    fwrite("]\n", sizeof(char), strlen("]\n"), file);       // Write operation end market 
    fclose(file);
    return 0; // Success
}

void free_operation(Operation* op) {
    if (op) {
        free(op->operand1); // Free operand1 matrix
        free(op->operand2); // Free operand2 matrix
        free(op->result);   // Free result matrix
        free(op);          // Finally, free the operation struct
    }
}

// Safe element access with bounds checking
unsigned short matrix_get_ushort(const Matrix* m, int row, int col) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    return m->ushort_data[row * m->cols + col];
}

// Safe element access with bounds checking
unsigned int matrix_get_uint(const Matrix* m, int row, int col) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    return m->uint_data[row * m->cols + col];
}

 //Safe element modification with bounds checking
void matrix_set_ushort(Matrix* m, int row, int col, unsigned short int value) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    m->ushort_data[row * m->cols + col] = value;
}

// Safe element modification with bounds checking
void matrix_set_uint(Matrix* m, int row, int col, unsigned int value) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
        throw std::out_of_range("Matrix index out of bounds");
    }
    m->uint_data[row * m->cols + col] = value;
}

// Allocates matrix memory (initialized to zero)
Matrix* create_matrix(int _matrix_id, int _operand_id, int _rows, int _cols, int _matrix_type) {

    Matrix* m = static_cast<Matrix*>(malloc(sizeof(Matrix)));
    if (!m) return nullptr;

    m->matrix_id = _matrix_id;
    m->operand_id = _operand_id;
    m->rows = _rows;
    m->cols = _cols;

    if (_matrix_type == MATRIX_TYPE_OPERAND) {
        m->matrix_type = MATRIX_TYPE_OPERAND;
        m->ushort_data = static_cast<unsigned short int*>(calloc(_rows * _cols, sizeof(unsigned short int)));
    }
    else if (_matrix_type == MATRIX_TYPE_RESULT) {
        m->matrix_type = MATRIX_TYPE_RESULT;
        m->uint_data = static_cast<unsigned  int*>(calloc(_rows * _cols, sizeof(unsigned  int)));
    }
    else {
        free(m);
        return nullptr; // Invalid matrix type
    }

    if ((!m->ushort_data) && (!m->uint_data)) {
        free(m);
        return nullptr;
    }
    return m;
}

void matrix_print_info(Matrix* m) {
    if (m) {
        //printf("\nMatrix ID: %d\t Operand Id:%d\t Dimension: %d x %d\tMatrix Type: %d\n", m->matrix_id, m->operand_id, m->rows, m->cols, m->matrix_type);
        for (int i = 0; i < m->rows; ++i) {
            for (int j = 0; j < m->cols; ++j) {
                if (m->matrix_type == MATRIX_TYPE_OPERAND) {
                    //printf("%d ", m->ushort_data[i * m->cols + j]);
                }
                else if (m->matrix_type == MATRIX_TYPE_RESULT) {
                    //printf("%u ", m->uint_data[i * m->cols + j]);
                }
            }
            //printf("\n");
        }

    }
    else {
        printf("Matrix is NULL\n");
    }
}


int save_matrix_tofile(FILE* file, Matrix* _matrix ) {

    if (!_matrix || !file) return -1; // Invalid matrix or file pointer
	printf("Saving matrix to file...\n");

    if (_matrix->matrix_type == MATRIX_TYPE_OPERAND) {
        fwrite("+\n", sizeof(char), strlen("+\n"), file);
   

        fwrite("*\n", sizeof(char), strlen("*\n"), file);
    }

    else if (_matrix->matrix_type == MATRIX_TYPE_RESULT) {
        fwrite("$\n", sizeof(char), strlen("$\n"), file);

		fwrite("-\n", sizeof(char), strlen("-\n"), file);
    }
    else {
        return -1; // Invalid matrix type
	}

   
    // Write matrix metadata
    //fwrite(&_matrix->matrix_id, sizeof(_matrix->matrix_id), 1, file);
    //fwrite(&_matrix->operand_id, sizeof(_matrix->operand_id), 1, file);
    //fwrite(&_matrix->rows, sizeof(_matrix->rows), 1, file);
    //fwrite(&_matrix->cols, sizeof(_matrix->cols), 1, file);
    //fwrite(&_matrix->matrix_type, sizeof(_matrix->matrix_type), 1, file);
    // Write matrix data
    //if (_matrix->matrix_type == MATRIX_TYPE_OPERAND) {
    //    fwrite(_matrix->ushort_data, sizeof(unsigned short int), _matrix->rows * _matrix->cols, file);
    //}
    //else if (_matrix->matrix_type == MATRIX_TYPE_RESULT) {
    //    fwrite(_matrix->uint_data, sizeof(unsigned int), _matrix->rows * _matrix->cols, file);
    //}
    return 0; // Success
}

// Safely deallocates matrix memory
void free_matrix(Matrix* m) {
    if (m) {
        if (m->matrix_type == MATRIX_TYPE_OPERAND) {
            free(m->ushort_data); // Free ushort data array
        }
        else if (m->matrix_type == MATRIX_TYPE_RESULT) {
            free(m->uint_data);   // Free uint data array
        }
        else {
            // Invalid matrix type, nothing to free
		}
        
        free(m);        // Then free struct
    }
}

unsigned short int generate_random_ushort(void) {
    unsigned int r1 = (unsigned int)rand();
    unsigned int r2 = (unsigned int)rand();
    return (unsigned short int)(((r1 >> 7) << 8) | (r2 >> 7));
}

unsigned short init_matrix_operand1(Matrix* mat) {

    for (int i = 0; i < mat->rows; ++i) {
        for (int j = 0; j < mat->cols; ++j) {
            matrix_set_ushort(mat, i, j, generate_random_ushort()); // Initialize remaining elements to random values
        }
    }

    //std::cout << "Matrix Operand 1 Initialized, Size:" << mat->rows << " x " << mat->cols << " matrix " << std::endl; // Debug output

    return (unsigned short)0;
}

unsigned short init_matrix_operand2(Matrix* mat) {

    for (int i = 0; i < mat->rows; ++i) {
        for (int j = 0; j < mat->cols; ++j) {
            matrix_set_ushort(mat, i, j, generate_random_ushort()); // Initialize remaining elements to random values
        }
    }

    //std::cout << "Matrix Operand 2 Initialized, Size:" << mat->rows << " x " << mat->cols << " matrix " << std::endl; // Debug output

    return (unsigned short)0;
}

unsigned int multiply_matrices(Matrix* _matrix1, Matrix* _matrix2, Matrix* _matrix_result) {

    unsigned int result = 0;
    // check if matrices can be multiplied
    if (_matrix1->cols != _matrix2->rows) {
        result = MATX_OP_INCOMPATIBLE;
        return result;
    }
    // Initialize result matrix
    //_matrix_result = create_matrix(_matrix1->rows, _matrix2->cols);
    /*if (!_matrix_result) {
        result = MATX_OP_FAILURE;
        return result;
    }*/

    else {
        for (int row_counter = 0; row_counter < _matrix_result->rows; ++row_counter) {                // _matrix_result->rows = _matrix1->rows
            for (int col_counter = 0; col_counter < _matrix_result->cols; ++col_counter) {            // _matrix_result->cols = _matrix2->cols
                unsigned int sum = 0;
                for (int common_dim = 0; common_dim < _matrix2->rows; ++common_dim) {                   // _matrix1->cols = _matrix2->rows
                    unsigned short int a = _matrix1->ushort_data[row_counter * _matrix1->cols + common_dim];
                    unsigned short int b = _matrix2->ushort_data[common_dim * _matrix2->cols + col_counter];
                    //std::cout << "Multiplying: " << a << " * " << b << std::endl; // Debug output
                    sum += a * b;
                }
                //std::cout << "Sum: " << sum << std::endl; // Debug output
                _matrix_result->uint_data[row_counter * _matrix_result->cols + col_counter] = sum; // Store result
                //std::cout << "Result of Row: " << row_counter << " Col: " << col_counter << " = " << _matrix_result->uint_data[row_counter * _matrix_result->cols + col_counter] << std::endl; // Debug output
            }
        }
        result = MATX_OP_SUCCESS;
    }

    return result;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
