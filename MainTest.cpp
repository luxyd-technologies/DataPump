#include <iostream>
#include <assert.h>

#include "Parameters.h"

#include "MemTrack.h" // Memory tracking header
#include "Matrix.h"
#include "common.h"

#include "Persistence.h"
#include "MatrixAllocation.h"

// no longer needed as matrix data in Matrix 
int static test_allocate_matrix_data() {

	//MatrixData* matrix_data1 = nullptr;
	//MatrixData* matrix_data2 = nullptr;

	//matrix_data1 = allocate_matrix_data(UNSIGNED_SHORT, 20, 100);
	//assert(matrix_data1 != NULL);
	//matrix_data2 = allocate_matrix_data(UNSIGNED_SHORT, 20, 100);
	//assert(matrix_data2 != NULL);
	//report_leaks("Test allocate matrix data point 1");
	//assert(deallocate_matrix_data(matrix_data, ) == EXIT_SUCCESS);
	//assert(deallocate_matrix_data(matrix_data2) == EXIT_SUCCESS);
	//report_leaks("Test allocate matrix data point 2");
	return EXIT_SUCCESS;
}

int static test_allocate_matrix() {

		int test_matrix1_row_count = 20;
		int test_matrix1_col_count = 100;
		int test_matrix2_row_count = 100;
		int test_matrix2_col_count = 25;

		//int test_matrix1_row_count = 4;
		//int test_matrix1_col_count = 2;
		//int test_matrix2_row_count = 2;
		//int test_matrix2_col_count = 3;

		MatrixData* matrix_data1 = nullptr;
		MatrixData* matrix_data2 = nullptr;
		MatrixData* result_matrixdata = nullptr;

		Matrix* matrix1 = nullptr;
		Matrix* matrix2 = nullptr;
		Matrix* result_matrix = nullptr;

		// no longer required
		//matrix_data1 = allocate_matrix_data(UNSIGNED_SHORT, test_matrix1_row_count, test_matrix1_col_count);
		//assert(matrix_data1 != NULL);
		////matrix_data1 = init_matrix_data(matrix_data1);0
		//matrix_data2 = allocate_matrix_data(UNSIGNED_SHORT, test_matrix2_row_count, test_matrix2_col_count);
		//assert(matrix_data2 != NULL);
		//matrix_data2 = init_matrix_data(matrix_data2);
		//result_matrixdata = allocate_memory_matrix_data(UNSIGNED_INTEGER, test_matrix1_row_count, test_matrix2_col_count);
		//assert(result_matrixdata != NULL);
		//result_matrixdata = init_matrix_data(result_matrixdata);

		int operation_id = 0;
		int matrix_id = 1;
		matrix1 = allocate_matrix_with_matrix_data(
			operation_id, matrix_id, test_matrix1_row_count, test_matrix1_col_count, MATRIX_TYPE_OPERAND);
		assert(matrix1 != NULL); // Placeholder for actual matrix allocation test

		matrix_id = 2; // Change matrix_id for the second matrix
		matrix2 = allocate_matrix_with_matrix_data(operation_id, matrix_id, test_matrix2_row_count, test_matrix2_col_count, MATRIX_TYPE_OPERAND);
		assert(matrix2 != NULL); // Placeholder for actual matrix allocation test

		deallocate_matrix(matrix1);
		deallocate_matrix(matrix2);
		
		//test_deallocate_matrix(matrix1);
		//test_deallocate_matrix(matrix2);
	
	return EXIT_SUCCESS;
}

int static test_allocation_operation() {

	//int test_matrix1_row_count = 20;
	//int test_matrix1_col_count = 100;
	//int test_matrix2_row_count = 100;
	//int test_matrix2_col_count = 25;

	int test_matrix1_row_count = 4;
	int test_matrix1_col_count = 2;
	int test_matrix2_row_count = 2;
	int test_matrix2_col_count = 4;
	int g_status = 0;

	MatrixData* matrix_data1 = nullptr;
	Matrix* matrix1 = nullptr;

	MatrixData* matrix_data2 = nullptr;
	Matrix* matrix2 = nullptr;

	MatrixData* result_matrixdata = nullptr;
	Matrix* result_matrix = nullptr;

	Operation* operation = nullptr;

	// no longer needed as matrix data in Matrix
	//matrix_data1 = allocate_matrix_data(UNSIGNED_SHORT, test_matrix1_row_count, test_matrix1_col_count);
	//assert(matrix_data1 != NULL);
	//g_status = 2;
	//matrix_data1 = init_matrix_data(matrix_data1);
	//matrix_data2 = allocate_matrix_data(UNSIGNED_SHORT, test_matrix2_row_count, test_matrix2_col_count);
	//assert(matrix_data2 != NULL);
	//matrix_data2 = init_matrix_data(matrix_data2);
	//result_matrixdata = allocate_memory_matrix_data(UNSIGNED_INTEGER, test_matrix1_row_count, test_matrix2_col_count);
	//assert(result_matrixdata != NULL);
	//result_matrixdata = init_matrix_data(result_matrixdata);

	g_status = 1;
	int operation_id = 1;
	int matrix_id = 0;
	g_status = 3;
	matrix1 = allocate_matrix_with_matrix_data(operation_id, matrix_id++, test_matrix1_row_count, test_matrix1_col_count, MATRIX_TYPE_OPERAND);
	assert(matrix1 != NULL); // Placeholder for actual matrix allocation test

	g_status = 4;
	matrix_id = 2; // Change matrix_id for the second matrix
	matrix2 = allocate_matrix_with_matrix_data(operation_id, matrix_id++, test_matrix2_row_count, test_matrix2_col_count, MATRIX_TYPE_OPERAND);
	assert(matrix2 != NULL); // Placeholder for actual matrix allocation test

	//g_status = 5;
	//result_matrix = allocate_matrix(operation_id, matrix_id++, test_matrix1_row_count, test_matrix2_col_count, result_matrixdata);
	//assert(result_matrix != NULL); // Placeholder for actual matrix allocation test

	// multiply matrices
	g_status = 6;
	matrix_id = 3;
	//Matrix* multiplication_result = calc_matrix_multiplication(matrix1, matrix2, matrix_id);

	g_status = 7;
	//operation = allocate_operation(operation_id, matrix1, matrix2, multiplication_result);

	operation_print_info(operation);

	deallocate_operation(operation);

	return EXIT_SUCCESS;
}

//int main() {
//
//
//	// test
//	assert(test_allocate_matrix_data() == EXIT_SUCCESS);
//	assert(test_allocate_matrix() == EXIT_SUCCESS);
//	assert(test_allocation_operation() == EXIT_SUCCESS);
//	report_leaks();
//
//	return 0;
//
//}