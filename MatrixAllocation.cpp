#include <iostream>
#include <assert.h>

#include "Parameters.h"
#include "common.h"
#include "MemTrack.h"
#include "MatrixAllocation.h"
#include "Matrix.h"



MatrixData* allocate_matrix_data(unsigned short _data_type, unsigned short int rows, unsigned short int cols) {

	MatrixData* matrix_data = nullptr;

	// allocate memory for the MatrixData header
	matrix_data = (MatrixData*)c_alloc(SINGLE_INSTANCE, sizeof(MatrixData), "MatrixData Header", 0);
	if (matrix_data) {
		//matrix_data->rows = rows;
		//matrix_data->cols = cols;
		matrix_data->total_allocated_memory_bytes += SINGLE_INSTANCE * sizeof(MatrixData);
		if (DEBUG_MEMORY_ALLOC) {
			printf("Allocated memory size for MatrixData: %d bytes vs sizeOf(MatrixData): %llu\n", matrix_data->total_allocated_memory_bytes, sizeof(MatrixData));
		}
	}
	else {
		return nullptr;
	}

	// Allocate memory for the matrix data values
	switch (_data_type) {
		case UNSIGNED_SHORT: {
			matrix_data->data_type = UNSIGNED_SHORT;
			matrix_data->uint_data = nullptr; // Ensure uint_data is set to nullptr
			matrix_data->ushort_data = (unsigned short int*) c_alloc(rows * cols, sizeof(unsigned short int), "MatrixData Operand", 1);
			if (matrix_data->ushort_data == NULL) {
				return nullptr;
			}
			matrix_data->total_allocated_memory_bytes += (short) rows * (short) cols * (short) sizeof(unsigned short int);
			break;
		}
		case UNSIGNED_INTEGER: {
			matrix_data->data_type = UNSIGNED_INTEGER;
			matrix_data->ushort_data = nullptr; // Ensure ushort_data is set to nullptr
			matrix_data->uint_data = (unsigned int*) c_alloc(rows * cols, sizeof(unsigned int), "MatrixData Result", 1);
			if (matrix_data->uint_data == NULL) {
				return nullptr;
			}
			matrix_data->total_allocated_memory_bytes += (short) rows * (short) cols * (short) sizeof(unsigned int);
			break;
		}
		default: {
			std::cerr << "Failed allocation of matrix data - Unknown data type" << std::endl;
			return nullptr;
		}
	}
	if (DEBUG_MEMORY_ALLOC) {
		printf("Total allocated memory size for MatrixData: %d bytes vs sizeOf(MatrixData): %llu\n", matrix_data->total_allocated_memory_bytes, sizeof(MatrixData));
	}
	return matrix_data;
}

// allocate matrix with matrix data
Matrix* allocate_matrix_with_matrix_data(unsigned short _operation_id, unsigned short _matrix_id, 
	unsigned short int _rows, unsigned short int _cols, int _matrix_type) {
	
	// allocate memory for matrix header
	Matrix* matrix = (Matrix*) c_alloc(SINGLE_INSTANCE, sizeof(Matrix), "Matrix Header", 0);
	if (matrix) {

		if (_matrix_type == MATRIX_TYPE_RESULT) {
			printf("\n -- allocating for result matrix\n");
		}

		matrix->operation_id = _operation_id;
		matrix->matrix_id = _matrix_id;
		matrix->rows = _rows;
		matrix->cols = _cols;
		matrix->matrix_type = _matrix_type;
		matrix->total_allocated_memory_bytes = SINGLE_INSTANCE * sizeof(Matrix);

		if (DEBUG_MEMORY_ALLOC) {
			printf("Allocated memory size for Matrix Header: %d bytes vs sizeOf(Matrix): %llu\n", matrix->total_allocated_memory_bytes, sizeof(Matrix));
		}

		// set matrix data type based on matrix type
		int matrix_data_type = 0; // Default to 0, will be set based on matrix type
		if (_matrix_type == MATRIX_TYPE_OPERAND) {
			matrix_data_type = UNSIGNED_SHORT;
		}
		else if (_matrix_type == MATRIX_TYPE_RESULT) {
			matrix_data_type = UNSIGNED_INTEGER;
		}
		else {
			fprintf(stderr, "Invalid matrix type for matrix id: %d\n", _matrix_id);
			f_ree(matrix); // Free the matrix header if type is invalid
			return nullptr;
		}

		// allocate matrix data
		MatrixData* matrix_data = allocate_matrix_data(matrix_data_type, _rows, _cols); // user matrix_data_type to allocate the correct type of data
		if (matrix_data != nullptr) {
			matrix->data = matrix_data; // Assign the allocated matrix data to the matrix
			matrix->total_allocated_memory_bytes += matrix_data->total_allocated_memory_bytes;
			if (DEBUG_MEMORY_ALLOC) {
				printf("Total allocated memory size for Matrix id: %d with data: %d bytes vs sizeOf(Matrix): %llu\n", 
					matrix->matrix_id, matrix->total_allocated_memory_bytes, sizeof(Matrix));
			}
		}
		else {
			fprintf(stderr, "Matrix data allocation failed for matrix data for Matrix id: %d\n", _matrix_id);
			f_ree(matrix); // Free the matrix header if data allocation fails
			return nullptr;
		}
	}
	else {
		fprintf(stderr, "Memory allocation for matrix id: %d failed\n", _matrix_id);
		return nullptr;
	}
	if (_matrix_type == MATRIX_TYPE_RESULT) {
		printf(" -- end allocating for result matrix\n\n");
	}

	return matrix;
}

// allocate matrix without allocatng matrix data
Matrix* deprecated_allocate_matrix(unsigned short _operation_id, unsigned short _matrix_id, unsigned short int rows, unsigned short int cols, int _matrix_type, MatrixData* _matrix_data) {

	Matrix* matrix = nullptr;
	matrix = (Matrix*)c_alloc(SINGLE_INSTANCE, sizeof(Matrix), "Matrix Header", 0);


	if (matrix) {
		matrix->operation_id = _operation_id;
		matrix->matrix_id = _matrix_id;
		matrix->rows = rows;
		matrix->cols = cols;
		matrix->matrix_type = _matrix_type;
		matrix->total_allocated_memory_bytes = SINGLE_INSTANCE * sizeof(Matrix);
		if (DEBUG_MEMORY_ALLOC) {
			printf("Allocated memory size for Matrix: %d bytes vs sizeOf(Matrix): %llu\n", matrix->total_allocated_memory_bytes, sizeof(Matrix));
		}
		if (_matrix_data == nullptr) {
			//printf("Matrix Data is nullptr, Operation id: %d Matrix id: %d and g_status: %d\n", _operation_id, _matrix_id, g_status);
			matrix->data = allocate_matrix_data(_matrix_type, rows, cols);
		}
		matrix->data = _matrix_data;
		matrix->total_allocated_memory_bytes += _matrix_data->total_allocated_memory_bytes;
	}
	else {
		fprintf(stderr, "Memory allocation for matrix id: %d failed\n", _matrix_id);
		return NULL;
	}

	
	return matrix;

}

Operation* allocate_operation(unsigned short _operation_id, Matrix* _operand1, Matrix* _operand2, Matrix* _result) {

	Operation* operation = nullptr;
	operation = (Operation*) c_alloc(SINGLE_INSTANCE, sizeof(Operation), "Operation Header", 0);

	if (operation) {
		operation->operation_id = _operation_id;
		operation->total_allocated_memory_bytes = SINGLE_INSTANCE * sizeof(Operation);
		if (DEBUG_MEMORY_ALLOC) {
			printf("Allocated memory for operation id: %d totalling %d bytes vs sizeOf(Operation): %llu\n",
				_operation_id, operation->total_allocated_memory_bytes, sizeof(Operation));
		}
		operation->operand1 = _operand1;
		operation->total_allocated_memory_bytes += _operand1->total_allocated_memory_bytes;
		operation->operand2 = _operand2;
		operation->total_allocated_memory_bytes += _operand2->total_allocated_memory_bytes;
		operation->result = _result;
		operation->total_allocated_memory_bytes += _result->total_allocated_memory_bytes;
		operation->next = nullptr; // Initialize next pointer to null

		if (DEBUG_MEMORY_ALLOC) {
			printf("Total allocated memory for operation id: %d inclusive of matrices and data totalling %d bytes\n",
				_operation_id, operation->total_allocated_memory_bytes);
		}
		return operation;
	}
	else {
		fprintf(stderr, "Memory allocation for operation id: %d failed\n", _operation_id);
		return nullptr;
	}
}

int deallocate_matrix_data(MatrixData* _matrix_data, Matrix* _parent_matrix) {

	unsigned int	tmp_memory_size = 0;

	switch (_matrix_data->data_type) {
		case UNSIGNED_SHORT: {
			f_ree(_matrix_data->ushort_data);
			tmp_memory_size = _matrix_data->total_allocated_memory_bytes;
			_matrix_data->total_allocated_memory_bytes -= (short)_parent_matrix->rows * (short)_parent_matrix->cols * (short)sizeof(unsigned short int);
			break;
		}
		case UNSIGNED_INTEGER: {
			f_ree(_matrix_data->uint_data);
			tmp_memory_size = _matrix_data->total_allocated_memory_bytes;
			_matrix_data->total_allocated_memory_bytes -= (short)_parent_matrix->rows * (short)_parent_matrix->cols * (short)sizeof(unsigned int);
			break;
		}
		default: {
			std::cerr << "Deallocation - unknown data type" << std::endl;
			return EXIT_FAILURE;
		}
	}
	if (DEBUG_MEMORY_ALLOC) {
		printf("Deallocating %u bytes for matrix data with %d bytes remaining\n", tmp_memory_size, _matrix_data->total_allocated_memory_bytes);
	}
	f_ree(_matrix_data);

	return EXIT_SUCCESS;
}

int deallocate_matrix(Matrix* _matrix) {

	int tmp_memory_size = 0;

	if (_matrix == nullptr) {
		return EXIT_FAILURE;
	}
	if (_matrix->data != nullptr) {
		tmp_memory_size = _matrix->data->total_allocated_memory_bytes;
		if (deallocate_matrix_data(_matrix->data, _matrix) == EXIT_SUCCESS) {
			_matrix->total_allocated_memory_bytes -= tmp_memory_size;
		}
		else {
			std::cerr << "Failed to deallocate matrix data for Matrix id: " << _matrix->matrix_id << std::endl;
			return EXIT_FAILURE;
		}
	}
	else {
		std::cerr << "Matrix is NULL" << std::endl;
		return EXIT_FAILURE;
	}

	f_ree(_matrix);

	printf("Deallocated memory for Matrix: %d bytes that was remaining\n", tmp_memory_size);

	return SUCCESS;
}

int deallocate_operation(Operation* _operation) {

	int freed_memory = 0;
	if (_operation) {
		//freed_memory += _operation->operand1->total_allocated_memory_bytes;
		deallocate_matrix(_operation->operand1);
		//freed_memory += _operation->operand2->total_allocated_memory_bytes;
		deallocate_matrix(_operation->operand2);
		//freed_memory += _operation->result->total_allocated_memory_bytes;
		deallocate_matrix(_operation->result);

		freed_memory += _operation->total_allocated_memory_bytes;
		unsigned int operation_id = _operation->operation_id;
		f_ree(_operation);
		if (DEBUG_MEMORY_ALLOC) {
			printf("\nTotal memory freed by releasing Operation Id: %u = %u\n", operation_id, freed_memory);
			report_leaks("Deallocate Operation");
		}
	}
	else {
		std::cerr << "Operation is NULL" << std::endl;
	}

	return EXIT_SUCCESS;
}


int deallocate_all_operations(Operation* _operation_list_head ) {
	
	Operation* current_operation = _operation_list_head;
	Operation* next_operation = nullptr;

	while (current_operation != nullptr) {
		next_operation = current_operation->next; // Store the next operation
		deallocate_operation(current_operation); // Deallocate the current operation
		current_operation = next_operation; // Move to the next operation
	}
	return EXIT_SUCCESS;
}

// test functions
int test_allocate_matrix_data() { return EXIT_FAILURE; }
int test_allocate_matrix() { return EXIT_FAILURE; }
int test_allocate_operation() { return EXIT_FAILURE; }
int test_deallocate_matrix_data() { return EXIT_FAILURE; }
int test_deallocate_matrix() { return EXIT_FAILURE; }
int test_deallocate_operation() { return EXIT_FAILURE; }

