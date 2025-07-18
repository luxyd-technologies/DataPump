#include <cstdlib>   
#include <stdlib.h>
#include <stdio.h>
#include <time.h>    // For time, srand
#include <iostream> // For std::cerr
#include <assert.h>

#include "Parameters.h"

#include "common.h"
#include "Matrix.h" // Assuming Matrix.h contains the necessary definitions
#include "MemTrack.h" // Memory tracking header
#include "MatrixAllocation.h"

int matrix_print_info(Matrix* _matrix) {

	if (_matrix) {

		printf("\n -- Matrix Print Info -- \nMatrix ID: %d\t Operation Id:%d\t Dimension: %d x %d\tMatrix Type: %d\n", 
			_matrix->matrix_id, _matrix->operation_id, _matrix->rows, _matrix->cols, _matrix->matrix_type);

		for (int i = 0; i < _matrix->rows; ++i) {
			for (int j = 0; j < _matrix->cols; ++j) {
				if (_matrix->matrix_type == MATRIX_TYPE_OPERAND) {
				//if (_matrix->data->data_type == UNSIGNED_SHORT) {
					printf("%hu", matrix_get_ushort(_matrix, i, j)); // Print the value for debugging
				}
				else if (_matrix->matrix_type == MATRIX_TYPE_RESULT) {
					printf("%u", matrix_get_uint(_matrix, i, j));

				}
				if (j < _matrix->cols - 1) {
					printf(" ");
				}
			}
			printf("\n");
		}
		return EXIT_SUCCESS;
	}
	else {
		printf("_Matrix is NULL\n");
		return EXIT_FAILURE;
	}
}

int operation_print_info(Operation* op) {

	if (op) {
		printf("\n\n---- Operation Print Info ---\nOperation ID: %u", op->operation_id);
		printf("\nOperand 1: Matrix");
		matrix_print_info(op->operand1);
		printf("\nOperand 2 Matrix:");
		matrix_print_info(op->operand2);
		printf("\nResult Matrix:");
		matrix_print_info(op->result);
		printf(" ---- \n");
		return EXIT_SUCCESS;
	}
	else {
		printf("Operation is NULL\n");
		return EXIT_FAILURE;
	}
}

// Safe element access with bounds checking
unsigned short matrix_get_ushort(const Matrix* m, int row, int col) {
	if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
		//throw std::out_of_range("_Matrix index out of bounds");
		fprintf(stderr, "_Matrix index out of bounds: row=%d, col=%d, rows=%d, cols=%d\n", row, col, m->rows, m->cols);
		return NULL; // Return an error value or handle it as needed
	}
	return m->data->ushort_data[row * m->cols + col];
}

// Safe element access with bounds checking
unsigned int matrix_get_uint(const Matrix* m, int row, int col) {
	if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
		//throw std::out_of_range("_Matrix index out of bounds");
		fprintf(stderr, "_Matrix index out of bounds: row=%d, col=%d, rows=%d, cols=%d\n", row, col, m->rows, m->cols);
		return NULL; // Return an error value or handle it as needed
	}
	return m->data->uint_data[row * m->cols + col];
}

//Safe element modification with bounds checking
void matrix_set_ushort(Matrix* m, int row, int col, unsigned short int value) {
	if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
		//throw std::out_of_range("_Matrix index out of bounds");
		fprintf(stderr, "_Matrix index out of bounds: row=%d, col=%d, rows=%d, cols=%d\n", row, col, m->rows, m->cols);
	}
	m->data->ushort_data[row * m->cols + col] = value;
}

// Safe element modification with bounds checking
void matrix_set_uint(Matrix* m, int row, int col, unsigned int value) {
	if (row < 0 || row >= m->rows || col < 0 || col >= m->cols) {
		//throw std::out_of_range("_Matrix index out of bounds");
		fprintf(stderr, "_Matrix index out of bounds: row=%d, col=%d, rows=%d, cols=%d\n", row, col, m->rows, m->cols);
	}
	m->data->uint_data[row * m->cols + col] = value;
}


Matrix* init_matrix_zeroes(Matrix* _matrix) {

	// set the matrix header data
	if (_matrix != NULL) {
		// set the matrix value data
		for (int row_counter = 0; row_counter < _matrix->rows; ++row_counter) {
			for (int column_counter = 0; column_counter < _matrix->cols; ++column_counter) {
				if (_matrix->matrix_type == MATRIX_TYPE_OPERAND) {
					matrix_set_ushort(_matrix, row_counter, column_counter, 0);
					if (DEBUG_MATRIX_INIT) {
						printf("%hu,", matrix_get_ushort(_matrix, row_counter, column_counter)); // Print the value for debugging
					}
				}
				else if (_matrix->matrix_type == MATRIX_TYPE_RESULT) {
					matrix_set_uint(_matrix, row_counter, column_counter, 0);
					if (DEBUG_MATRIX_INIT) {
						printf("%hu,", matrix_get_uint(_matrix, row_counter, column_counter)); // Print the value for debugging
					}
				}
			}
			if (DEBUG_MATRIX_INIT) {
				printf(" \n");
			}
		}
		if (DEBUG_MATRIX_INIT) {
			printf(" \n");
		}
	}
	return _matrix;
}

Matrix* init_matrix_random(Matrix* _matrix) {

	// set the matrix header data
	if (_matrix != nullptr) {
		// set the matrix value data
		//for (int row_counter = 0; row_counter < _matrix->rows; ++row_counter) {
		//	for (int column_counter = 0; column_counter < _matrix->cols; ++column_counter) {
		//		if (_matrix->matrix_type == MATRIX_TYPE_OPERAND) {
		//			matrix_set_ushort(_matrix, row_counter, column_counter, generate_random_ushort());
		//			if (DEBUG_MATRIX_INIT) {
		//				printf("%hu,", matrix_get_ushort(_matrix, row_counter, column_counter)); // Print the value for debugging
		//			}
		//		}
		//		else if (_matrix->matrix_type == MATRIX_TYPE_RESULT) {
		//			matrix_set_uint(_matrix, row_counter, column_counter, generate_truly_random_uint());
		//			if (DEBUG_MATRIX_INIT) {
		//				printf("%hu,", matrix_get_uint(_matrix, row_counter, column_counter)); // Print the value for debugging
		//			}
		//		}
		//	}
			for (int row_counter = 0; row_counter < _matrix->rows; row_counter++) {
				for (int column_counter = 0; column_counter < _matrix->cols; column_counter++) {
					if (_matrix->matrix_type == MATRIX_TYPE_OPERAND) {
						matrix_set_ushort(_matrix, row_counter, column_counter, generate_random_ushort());
						if (DEBUG_MATRIX_INIT) {
							printf("%hu,", matrix_get_ushort(_matrix, row_counter, column_counter)); // Print the value for debugging
						}
					}
					else if (_matrix->matrix_type == MATRIX_TYPE_RESULT) {
						matrix_set_uint(_matrix, row_counter, column_counter, generate_truly_random_uint());
						if (DEBUG_MATRIX_INIT) {
							printf("%u,", matrix_get_uint(_matrix, row_counter, column_counter)); // Print the value for debugging
						}
					}
				}
			if (DEBUG_MATRIX_INIT) {
				printf(" \n");
			}
		}
		if (DEBUG_MATRIX_INIT) {
			printf(" \n");
		}
	}
	return _matrix;
}


Matrix* calc_matrix_multiplication(Matrix* _operand1, Matrix* _operand2, Matrix* _result_matrix) {

	if (_operand1 == NULL || _operand2 == NULL) {
		std::cerr << "One of the operands is NULL." << std::endl;
		return NULL;
	}
	if (_operand1->cols != _operand2->rows) {
		std::cerr << "Matrix multiplication not possible due to incompatible dimensions." << std::endl;
		return NULL;
	}

	unsigned short int result_rows = _operand1->rows;
	unsigned short int result_cols = _operand2->cols;
	//MatrixData* result_data = allocate_matrix_data(UNSIGNED_INTEGER, result_rows, result_cols);
	//if (result_data == NULL) {
	//	return NULL;
	//}

	int result_rows_count = _operand1->rows; // Result matrix rows
	int result_cols_count = _operand2->cols; // Result matrix columns
	int common_dim_count = _operand1->cols; //  or operand2->rows, Common dimension for multiplication 

	/*Matrix* result_matrix = nullptr;
	result_matrix = allocate_matrix_with_matrix_data(_operand1->operation_id, _matrix_id, result_rows, result_cols, MATRIX_TYPE_RESULT);
	if (result_matrix == NULL) {
		deallocate_matrix_data(result_data, result_matrix);
		return NULL;
	}*/

	for (int row_counter = 0; row_counter < result_rows; ++row_counter) {                // _matrix_result->rows = _matrix1->rows
		for (int col_counter = 0; col_counter < result_cols; ++col_counter) {            // _matrix_result->cols = _matrix2->cols
			unsigned int sum = 0;
			for (int common_dim = 0; common_dim < common_dim_count; ++common_dim) {                   // _matrix1->cols = _matrix2->rows
				unsigned short int a = matrix_get_ushort(_operand1, row_counter, common_dim);
				unsigned short int b = matrix_get_ushort(_operand2, common_dim, col_counter);
				sum += a * b;
			}
			matrix_set_uint(_result_matrix, row_counter, col_counter, sum); // Store result in the result matrix data
		}
	}
	
	return _result_matrix;
}

MatrixData* create_matrix_data(int _data_type, int _rows, int _cols) {
	// allocate memory for matrix data header
	MatrixData* matrix_data = allocate_matrix_data(_data_type, _rows, _cols);
	if (!matrix_data) {
		fprintf(stderr, "Memory allocation for matrix data failed\n");
		return nullptr;
	}
	// initialize with zeros - not required since using calloc inside allocate_matrix_data

	return matrix_data;
}

Matrix* create_matrix(short _mode, unsigned short _operation_id, unsigned short _matrix_id, 
	unsigned short int _rows, unsigned short int _cols, int _matrix_type) {

	if (DEBUG_MEMORY_ALLOC) {
		printf("\n\t<---- Operation %d Matrix Id: %d Creation ---------> \n", _operation_id, _matrix_id);
	}
	// allocate memory for matrix header together with matrix data
	Matrix* matrix = allocate_matrix_with_matrix_data(_operation_id, _matrix_id, _rows, _cols, _matrix_type); // Create a matrix with no data
	

	if (DEBUG_MEMORY_ALLOC) {
		printf("\n\t<---- Operation %d Matrix Id: %d End Creation ---------> \n", _operation_id, _matrix_id);
	}
	return matrix;
}

Operation* create_operation(int _operation_id, Matrix* _operand1, Matrix* _operand2, Matrix* _result_matrix) { 

	// allocate memory for operation header
	Operation* this_operation = allocate_operation(_operation_id, _operand1, _operand2, _result_matrix);


	if (!this_operation) {
		fprintf(stderr, "Memory allocation for operation failed\n");
		return nullptr;
	}
	this_operation->next = nullptr; // Initialize next pointer to null
	if (DEBUG_MATRIX) {
		printf("Allocated memory for operation id: %d, total allocated memory bytes: %d\n",
			_operation_id, this_operation->total_allocated_memory_bytes);
	}

	return this_operation;
}
Operation* create_operation_list() { return nullptr; }




// Allocates matrix memory (initialized to zero)
//Matrix* create_matrix(int _matrix_id, int _operation_id, int _rows, int _cols, int _matrix_type) {
//	if (DEBUG_MEMORY_ALLOC) {
//		printf("Allocating memory for matrix size: %zu\n", sizeof(Matrix));
//	}
//	Matrix* m = (Matrix*) (c_alloc(SINGLE_INSTANCE, sizeof(Matrix)));
//	if (!m) return nullptr;
//
//	m->matrix_id = _matrix_id;
//	m
// ->operation_id = _operation_id;
//	m->rows = _rows;
//	m->cols = _cols;
//
//	if (_matrix_type == MATRIX_TYPE_OPERAND) {
//		m->matrix_type = MATRIX_TYPE_OPERAND;
//		m->ushort_data = (unsigned short int*) (c_alloc(_rows * _cols, sizeof(unsigned short int)));
//	}
//	else if (_matrix_type == MATRIX_TYPE_RESULT) {
//		m->matrix_type = MATRIX_TYPE_RESULT;
//		m->uint_data = (unsigned int*)(c_alloc(_rows * _cols, sizeof(unsigned int)));
//	}
//	else {
//		printf("Invalid matrix type specified.\n");
//		f_ree(m->ushort_data); // Free allocated memory for ushort_data
//		f_ree(m->uint_data);   // Free allocated memory for uint_data
//		f_ree(m);              // Free the matrix struct
//		return nullptr;     // not possible path, but just in case
//	}
//
//
//	if ((!m->ushort_data) && (!m->uint_data)) {
//		f_ree(m);
//		return nullptr;
//	}
//	return m;
//}

