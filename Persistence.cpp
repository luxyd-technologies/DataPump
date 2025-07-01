#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "common.h"
#include "Matrix.h"
#include "Persistence.h"

// state definitions
#define STATE_OUT_OF_OPERATION      0
#define STATE_INSIDE_OPERATION      1
#define STATE_IN_OPERAND_MATRIX     2
#define STATE_NEXT_OPERAND          3
#define STATE_IN_RESULT_MATRIX      4

// 
#define READ_OPERAND_MATRIX 10
#define READ_RESULT_MATRIX  11

#define MATRIX_COUNT_PER_OPERATION 3
#define MATRIX_HEADER_SIZE 5

#define MAX_LINE 10000
#define DEBUG_1 1

void debug_print(const char* _message_format, const char* _message) {
    if (DEBUG_1) {
        printf(_message_format, _message);
    }
}

unsigned short* extract_operand_matrix_row( char* _line, unsigned short _col_count, unsigned short* _values) {

    _values = static_cast<unsigned short*>(calloc(_col_count, sizeof(unsigned short)));

    if (_values) {
       //debug_print("Allocated memory for values.\n", "");
        char* token = strtok(_line, ",");
        int counter = 0;
        while (token != NULL && counter < _col_count) {
			unsigned short value = (unsigned short) atoi(token);
            _values[counter] = value;
            token = strtok(NULL, ",");
            counter++;
        }
        for (int count = 0; count < _col_count; count++) {
            //if (values != NULL) {
                debug_print("%hu,", (char*)_values[count]);
            //}
        }
        debug_print("\n", "");
    }
    else {
        debug_print("Memory allocation failed for values.\n", "");
        return NULL; // Handle memory allocation failure
    }
	
    return _values; // placeholder, should return a 0

}

unsigned int* extract_result_matrix_row(char* _line, unsigned short _col_count, unsigned int* _values) {

     _values = static_cast<unsigned int*>(calloc(_col_count, sizeof(unsigned int)));

     if (_values) {
         //debug_print("Allocated memory for values.\n", "");
         char* token = strtok(_line, ",");
         int counter = 0;
         while (token != NULL && counter < _col_count) {
             unsigned int value = (unsigned int)atoi(token);
             _values[counter] = value;
             token = strtok(NULL, ",");
             counter++;
         }

     }
     else {
         debug_print("Memory allocation failed for values.\n", ""); 
         return NULL; // Handle memory allocation failure
     }

     return _values; // placeholder, should return a 0

 }

Matrix* extract_matrix_operand( char* _line, int _matrix_id, int _operand_id, int _row_count, int _col_count, int _matrix_type) {

    unsigned short* values = static_cast<unsigned short*>(calloc(_row_count * _col_count, sizeof(unsigned short)));
	Matrix* matrix = create_matrix( _matrix_id, _operand_id, _row_count, _col_count, _matrix_type); 

    if ( (values) && (matrix)) {
        //debug_print("Allocated memory for values.\n", "");
		for (int row_counter = 0; row_counter < _row_count; row_counter++) {        // iterate through rows
			char* token = strtok(_line, ",");                                       // get first short value
            int counter = 0;
            while (token != NULL && counter < _col_count) {
                unsigned int value = (unsigned int)atoi(token);
                values[counter] = value;
				matrix_set_ushort(matrix, row_counter, counter, value); // set value in matrix
                token = strtok(NULL, ",");
                counter++;
            }
            for (int count = 0; count < _col_count; count++) {
                debug_print("%d,", (char*)values[count]);
            }
            debug_print("\n", "");
        }
    }
    else {
        debug_print("Memory allocation failed in extract_matrix_operand\n", "");
        return nullptr; // Handle memory allocation failure
    }
    free(values);
    return matrix; // placeholder, should return a valid Matrix pointer
}

FILE* open_matrix_file(const char* filename, const char* option) {
    FILE* file = fopen(filename, option);
    if (!file) {
        perror("fopen");
        exit(1);
    }
    return file;
}

char* read_rawline(FILE* file, char* buffer, int size) {
    if (fgets(buffer, size, file) == NULL) {
        return NULL; // EOF or error
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0'; // Remove newline character
    }
    return buffer;
}

int  test_persistence(char* _filename, Operation* _operations) {
//int main(int argc, char* argv[]) {

	char filename[MAX_FILENAME_LENGTH] = "";
    char line[MAX_LINE];
    int state = STATE_OUT_OF_OPERATION;
    int matrix_count = 0;
	int line_count = 0;
	int row_count = 0;
	int col_count = 0;
    int current_row_counter = 0;
	int operation_count = 0;
	Matrix* current_matrix = NULL;
	Operation* current_operation = NULL;
    //Operation* _operations[MAX_OPERATION_COUNT] = { nullptr, nullptr, nullptr };


    if (_operations == nullptr) {
        printf("Using MatrixSample.txt as the matrix input file\n");
		strcat(filename, "MatrixSample.txt"); // set to Default filename
        return RESULT_ERROR; // Handle error
	}
    else {
        printf("Using %s as the matrix input file\n", _filename);
        strcat(filename, _filename);
	}

    FILE* file = open_matrix_file(filename, "r");

    while (fgets(line, MAX_LINE, file) != NULL) {

        //debug_print("Read Raw line: %s", line);
        // Remove newline character if present
        size_t len = strlen(line);

        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (strcmp(line, "[") == 0) {
            if (state == STATE_OUT_OF_OPERATION) {
                debug_print("%s\n", "Start Operation");
				current_operation = create_operation(NULL, nullptr, nullptr, nullptr);
                state = STATE_INSIDE_OPERATION;
                matrix_count = 0;
                line_count++;
                continue;
            }
        }
        else if (strcmp(line, "+") == 0) {
            if (state == STATE_INSIDE_OPERATION) {
                debug_print("%s\n", "\tStart Operand Matrix");
                state = READ_OPERAND_MATRIX;
                current_row_counter = 0;
                line_count++;
                continue;
            }
        }

        else if (strcmp(line, "*") == 0) {
            if (state == STATE_IN_OPERAND_MATRIX) {
                debug_print("%s\n", "\tEnd Operand Matrix");
                state = STATE_INSIDE_OPERATION;
                line_count++;
                continue;
            }
        }

        else if (strcmp(line, "$") == 0) {
            if (state == STATE_INSIDE_OPERATION) {
                debug_print("%s\n", "\tStart Result Matrix");
                state = READ_RESULT_MATRIX;
                line_count++;
                continue;
            }
        }

        else if (strcmp(line, "-") == 0) {
            if (state == STATE_IN_RESULT_MATRIX) {
                debug_print("%s\n", "\tEnd Result Matrix");
                state = STATE_INSIDE_OPERATION;
                line_count++;
                continue;
            }
        }

        else if (strcmp(line, "]") == 0) {
            if (state == STATE_INSIDE_OPERATION) {
                printf("End of Operation with Id:%d\n", current_operation->operation_id);
				_operations[operation_count] = *current_operation; // Store the current operation in the _operations array
                operation_count++;
				current_operation = nullptr; // Reset current operation
                state = STATE_OUT_OF_OPERATION;
                line_count++;
                continue;
            }
        }

        else if (state == READ_OPERAND_MATRIX) {
            // extract header
            int header[MATRIX_HEADER_SIZE];
            int count = 0;
            char* token = strtok(line, ",");
            while (token != NULL && count < MATRIX_HEADER_SIZE) {
                header[count] = atoi(token);
                count++;
                token = strtok(NULL, ",");
            }
            if (count != MATRIX_HEADER_SIZE) {
                fprintf(stderr, "Error: Header must have exactly %d integers.\n", MATRIX_HEADER_SIZE);
                line_count++;
                break;
            }
            int matrix_id = header[0];
            int operation_id = header[1];
            row_count = header[2];
            col_count = header[3];
            int matrix_type = header[4]; // 0 for operand, 1 for result
            current_matrix = create_matrix(matrix_id, operation_id, row_count, col_count, matrix_type);

            if (current_operation->operand1 == nullptr) {
                current_operation->operation_id = operation_id;
                current_operation->operand1 = current_matrix; // set first operand
            }
            else if (current_operation->operand2 == nullptr) {
                current_operation->operand2 = current_matrix; // set second operand
            }
            else {
                fprintf(stderr, "Current Operation Error, Id:%d\n", current_operation->operation_id);
            }

            current_row_counter = 0; // reset row counter for new matrix
            state = STATE_IN_OPERAND_MATRIX;

            // print header for debugging
            //for (int i = 0; i < count; i++) {
            //    printf("Header[%d]: %d\n", i, header[i]);
            //    // extract operand matrix data
            //    // increment matrix count
            //}

            line_count++;
            continue;
        }

        else if (state == READ_RESULT_MATRIX) {
            // extract header
            int header[MATRIX_HEADER_SIZE];
            int count = 0;
            char* token = strtok(line, ",");
            while (token != NULL && count < MATRIX_HEADER_SIZE) {
                header[count] = atoi(token);
                count++;
                token = strtok(NULL, ",");
            }
            if (count != MATRIX_HEADER_SIZE) {
                fprintf(stderr, "Error: Header must have exactly %d integers.\n", MATRIX_HEADER_SIZE);
                line_count++;
                break;
            }

            int matrix_id = header[0];
            int operation_id = header[1];
            row_count = header[2];
            col_count = header[3];
            int matrix_type = header[4]; // 0 for operand, 1 for result
            current_matrix = create_matrix(matrix_id, operation_id, row_count, col_count, matrix_type);
            if (current_operation->result == nullptr) {
				// assert (current_operation->operation_id == nullptr && current_operation->operand2 != nullptr);
                current_operation->result = current_matrix; // set first operand
            }
            else {
                fprintf(stderr, "Trying to assign result matrix when result matrix pointer not null, Id:%d\n", current_operation->operation_id);
            }
            current_row_counter = 0; // reset row counter for new matrix
            state = STATE_IN_RESULT_MATRIX;
            continue;
            //print header for debugging
            //for (int i = 0; i < count; i++) {
                //printf("Header[%d]: %d\n", i, header[i]);
            //}

        }

        else if (state == STATE_IN_OPERAND_MATRIX) {
            // here current matrix already exists
            unsigned short* values = NULL;
            values = extract_operand_matrix_row(line, col_count, values);
            for (int counter = 0; counter < col_count; counter++) {
                //printf("Matrix id:%d\t Current row:%d\t Current Col:%d\t Value:%hu\n", current_matrix->matrix_id, current_row_counter, counter, values[counter]);
                matrix_set_ushort(current_matrix, current_row_counter, counter, values[counter]); // set first value as an example
            }
            current_row_counter++;
            continue;
        }

        else if (state == STATE_IN_RESULT_MATRIX) {
            // here current matrix already exists
            unsigned int* values = NULL;
            values = extract_result_matrix_row(line, col_count, values);
            for (int counter = 0; counter < col_count; counter++) {
                matrix_set_uint(current_matrix, current_row_counter, counter, values[counter]); // set first value as an example
            }
            current_row_counter++;
            continue;
        }

    }

//cleanup:
    fclose(file);

    for (int i = 0; i < operation_count; i++) {
		
   //     if (_operations[i] != nullptr) {
			operation_print_info(&_operations[i]); // Print each operation
   //     }
	}
    return 0;
}