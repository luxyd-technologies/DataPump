#pragma once

#ifndef MATRIX_ALLOCATION_H
#define MATRIX_ALLOCATION_H

#include "common.h"
#include "Matrix.h"

//int g_status = 0; // Global status variable

// allo
MatrixData* allocate_matrix_data(unsigned short _data_type, unsigned short int rows, unsigned short int cols);
Matrix*		allocate_matrix_with_matrix_data(unsigned short _operation_id, unsigned short _matrix_id, 
	unsigned short int rows, unsigned short int cols, int _matrix_type);

Operation* allocate_operation_empty(unsigned short _operation_id);
Operation*	allocate_operation(unsigned short _operation_id, Matrix* _operand1, Matrix* _operand2, Matrix* _result);

// deallocation functions
int deallocate_matrix_data(MatrixData* _matrix_data, Matrix* _parent_matrix);
int deallocate_matrix(Matrix* _matrix);
int deallocate_operation(Operation* _operation);
int deallocate_all_operations(Operation* _operation_list_head);

// test functions
int test_allocate_matrix_data();
int test_allocate_matrix();
int test_allocate_operation();
int test_deallocate_matrix_data();
int test_deallocate_matrix();
int test_deallocate_operation();

#endif// MATRIX_ALLOCATION_H