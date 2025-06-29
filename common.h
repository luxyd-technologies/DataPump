#pragma once
#ifndef COMMON_H
#define COMMON_H

#define MSG_SYSTEM_OUTPUT 0
#define MSG_ERROR_OUTPUT 1
#define RESULT_ERROR		-1
#define RESULT_SUCCESS		0


int		print_message(int message_type, char* message);
char*	ushort_to_string(unsigned short int num);
char*	uint_to_string(unsigned int num);

//struct Matrix {
//	unsigned short int matrix_id; // Unique identifier for the matrix
//	unsigned short int operand_id; // Identifier for the operand
//    unsigned short int rows;
//    unsigned short int cols;
//    unsigned short int* ushort_data;
//    unsigned int* uint_data;
//	unsigned int matrix_type ; // default to operand type
//	
//    //unsigned short int data[rows][cols]; // Using a 2D array for simplicity, but this is not dynamic
//};
//
//struct Operation {
//    unsigned int operation_id; // Unique identifier for the operation
//	Matrix* operand1;   // Pointer to the first operand matrix
//    Matrix* operand2;   // Pointer to the second operand matrix
//    Matrix* result;     // Pointer to the result matrix
//};
//
//struct MatrixHeader {
//    unsigned short int matrix_id; // Unique identifier for the matrix
//    unsigned short int operand_id; // Identifier for the operand
//    unsigned short int rows;
//    unsigned short int cols;
//    unsigned short int matrix_type; // default to operand type
//};

//Operation*      create_operation(unsigned int _operation_id, Matrix* _matrix_operand1, Matrix* _matrix_operand2, Matrix* _matrix_result);  
//void            operation_print_info(const Operation* op);
//void            free_operation(Operation* op);
//
//Matrix*         create_matrix(int _matrix_id, int _operand_id, int _rows, int _cols, int _matrix_type);
//double          matrix_get_ushort(const Matrix* m, int row, int col); 
//double          matrix_get_uint(const Matrix* m, int row, int col);
//void            matrix_set_ushort(Matrix* m, int row, int col, unsigned short int value);
//void            matrix_set_uint(Matrix* m, int row, int col, unsigned short int value); 
//void            matrix_print_info(const Matrix* m);
//void            free_matrix(Matrix* m);
//
//unsigned short  generate_random_ushort(void);

//
//unsigned int    multiply_matrices(Matrix* _matrix1, Matrix* _matrix2, Matrix* _matrix_result);

#endif // COMMON_H