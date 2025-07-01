// MatMul1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>   // For calloc, calloc, free
#include <stdlib.h>
#include <stdexcept> // For std::out_of_range
#include "Matrix.h"
#include "common.h"
#include <string.h>

//using namespace std;

Operation* create_operation(unsigned int _operation_id, Matrix* _matrix_operand1, Matrix* _matrix_operand2, Matrix* _matrix_result) {
    Operation* op = static_cast<Operation*>(calloc(1, sizeof(Operation)));
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

    if (!_operation || !filename) return RESULT_ERROR; // Invalid operation or filename

    FILE* file = fopen(filename, "ab");
    if (!file) return RESULT_ERROR; // File opening failed

	printf("\n\tSaving operation %d to file %s...\n\n", _operation->operation_id, filename);

	fwrite("[\n", sizeof(char), strlen("[\n"), file);       // Write operation start marker 
	save_matrix_tofile(file, _operation->operand1, _operation->operation_id, 1);         // write operand1 matrix to file    
	save_matrix_tofile(file, _operation->operand2, _operation->operation_id, 2);         // write operand2 matrix to file
	save_matrix_tofile(file, _operation->result, _operation->operation_id, 3);           // write result matrix to file
    fwrite("]\n", sizeof(char), strlen("]\n"), file);       // Write operation end market 

	printf("\tOperation %d saved to file %s successfully\n", _operation->operation_id, filename);
    fclose(file);

    return RESULT_SUCCESS; // Success
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
        //throw std::out_of_range("Matrix index out of bounds");
		fprintf(stderr, "Matrix index out of bounds: row=%d, col=%d, rows=%d, cols=%d\n", row, col, m->rows, m->cols);
        return NULL; // Return an error value or handle it as needed
    }
    return m->ushort_data[row * m->cols + col];
}

// Safe element access with bounds checking
unsigned int matrix_get_uint(const Matrix* m, int row, int col) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
        //throw std::out_of_range("Matrix index out of bounds");
        fprintf(stderr, "Matrix index out of bounds: row=%d, col=%d, rows=%d, cols=%d\n", row, col, m->rows, m->cols);
        return NULL; // Return an error value or handle it as needed
    }
    return m->uint_data[row * m->cols + col];
}

 //Safe element modification with bounds checking
void matrix_set_ushort(Matrix* m, int row, int col, unsigned short int value) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
        //throw std::out_of_range("Matrix index out of bounds");
        fprintf(stderr, "Matrix index out of bounds: row=%d, col=%d, rows=%d, cols=%d\n", row, col, m->rows, m->cols);
    }
    m->ushort_data[row * m->cols + col] = value;
}

// Safe element modification with bounds checking
void matrix_set_uint(Matrix* m, int row, int col, unsigned int value) {
    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
        //throw std::out_of_range("Matrix index out of bounds");
        fprintf(stderr, "Matrix index out of bounds: row=%d, col=%d, rows=%d, cols=%d\n", row, col, m->rows, m->cols);
    }
    m->uint_data[row * m->cols + col] = value;
}

// Allocates matrix memory (initialized to zero)
Matrix* create_matrix(int _matrix_id, int _operand_id, int _rows, int _cols, int _matrix_type) {

    Matrix* m = (Matrix*)(calloc(SINGLE_INSTANCE, sizeof(Matrix)));
    if (!m) return nullptr;

    m->matrix_id = _matrix_id;
    m->operand_id = _operand_id;
    m->rows = _rows;
    m->cols = _cols;
    m->ushort_data = (unsigned short int*) (calloc(_rows * _cols, sizeof(unsigned short int)));
    m->uint_data = (unsigned int*)(calloc(_rows * _cols, sizeof(unsigned int)));

    if (_matrix_type == MATRIX_TYPE_OPERAND) {
        m->matrix_type = MATRIX_TYPE_OPERAND;
    }
    else if (_matrix_type == MATRIX_TYPE_RESULT) {
        m->matrix_type = MATRIX_TYPE_RESULT;
    }
    else {
        free(m->ushort_data); // Free allocated memory for ushort_data
        free(m->uint_data);   // Free allocated memory for uint_data
        free(m);              // Free the matrix struct
		printf("Invalid matrix type specified.\n");
		return nullptr;     // not possible path, but just in case
	}


    if ((!m->ushort_data) && (!m->uint_data)) {
        free(m);
        return nullptr;
    }
    return m;
}

void matrix_print_info(Matrix* m) {
    if (m) {
        printf("\nMatrix ID: %d\t Operand Id:%d\t Dimension: %d x %d\tMatrix Type: %d\n", m->matrix_id, m->operand_id, m->rows, m->cols, m->matrix_type);
        for (int i = 0; i < m->rows; ++i) {
            for (int j = 0; j < m->cols; ++j) {
                if (m->matrix_type == MATRIX_TYPE_OPERAND) {
                    printf("%d ", m->ushort_data[i * m->cols + j]);
                }
                else if (m->matrix_type == MATRIX_TYPE_RESULT) {
                    printf("%u ", m->uint_data[i * m->cols + j]);
                }
            }
            //printf("\n");
        }

    }
    else {
        printf("Matrix is NULL\n");
    }
}


int save_matrix_tofile(FILE* file, Matrix* _matrix, int _operation_id, int operand_no) {

    if (!_matrix || !file) return -1; // Invalid matrix or file pointer
	

    int row_count = _matrix->rows;
	int col_count = _matrix->cols;

	
    if (_matrix->matrix_type == MATRIX_TYPE_OPERAND) {

        printf("\t\tOperation id %d - Saving operand %d with matrix id %d size (%d x %d) to file...\n", _operation_id, operand_no, _matrix->matrix_id, _matrix->rows, _matrix->cols);
		
        fprintf(file, "+\n%d,%d,%d,%d,%d\n", _matrix->matrix_id, _operation_id, row_count, col_count, _matrix->matrix_type); // Write matrix dimensions
        
        for (int row_counter = 0; row_counter < row_count; row_counter++) {
            char row_string[MAX_ROW_STRING_LENGTH] = "";
            for (int col_counter = 0; col_counter < col_count; col_counter++) {
                char col_string[20] = ""; // Temporary string for column value
                unsigned int value = (unsigned int) matrix_get_ushort(_matrix, row_counter, col_counter);
				fprintf(file, "%hu", value); // Write ushort data as string
				sprintf(col_string, "%d", value); // Debug output
                strcat(row_string, col_string); // Append comma for readability
                if (col_counter < col_count -1 ) { // If not the last column, write a comma
					fprintf(file, ","); // Write space after comma for readability  
                    strcat(row_string, ","); // Append comma for readability
				}
            }
            fprintf(file, "\n"); // next line after the last column
            printf("\t\t%s\n", row_string); // Debug output
		}
        //fwrite("*\n", sizeof(char), strlen("*\n"), file);
        fprintf(file, "*\n");
		printf("\t\tOperation id %d - Operand %d with matrix id %d saved to file successfully\n\n", _operation_id, operand_no, _matrix->matrix_id);
    }

    else if (_matrix->matrix_type == MATRIX_TYPE_RESULT) {
        
        printf("\t\tOperation id %d - Saving result matrix with matrix id %d size (%d x %d) to file...\n", _operation_id, _matrix->matrix_id, _matrix->rows, _matrix->cols);

        fprintf(file, "$\n%d,%d,%d,%d,%d\n", _matrix->matrix_id, _operation_id, row_count, col_count, _matrix->matrix_type); // Write matrix dimensions
        for (int row_counter = 0; row_counter < row_count; row_counter++) {
            char row_string[MAX_ROW_STRING_LENGTH] = "";
            for (int col_counter = 0; col_counter < col_count; col_counter++) {
				char col_string[20] = ""; // Temporary string for column value
                unsigned int value = (unsigned int) matrix_get_uint(_matrix, row_counter, col_counter);
                fprintf(file, "%u", value); // Write uint data as string
                sprintf(col_string, "%u", value); // Debug output
				strcat(row_string, col_string); // Append comma for readability
                if (col_counter < col_count - 1) { // If not the last column, write a space
                    fprintf(file, ","); // Write ushort data as string
                    strcat(row_string, ","); // Append comma for readability
                }
            }
            fprintf(file, "\n"); // next line after the last column
            printf("\t\t%s\n", row_string); // Debug output
        }
        fprintf(file, "-\n");
        printf("\t\tOperation id %d - Result matrix with matrix id %d saved to file successfully\n\n", _operation_id, _matrix->matrix_id);
    }
    else {
        return RESULT_ERROR; // Invalid matrix type
	}

    return RESULT_SUCCESS; // Success
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
	unsigned int r1 = (unsigned int)generate_truly_random_uint();     //todo: use better random number generator
    unsigned int r2 = (unsigned int)generate_truly_random_uint();
    return (unsigned short int)(((r1 >> 7) << 8) | (r2 >> 7));
}

unsigned short init_matrix_operand1(Matrix* mat) {

	printf("Initializing Matrix Operand 1 for Operation %d\n", mat->operand_id);
    for (int i = 0; i < mat->rows; ++i) {
		printf("Row %d\t ", i); // Debug output
        for (int j = 0; j < mat->cols; ++j) {
			unsigned short int value = generate_random_ushort(); // Generate a random ushort value
            matrix_set_ushort(mat, i, j, value); // Initialize remaining elements to random values
			printf("%hu, ", value); // Print each element for debugging
        }
		printf("\n"); // New line after each row
    }

    //std::cout << "Matrix Operand 1 Initialized, Size:" << mat->rows << " x " << mat->cols << " matrix " << std::endl; // Debug output

    return (unsigned short) 0;
}

unsigned short init_matrix_operand2(Matrix* mat) {

    printf("Initializing Matrix Operand 2 for Operation %d\n", mat->operand_id);
    for (int i = 0; i < mat->rows; ++i) {
        printf("Row %d\t ", i); // Debug output
        for (int j = 0; j < mat->cols; ++j) {
            unsigned short int value = generate_random_ushort(); // Generate a random ushort value
            matrix_set_ushort(mat, i, j, value); // Initialize remaining elements to random values
            printf("%hu, ", value); // Print each element for debugging
        }
        printf("\n"); // New line after each row
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
