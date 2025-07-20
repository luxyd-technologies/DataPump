#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "Parameters.h"
#include "common.h"
#include "Matrix.h"
#include "Persistence.h"
#include "MemTrack.h"


FILE* open_matrix_file(const char* _filename, const char* _option) {

    FILE* file = fopen(_filename, _option);

    if (!file) {
        fprintf(stderr, "Failed to open file with name %s and option %s\n", _filename, _option);
        return nullptr; // Return nullptr if file opening fails
    }

    return file;
}

int close_matrix_file(FILE* _file) {
    if (_file) {
        fclose(_file);
        return EXIT_SUCCESS; // Success
    }
    else {
        fprintf(stderr, "File pointer is NULL, cannot close file.\n");
        return EXIT_FAILURE; // Error
    }
}

int save_matrix_tofile(FILE* file, int _operation_id, int operand_no, Matrix* _matrix) {

    if (!_matrix || !file) return -1; // Invalid matrix or file pointer

    int row_count = _matrix->rows;
    int col_count = _matrix->cols;

    if (_matrix->matrix_type == MATRIX_TYPE_OPERAND) {

        if (DEBUG_PERSISTENCE) {
            printf("\t\tOperation id %d - Saving operand %d with matrix id %d size (%d x %d) to file...\n", _operation_id, operand_no, _matrix->matrix_id, _matrix->rows, _matrix->cols);
        }

        // Write matrix dimensions
        fprintf(file, "+\n%d,%d,%d,%d,%d\n", _matrix->matrix_id, _operation_id, row_count, col_count, _matrix->matrix_type);

        for (int row_counter = 0; row_counter < row_count; row_counter++) {
            char row_string[MAX_ROW_STRING_LENGTH] = "";
            for (int col_counter = 0; col_counter < col_count; col_counter++) {
                char col_string[20] = ""; // Temporary string for column value
                unsigned int value = (unsigned int)matrix_get_ushort(_matrix, row_counter, col_counter);
                fprintf(file, "%hu", value); // Write ushort data as string
                sprintf(col_string, "%hu", value); // Debug output
                strcat(row_string, col_string); // Append comma for readability
                if (col_counter < col_count - 1) { // If not the last column, write a comma
                    fprintf(file, ","); // Write space after comma for readability  
                    strcat(row_string, ","); // Append comma for readability
                }
            }
            fprintf(file, "\n"); // next line after the last column
            if (DEBUG_PERSISTENCE) {
                printf("\t\t%s\n", row_string); // Debug output
            }
        }
        //fwrite("*\n", sizeof(char), strlen("*\n"), file);
        fprintf(file, "*\n");
        if (DEBUG_PERSISTENCE) {
            printf("\t\tOperation id %d - Operand %d with matrix id %d saved to file successfully\n\n", _operation_id, operand_no, _matrix->matrix_id);
        }
        //     if (DEBUG_MEMORY_ALLOC) {
                 //report_leaks("save_matrix_tofile - SHORT"); // Report memory leaks if any
        //     }
    }

    else if (_matrix->matrix_type == MATRIX_TYPE_RESULT) {

        if (DEBUG_PERSISTENCE) {
            printf("\t\tOperation id %d - Saving result matrix with matrix id %d size (%d x %d) to file...\n", _operation_id, _matrix->matrix_id, _matrix->rows, _matrix->cols);
        }

        fprintf(file, "$\n%d,%d,%d,%d,%d\n", _matrix->matrix_id, _operation_id, row_count, col_count, _matrix->matrix_type); // Write matrix dimensions
        for (int row_counter = 0; row_counter < row_count; row_counter++) {
            char row_string[MAX_ROW_STRING_LENGTH] = "";
            for (int col_counter = 0; col_counter < col_count; col_counter++) {
                char col_string[20] = ""; // Temporary string for column value
                unsigned int value = (unsigned int)matrix_get_uint(_matrix, row_counter, col_counter);
                fprintf(file, "%u", value); // Write uint data as string
                sprintf(col_string, "%u", value); // Debug output
                strcat(row_string, col_string); // Append comma for readability
                if (col_counter < col_count - 1) { // If not the last column, write a space
                    fprintf(file, ","); // Write ushort data as string
                    strcat(row_string, ","); // Append comma for readability
                }
            }
            fprintf(file, "\n"); // next line after the last column
            if (DEBUG_PERSISTENCE) {
                printf("\t\t%s\n", row_string); // Debug output
            }
        }
        fprintf(file, "-\n");
        if (DEBUG_PERSISTENCE) {
            printf("\t\tOperation id %d - Result matrix with matrix id %d saved to file successfully\n\n", _operation_id, _matrix->matrix_id);
        }
        //if (DEBUG_MEMORY_ALLOC) {
        //    report_leaks("save_matrix_tofile - INTEGER"); // Report memory leaks if any
        //}
    }
    else {
        return EXIT_FAILURE; // Invalid matrix type
    }

    return EXIT_SUCCESS; // Success
}

int save_operation_to_file(Operation* _operation, FILE* _open_file, const char* _filename) {

    if (!_operation || !_open_file) return EXIT_FAILURE; // Invalid operation or filename

    if (DEBUG_PERSISTENCE) {
        printf("\n\tSaving operation %d to file %s...\n\n", _operation->operation_id, _filename);
    }

    fwrite("[\n", sizeof(char), strlen("[\n"), _open_file);       // Write operation start marker 
    save_matrix_tofile(_open_file, _operation->operation_id, 1, _operation->operand1);         // write operand1 matrix to file    
    save_matrix_tofile(_open_file, _operation->operation_id, 2, _operation->operand2);         // write operand2 matrix to file
    save_matrix_tofile(_open_file, _operation->operation_id, 3, _operation->result);           // write result matrix to file
    fwrite("]\n", sizeof(char), strlen("]\n"), _open_file);       // Write operation end market 

    if (DEBUG_PERSISTENCE) {
        printf("\tOperation %d saved to file %s successfully\n", _operation->operation_id, _filename);
    }

    return EXIT_SUCCESS; // Success
}

int save_matrix_operations(Operation* _operation_list_head, FILE* _file, const char* _filename) {

    Operation* current_operation = _operation_list_head;
    Operation* next_operation = nullptr;

    int result = EXIT_SUCCESS;
    while (current_operation != nullptr) {
        next_operation = current_operation->next; // Store the next operation
        result = save_operation_to_file(current_operation, _file, _filename);
        if (result != EXIT_SUCCESS) {
            fprintf(stderr, "Failed to save operation %d to file %s\n", current_operation->operation_id, _filename);
            return EXIT_FAILURE; // Return failure if saving operation fails
        }
        current_operation = next_operation; // Move to the next operation
    }

    return EXIT_SUCCESS; // Placeholder for actual implementation
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

//extract operand header
int extract_operand_header(char* _line, OperandHeader* _operand_header) {

	int header[MATRIX_HEADER_SIZE] = { -1 };  // all array values initialized to -1

    // extract header
    int count = 0;
    char* token = strtok(_line, ",");
    while (token != NULL && count < MATRIX_HEADER_SIZE) {
        header[count] = atoi(token);
        count++;
        token = strtok(NULL, ",");
    }
    if (count != MATRIX_HEADER_SIZE) {
        fprintf(stderr, "Error: Header must have exactly %d elements\n", MATRIX_HEADER_SIZE);
        // line_count++;
        return EXIT_FAILURE;
    }

    _operand_header->matrix_id = header[0];
    _operand_header->operation_id = header[1];
    _operand_header->row_count = header[2];
    _operand_header->col_count = header[3];
    _operand_header->matrix_type = header[4]; // 0 for operand, 1 for result

    return EXIT_SUCCESS;
}

// extract matrix row from an operand matrix line, need to allocate memory for the values array before calling this function
unsigned short* extract_operand_matrix_row( char* _line, unsigned short _col_count, unsigned short* _values) {

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
            if (DEBUG_PERSISTENCE) {
                printf("%hu,",_values[count]);
            }
        }
        if (DEBUG_PERSISTENCE) {
            printf("\n");
		}
       
    }
    else {
        debug_print("Memory allocation failed for values.\n", "");
        return NULL; // Handle memory allocation failure
    }
	
    return _values; // placeholder, should return a 0

}

// extract matrix row from an result matrix line, need to allocate memory for the values array before calling this function
unsigned int* extract_result_matrix_row(char* _line, unsigned short _col_count, unsigned int* _values) {

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
         debug_print("Memory allocation failed for values\n", ""); 
         return NULL; // Handle memory allocation failure
     }
     return _values; // placeholder, should return a 0

 }

// Matrix** extract_matrix_operand( char* _line, int _matrix_id, int _operand_id, int _row_count, int _col_count, int _matrix_type) {
//
//    unsigned short* values = (unsigned short*) c_alloc(_row_count * _col_count, sizeof(unsigned short));
//	 Matrix** matrix = create_matrix( _matrix_id, _operand_id, _row_count, _col_count, _matrix_type); 
//
//    if ( (values) && (matrix)) {
//        //debug_print("Allocated memory for values.\n", "");
//		for (int row_counter = 0; row_counter < _row_count; row_counter++) {        // iterate through rows
//			char* token = strtok(_line, ",");                                       // get first short value
//            int counter = 0;
//            while (token != NULL && counter < _col_count) {
//                unsigned int value = (unsigned int)atoi(token);
//                values[counter] = value;
//				matrix_set_ushort(matrix, row_counter, counter, value); // set value in matrix
//                token = strtok(NULL, ",");
//                counter++;
//            }
//            for (int count = 0; count < _col_count; count++) {
//                debug_print("%d,", (char*)values[count]);
//            }
//            debug_print("\n", "");
//        }
//    }
//    else {
//        debug_print("Memory allocation failed in extract_matrix_operand\n", "");
//        return nullptr; // Handle memory allocation failure
//    }
//    f_ree(values);
//    return matrix; // placeholder, should return a valid Matrix pointer
//}


Operation* load_matrix_operations() {

    return nullptr; // Placeholder for actual implementation
}

int test_load_matrix_operations() {
	return EXIT_FAILURE; // Placeholder for actual implementation
}

int test_save_matrix_operations() {
	return EXIT_FAILURE; // Placeholder for actual implementation
}

Operation*  load_matrix_test_data(FILE* _file, Operation* _operation_list) {

	char filename[MAX_FILENAME_LENGTH] = "";
    char line[MAX_LINE];
    int state = STATE_OUT_OF_OPERATION;
    int matrix_count = 0;
	int line_count = 0;
	int current_matrix_row_count = NULL;
	int current_matrix_col_count = NULL;
    int current_row_counter = 0;
	int operation_count = 0;

    short is_first_operand = 1;
	short head = 0; // Flag to indicate if this is the first operation
	Matrix* current_matrix = nullptr;
	Operation* current_operation = nullptr;
	Operation* operation_list_head = nullptr; // Head of the operation list
	Operation* previous_operation = nullptr; // Pointer to the previous operation
	
    OperandHeader* current_matrix_header = nullptr; 

	while (fgets(line, MAX_LINE, _file) != NULL) {      // get a line from the file into line buffer max size = 10,000

        // Remove newline character if present
        size_t len = strlen(line);

        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

		// detect beginning of operation
        // we don't create a new operation here, we create when we detect the end of the operation
        if (strcmp(line, "[") == 0) {                       
            if (state == STATE_OUT_OF_OPERATION) {
                debug_print("%s\n", "Start Operation");

                // create a new operation
				current_operation = create_operation(operation_count, nullptr, nullptr, nullptr); // Create an empty operation
                if (current_operation == nullptr) {
                    fprintf(stderr, "Failed to create current operation\n");
                    return nullptr; // Exit on error
                }
				
                state = STATE_INSIDE_OPERATION;
                matrix_count = 0;
                line_count++;
                continue;
            }
            else {
                fprintf(stderr, "Unexpected start of operation at line %d\n", line_count);
                return nullptr; // Exit on unexpected state
            }
        }

        // detect end of operation
        // we create the operation here, after we have read all matrices
        else if (strcmp(line, "]") == 0) {
            if (state == STATE_INSIDE_OPERATION) {
                printf("End of Operation with Id:%d\n", current_operation->operation_id);
                //_operations[operation_count] = *current_operation; // Store the current operation in the _operations array

                if (head == 0) { // If this is the first operation
                    head = 1; // Set head to 1 if this is the first operation
                    operation_list_head = current_operation; // Set the head of the operation list
                    previous_operation = current_operation; // Set the previous operation to the current one, only done once
                }
                else { // If this is not the first operation
                    previous_operation->next = current_operation; // Link the previous operation to the current one
                    previous_operation = current_operation; // Update the previous operation to the current one
				}

                operation_count++;
                current_operation = nullptr; // Reset current operation
                state = STATE_OUT_OF_OPERATION;
                line_count++;
                continue;
            }
        }

        // detect beginning of operand matrix
        // we don't create a matrix here, we create when we detect the end of the operand matrix
        else if (strcmp(line, "+") == 0) {
            if (state == STATE_INSIDE_OPERATION) {
                debug_print("%s\n", "\tStart Operand Matrix");
                state = READ_OPERAND_MATRIX;
                current_row_counter = 0;
                line_count++;
                continue;
            }
        }

		// detect end of operand matrix, but still inside operation
        // we still don't create the matrix here, we create it when we are inside the operand matrix 
        else if (strcmp(line, "*") == 0) {
            if (state == STATE_IN_OPERAND_MATRIX) {
                debug_print("%s\n", "\tEnd Operand Matrix");
                if (is_first_operand) {
                    current_operation->operand1 = current_matrix; // Set the first operand matrix in the current operation
                    is_first_operand = 0; // Set to false after first operand
                }
                else {
                    current_operation->operand2 = current_matrix; // Set the second operand matrix in the current operation
					is_first_operand = 1; // Reset for the first operand of the next operation
				}
                
				f_ree(current_matrix_header); // Free the current matrix header if it was allocated
                state = STATE_INSIDE_OPERATION;
                line_count++;
                continue;
            }
        }

		// detect beginning of result matrix
         // we still don't create the matrix here, we create it when we are inside the result matrix
        else if (strcmp(line, "$") == 0) {
            if (state == STATE_INSIDE_OPERATION) {
                debug_print("%s\n", "\tStart Result Matrix");
                state = READ_RESULT_MATRIX;
                line_count++;
                continue;
            }
        }

		// detect end of result matrix, but still inside operation
		// we still don't create the matrix here, we create it when we are inside the result matrix
        else if (strcmp(line, "-") == 0) {
            if (state == STATE_IN_RESULT_MATRIX) {
                debug_print("%s\n", "\tEnd Result Matrix");
				current_operation->result = current_matrix; // Set the result matrix in the current operation

                // free current matrix header
				f_ree(current_matrix_header); // Free the current matrix header if it was allocated
                state = STATE_INSIDE_OPERATION;
                line_count++;
                continue;
            }
        }

        // if already inside an operand matrix
        else if (state == READ_OPERAND_MATRIX) {

			current_matrix_header = (OperandHeader*) c_alloc(MATRIX_HEADER_SIZE, 
                sizeof(OperandHeader), "load_matrix_test_data", 0);

            if (current_matrix_header == nullptr) {
                fprintf(stderr, "Memory allocation failed for operand header.\n");
                break;
			}

			// extract operand header from the line
			int result = extract_operand_header(line, current_matrix_header);
            if (result != EXIT_SUCCESS) {
                fprintf(stderr, "Failed to extract operand header from line: %s\n", line);
                f_ree(current_matrix_header);
                break; // Exit on error
			}
			line_count++;
            //wrong
			//current_operation->operation_id = curent_operand_header->operation_id; // Set operation ID

            current_row_counter = 0; // reset row counter for new matrix, will be reset every new operand matrix
            // create a new empty but initialized matrix for the operand
            current_matrix = create_matrix(SINGLE_INSTANCE, current_matrix_header->operation_id, current_matrix_header->matrix_id,
                current_matrix_header->row_count, current_matrix_header->col_count, MATRIX_TYPE_OPERAND);
            if (current_matrix == nullptr) {
                fprintf(stderr, "Failed to create current matrix for operand\n");
                f_ree(current_matrix_header);
                break; // Exit on error
            }
			// set current matrix row and column count
            current_matrix_row_count = current_matrix_header->row_count; // Get row count from header
			current_matrix_col_count = current_matrix_header->col_count; // Get column count from header
			state = STATE_IN_OPERAND_MATRIX;                    // move to state of reading operand matrix data

            line_count++;
            continue;
        }

		// extract matrix data from an operand matrix lines
        else if (state == STATE_IN_OPERAND_MATRIX) {
            // here current matrix already exists
            unsigned short* values = nullptr;
            values = (unsigned short*) (c_alloc(current_matrix_col_count, sizeof(unsigned short), "extract_operand_matrix_row", 0));
            values = extract_operand_matrix_row(line, current_matrix_col_count, values);
            for (int counter = 0; counter < current_matrix_col_count; counter++) {
                //printf("Matrix id:%d\t Current row:%d\t Current Col:%d\t Value:%hu\n", 
                //    current_matrix->matrix_id, current_row_counter, counter, values[counter]);
                matrix_set_ushort(current_matrix, current_row_counter, counter, values[counter]); // set first value as an example
            }
            current_row_counter++;          // move to next matrix row
            f_ree(values);                  // release memory used for values
            continue;
        }

        // extract matrix data from an result matrix lines
        else if (state == READ_RESULT_MATRIX) {

            current_matrix_header = (OperandHeader*)c_alloc(MATRIX_HEADER_SIZE, 
                sizeof(OperandHeader), "load_matrix_test_data", 0);
            if (current_matrix_header == nullptr) {
                fprintf(stderr, "Memory allocation failed for result header\n");
                break;
            }

            // extract operand header from the line
            int result = extract_operand_header(line, current_matrix_header);
            if (result != EXIT_SUCCESS) {
                fprintf(stderr, "Failed to extract operand header from line: %s\n", line);
                f_ree(current_matrix_header);
                break; // Exit on error
            }
            line_count++;

            current_row_counter = 0; // reset row counter for new matrix, will be reset every new operand result matrix
            // create a new empty but initialized matrix for the result
            current_matrix = create_matrix(SINGLE_INSTANCE, current_matrix_header->operation_id, current_matrix_header->matrix_id,
                current_matrix_header->row_count, current_matrix_header->col_count, MATRIX_TYPE_RESULT);
            if (current_matrix == nullptr) {
                fprintf(stderr, "Failed to create current matrix for operand\n");
                f_ree(current_matrix_header);
                break; // Exit on error
            }

            // set current matrix row and column count
            current_matrix_row_count = current_matrix_header->row_count; // Get row count from header
            current_matrix_col_count = current_matrix_header->col_count; // Get column count from header
			line_count++;                               // go to next line in the next iteration
            
            state = STATE_IN_RESULT_MATRIX;             // get ready to read result matrix data from file
            continue;

        }

        else if (state == STATE_IN_RESULT_MATRIX) {
            // here current matrix already exists
            unsigned int* values = nullptr;
            values = (unsigned int*)c_alloc(current_matrix_col_count, sizeof(unsigned int), "extract_result_matrix_row", 0);
            values = extract_result_matrix_row(line, current_matrix_col_count, values);
            for (int counter = 0; counter < current_matrix_col_count; counter++) {
                matrix_set_uint(current_matrix, current_row_counter, counter, values[counter]); // set first value as an example
            }
            f_ree(values);
            current_row_counter++;
            continue;
        }

    }

    return operation_list_head;
}