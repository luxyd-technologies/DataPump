
#include <iostream>
#include <iomanip> // Required for std::hex and std::setw

#include <cstdlib>   
#include <stdlib.h>
#include <stdio.h>
#include <time.h>    // For time, srand
#include "common.h"
#include "MemTrack.h"

void debug_print(const char* _message_format, const char* _message) {
    if (DEBUG_1) {
        printf(_message_format, _message);
    }
}

//void printMemoryContents(const void* address, size_t size) {
//    const unsigned char* ptr = static_cast<const unsigned char*>(address);
//
//    std::cout << "Memory contents at " << address << " (size: " << size << " bytes):" << std::endl;
//
//    for (size_t i = 0; i < size; ++i) {
//        // Print address offset
//        if (i % 16 == 0) {
//            std::cout << std::endl << std::hex << std::setw(8) << std::setfill('0') << i << ": ";
//        }
//
//        // Print byte in hexadecimal
//        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ptr[i]) << " ";
//    }
//    std::cout << std::endl;
//}


int print_message(int message_type, char* message) {

	if (message_type == MSG_SYSTEM_OUTPUT) {
		printf(message);  // Simple string
		return 0;
	}
	else if (message_type == MSG_ERROR_OUTPUT) {
		fprintf(stderr, "Error: %s\n", message);
		return -1;
	}
	else {

		return -1;
	}

}

//int ushort_to_string(unsigned short int value, char* buffer, size_t size) {
//    char temp[6]; // Max 5 digits + null terminator
//    int i = 0;
//
//    // Handle zero explicitly
//    if (value == 0) {
//        if (size < 2) return -1; // Not enough space
//        buffer[0] = '0';
//        buffer[1] = '\0';
//        return 0;
//    }
//
//    // Extract digits in reverse order
//    while (value > 0) {
//        temp[i++] = '0' + (value % 10); // Convert last digit to char
//        value /= 10; // Remove last digit
//    }
//    temp[i] = '\0'; // Null-terminate temporary string
//
//    // Check if buffer is large enough
//    if (size < (size_t)(i + 1)) return -1;
//
//    // Reverse digits to correct order
//    int len = i;
//    for (int j = 0; j < len; j++) {
//        buffer[j] = temp[i - 1 - j];
//    }
//    buffer[len] = '\0'; // Null-terminate result
//
//    return 0; // Success
//}

unsigned int generate_truly_random_uint(void) {
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL)); // Seed using current time
        seeded = 1;
    }

    unsigned int num = 0;
    for (size_t i = 0; i < sizeof(unsigned int); i++) {
        num = (num << 8) | (rand() & 0xFF); // Combine random bytes
    }
    return num;
}

unsigned short generate_random_ushort(void) {
    unsigned int r1 = (unsigned int)generate_truly_random_uint();     //todo: use better random number generator
    unsigned int r2 = (unsigned int)generate_truly_random_uint();
    return (unsigned short int)(((r1 >> 7) << 8) | (r2 >> 7));
}

char* ushort_to_string(unsigned short int num) {
    if (num == 0) {
        char* str = (char*) c_alloc(1, 2, "ushort_to_string", 0);
        if (str == NULL) return NULL;
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    // Calculate length
    unsigned int temp = num;
    size_t len = 0;
    while (temp != 0) {
        len++;
        temp /= 10;
    }

    // Allocate memory
    char* str = (char*) c_alloc(1, len + 1, "ushort_to_string", 0);
    if (str == NULL) return NULL;

    // Convert digits
    size_t i = len;
    temp = num;
    while (temp != 0) {
        unsigned int digit = temp % 10;
        str[--i] = '0' + digit; // Convert digit to char
        temp /= 10;
    }
    str[len] = '\0'; // Null-terminate

    return str;
}

char* uint_to_string(unsigned int num) {
    if (num == 0) {
        char* str = (char*) c_alloc(SINGLE_INSTANCE, 2, "uint_to_string", 0);
        if (str == NULL) return NULL;
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    // Calculate length
    unsigned int temp = num;
    size_t len = 0;
    while (temp != 0) {
        len++;
        temp /= 10;
    }

    // Allocate memory
    char* str = (char*) c_alloc(SINGLE_INSTANCE, len + 1, "uint_to_string", 0);
    if (str == NULL) return NULL;

    // Convert digits
    size_t i = len;
    temp = num;
    while (temp != 0) {
        unsigned int digit = temp % 10;
        str[--i] = '0' + digit; // Convert digit to char
        temp /= 10;
    }
    str[len] = '\0'; // Null-terminate

    return str;
}

//unsigned short int generate_random_ushort(void) {
//    unsigned int r1 = (unsigned int)rand();
//    unsigned int r2 = (unsigned int)rand();
//    return (unsigned short int)(((r1 >> 7) << 8) | (r2 >> 7));
//}

//Operation* create_operation(unsigned int _operation_id, Matrix* _matrix_operand1, Matrix* _matrix_operand2, Matrix* _matrix_result) {
//    Operation* op = static_cast<Operation*>(calloc(sizeof(Operation)));
//    if (!op) return NULL; // Memory allocation failed
//    op->operation_id = _operation_id;
//    op->operand1 = _matrix_operand1;
//    op->operand2 = _matrix_operand2;
//    op->result = _matrix_result;
//    return op;
//}

//void operation_print_info(const Operation* op) {
//    if (op) {
//        printf(" ---- \nOperation ID: %u", op->operation_id);
//        printf("\nOperand 1:Matrix");
//        matrix_print_info(op->operand1);
//        printf("\nOperand 2 Matrix:");
//        matrix_print_info(op->operand2);
//        printf("\nResult Matrix:");
//        matrix_print_info(op->result);
//		printf(" ---- \n");
//    } else {
//        printf("Operation is NULL\n");
//    }
//}
//
//void free_operation(Operation* op) {
//    if (op) {
//        free(op->operand1); // Free operand1 matrix
//        free(op->operand2); // Free operand2 matrix
//        free(op->result);   // Free result matrix
//        free(op);          // Finally, free the operation struct
//    }
//}

//// Safe element access with bounds checking
//unsigned int matrix_get_ushort(const Matrix* m, int row, int col) {
//    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
//        throw std::out_of_range("Matrix index out of bounds");
//    }
//    return m->ushort_data[row * m->cols + col];
//}
//
//// Safe element access with bounds checking
//unsigned int matrix_get_uint(const Matrix* m, int row, int col) {
//    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
//        throw std::out_of_range("Matrix index out of bounds");
//    }
//    return m->uint_data[row * m->cols + col];
//}

//// Safe element modification with bounds checking
//void matrix_set_ushort(Matrix* m, int row, int col, unsigned short int value) {
//    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
//        throw std::out_of_range("Matrix index out of bounds");
//    }
//    m->ushort_data[row * m->cols + col] = value;
//}
//
//// Safe element modification with bounds checking
//void matrix_set_uint(Matrix* m, int row, int col, unsigned short int value) {
//    if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
//        throw std::out_of_range("Matrix index out of bounds");
//    }
//    m->uint_data[row * m->cols + col] = value;
//}

//// Allocates matrix memory (initialized to zero)
//Matrix* create_matrix(int _matrix_id, int _operand_id, int _rows, int _cols, int _matrix_type) {
//    Matrix* m = static_cast<Matrix*>(calloc(sizeof(Matrix)));
//    if (!m) return nullptr;
//
//    m->matrix_id = _matrix_id;
//    m->operand_id = _operand_id;
//    m->rows = _rows;
//    m->cols = _cols;
//
//    if (_matrix_type == MATRIX_TYPE_OPERAND) {
//        m->matrix_type = MATRIX_TYPE_OPERAND;
//        m->ushort_data = static_cast<unsigned short int*>(calloc(_rows * _cols, sizeof(unsigned short int)));
//    }
//    else if (_matrix_type == MATRIX_TYPE_RESULT) {
//        m->matrix_type = MATRIX_TYPE_RESULT;
//        m->uint_data = static_cast<unsigned  int*>(calloc(_rows * _cols, sizeof(unsigned  int)));
//    }
//    else {
//        free(m);
//        return nullptr; // Invalid matrix type
//    }
//
//    if ((!m->ushort_data) && (!m->uint_data)) {
//        free(m);
//        return nullptr;
//    }
//    return m;
//}

// print matrix information
// This function prints the matrix information to stdout
//void matrix_print_info(const Matrix* m) {
//    if (m) {
//        printf("\nMatrix ID: %d\t Operand Id:%d\t Dimension: %d x %d\tMatrix Type: %d\n", m->matrix_id, m->operand_id, m->rows, m->cols, m->matrix_type);
//        for (int i = 0; i < m->rows; ++i) {
//            for (int j = 0; j < m->cols; ++j) {
//                if (m->matrix_type == MATRIX_TYPE_OPERAND) {
//                    printf("%d ", m->ushort_data[i * m->cols + j]);
//                } else if (m->matrix_type == MATRIX_TYPE_RESULT) {
//                    printf("%u ", m->uint_data[i * m->cols + j]);
//                }
//            }
//            printf("\n");
//		}
//       
//    } else {
//        printf("Matrix is NULL\n");
//    }
//}
//
//// Safely deallocates matrix memory
//void free_matrix(Matrix* m) {
//    if (m) {
//        free(m->ushort_data);  // Free data array first
//        free(m);        // Then free struct
//    }
//}

//void print_matrix(const Matrix* mat, int print_type) {
//    for (int i = 0; i < mat->rows; ++i) {
//        for (int j = 0; j < mat->cols; ++j) {
//            if (print_type == MATRIX_TYPE_OPERAND) {
//                std::cout << matrix_get_ushort(mat, i, j) << " "; // Print as double
//            }
//            else if (print_type == MATRIX_TYPE_RESULT) {
//                std::cout << matrix_get_uint(mat, i, j) << " "; // Print as long
//            }
//            else {
//                std::cerr << "Unknown print type!" << std::endl;
//                return;
//            }
//        }
//        std::cout << "\n";
//    }
//}

//unsigned int multiply_matrices(Matrix* _matrix1, Matrix* _matrix2, Matrix* _matrix_result) {
//
//    unsigned int result = 0;
//    // check if matrices can be multiplied
//    if (_matrix1->cols != _matrix2->rows) {
//        result = MATX_OP_INCOMPATIBLE;
//        return result;
//    }
//    // Initialize result matrix
//    //_matrix_result = create_matrix(_matrix1->rows, _matrix2->cols);
//    if (!_matrix_result) {
//        result = MATX_OP_FAILURE;
//        return result;
//    }
//
//    else {
//        for (int row_counter = 0; row_counter < _matrix_result->rows; ++row_counter) {                // _matrix_result->rows = _matrix1->rows
//            for (int col_counter = 0; col_counter < _matrix_result->cols; ++col_counter) {            // _matrix_result->cols = _matrix2->cols
//                unsigned int sum = 0;
//                for (int common_dim = 0; common_dim < _matrix2->rows; ++common_dim) {                   // _matrix1->cols = _matrix2->rows
//                    unsigned short int a = _matrix1->ushort_data[row_counter * _matrix1->cols + common_dim];
//                    unsigned short int b = _matrix2->ushort_data[common_dim * _matrix2->cols + col_counter];
//                    std::cout << "Multiplying: " << a << " * " << b << std::endl; // Debug output
//                    sum += a * b;
//                }
//                std::cout << "Sum: " << sum << std::endl; // Debug output
//                _matrix_result->uint_data[row_counter * _matrix_result->cols + col_counter] = sum; // Store result
//                std::cout << "Result of Row: " << row_counter << " Col: " << col_counter << " = " << _matrix_result->uint_data[row_counter * _matrix_result->cols + col_counter] << std::endl; // Debug output
//            }
//        }
//        result = MATX_OP_SUCCESS;
//    }
//
//    return result;
//}

//unsigned int matrixMultiply1(int** mat1, int rows1, int cols1,
//    int** mat2, int rows2, int cols2,
//    int** result) {
//    // Check if matrices can be multiplied
//    if (cols1 != rows2) {
//        // Handle incompatible dimensions: set result to zeros or leave unchanged
//        return 0;
//    }
//
//    // Initialize result matrix to zeros
//    for (int i = 0; i < rows1; ++i) {
//        for (int j = 0; j < cols2; ++j) {
//            result[i][j] = 0;
//        }
//    }
//
//    // Perform matrix multiplication
//    for (int i = 0; i < rows1; ++i) {
//        for (int j = 0; j < cols2; ++j) {
//            for (int k = 0; k < cols1; ++k) {
//                result[i][j] += mat1[i][k] * mat2[k][j];
//            }
//        }
//    }
//}