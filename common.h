#pragma once

#ifndef COMMON_H
#define COMMON_H

#define NEW_STRUCTURE				1

#define MSG_SYSTEM_OUTPUT			0
#define MSG_ERROR_OUTPUT			1

#define SINGLE_INSTANCE				1	

#define MAX_OPERATION_COUNT			1000 // Maximum number of operations in a matrix operation
#define MAX_ROW_STRING_LENGTH		4096
#define MAX_MEMORY_ALLOCATIONS		4096 // Maximum number of memory allocations to track
#define MAX_FILENAME_LENGTH			256 // Maximum length for filenames

#define FAIL	-1
#define SUCCESS 0
#pragma once

#define SINGLE_INSTANCE				1

#define MATRIX_TYPE_OPERAND			0
#define MATRIX_TYPE_RESULT			1

// matrix data types
#define UNSIGNED_SHORT				21
#define UNSIGNED_INTEGER			22

// variable types
#define	VAR_TYPE_USHORT_INT			11
#define VAR_TYPE_UINTEGER			12
#define VAR_TYPE_MATRIX_DATA		13
#define VAR_TYPE_MATRIX				14
#define VAR_TYPE_OPERATION			15

// state definitions
#define STATE_OUT_OF_OPERATION      0
#define STATE_INSIDE_OPERATION      1
#define STATE_IN_OPERAND_MATRIX     2
#define STATE_NEXT_OPERAND          3
#define STATE_IN_RESULT_MATRIX      4

// Matrix reading states
#define READ_OPERAND_MATRIX			10
#define READ_RESULT_MATRIX			11

// Constants
#define MATRIX_COUNT_PER_OPERATION	3
#define MATRIX_HEADER_SIZE			5
#define MAX_LINE					10000
#define DEBUG_1						0

//unsigned int	generate_truly_random_uint();

unsigned short 			generate_random_ushort(void);
unsigned int			generate_truly_random_uint(void);
char*					ushort_to_string(unsigned short int num);
char*					uint_to_string(unsigned int num);

// I/O functions
int						print_message(int message_type, char* message);
void					debug_print(const char* _message_format, const char* _message);
void					printMemoryContents(const void* address, size_t size);

//struct MemoryManager* create_memory_manager(void);
//void free_memory_manager(struct MemoryManager* manager);

#endif //COMMON_H