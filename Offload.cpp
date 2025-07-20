#include <iostream>
#include <assert.h>

#include "Parameters.h"

#include "MemTrack.h" // Memory tracking header
#include "Matrix.h"
#include "common.h"
#include "Persistence.h"
#include "MatrixAllocation.h"
#include "ProgramArguments.h"

#define MATRIX_OPERAND_1		101
#define MATRIX_OPERAND_2		102
#define MATRIX_RESULT			103


/* most the access memebers are located here in Offload.cpp */

struct MatrixInfo {
	int row_count;
	int col_count;
};

MatrixInfo* get_matrix_data_ushort(Matrix* _this_matrix, unsigned short* _matrix_data_to_passback) {

	MatrixInfo* matrix_info = (MatrixInfo*)c_alloc(SINGLE_INSTANCE, sizeof(MatrixInfo), "get_matrix_data", 0);
	if (!matrix_info) {
		fprintf(stderr, "Error allocating memory for matrix info\n");
		return nullptr;
	}
	matrix_info->row_count = _this_matrix->rows;
	matrix_info->col_count = _this_matrix->cols;

	_matrix_data_to_passback = _this_matrix->data->ushort_data;
	return matrix_info;
}

MatrixInfo* get_matrix_data_uint(Matrix* _this_matrix, unsigned int* _matrix_data_to_passback) {

	MatrixInfo* matrix_info = (MatrixInfo*)c_alloc(SINGLE_INSTANCE, sizeof(MatrixInfo), "get_matrix_data", 0);
	if (!matrix_info) {
		fprintf(stderr, "Error allocating memory for matrix info\n");
		return nullptr;
	}
	matrix_info->row_count = _this_matrix->rows;
	matrix_info->col_count = _this_matrix->cols;

	_matrix_data_to_passback = _this_matrix->data->uint_data;
	return matrix_info;
}

Matrix* get_operand_1(Operation* _operation) {

	if (!_operation) {
		fprintf(stderr, "Error accesing Operation\n");
		return nullptr;
	}

	Matrix* operand1 = _operation->operand1;
	return operand1;
}

Matrix* get_operand_2(Operation* _operation) {

	if (!_operation) {
		fprintf(stderr, "Error accesing Operation\n");
		return nullptr;
	}

	Matrix* operand2 = _operation->operand2;
	return operand2;
}

Matrix* get_result_matrix(Operation* _operation) {

	if (!_operation) {
		fprintf(stderr, "Error accesing Operation\n");
		return nullptr;
	}

	Matrix* result_matrix = _operation->result;
	return result_matrix;
}

Operation* get_next_operation(Operation* _current_operation_list) {

	if (!_current_operation_list) {
		fprintf(stderr, "Error returning next operation\n");
		return nullptr;
	}
	Operation* current_operation = _current_operation_list;
	return _current_operation_list->next;
}