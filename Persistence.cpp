#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "Parameters.h"
#include "common.h"
#include "Matrix.h"
#include "Persistence.h"
#include "MemTrack.h"

unsigned short* extract_operand_matrix_row( char* _line, unsigned short _col_count, unsigned short* _values) {

    _values = (unsigned short*) (c_alloc(_col_count, sizeof(unsigned short), "extract_operand_matrix_row", 0));

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

    _values = (unsigned int*) c_alloc(_col_count, sizeof(unsigned int), "extract_result_matrix_row", 0);

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
        return EXIT_FAILURE ; // Error
    }
}

int save_matrix_tofile(FILE* file, int _operation_id, int operand_no, Matrix* _matrix ) {

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
                unsigned int value = (unsigned int) matrix_get_ushort(_matrix, row_counter, col_counter);
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
    save_matrix_tofile(_open_file, _operation->operation_id, 1, _operation->operand1 );         // write operand1 matrix to file    
    save_matrix_tofile(_open_file, _operation->operation_id, 2, _operation->operand2 );         // write operand2 matrix to file
    save_matrix_tofile(_open_file, _operation->operation_id, 3, _operation->result );           // write result matrix to file
    fwrite("]\n", sizeof(char), strlen("]\n"), _open_file);       // Write operation end market 

    if (DEBUG_PERSISTENCE) {
        printf("\tOperation %d saved to file %s successfully\n", _operation->operation_id, _filename);
    }

    return EXIT_SUCCESS; // Success
}



Operation* load_matrix_operations() {

	return nullptr; // Placeholder for actual implementation
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

int test_load_matrix_operations() {
	return EXIT_FAILURE; // Placeholder for actual implementation
}

int test_save_matrix_operations() {
	return EXIT_FAILURE; // Placeholder for actual implementation
}

int  test_persistence(char* _filename, Operation* _operations) {

	char filename[MAX_FILENAME_LENGTH] = "";
    char line[MAX_LINE];
    int state = STATE_OUT_OF_OPERATION;
    int matrix_count = 0;
	int line_count = 0;
	int row_count = 0;
	int col_count = 0;
    int current_row_counter = 0;
	int operation_count = 0;
	 Matrix** current_matrix = NULL;
	Operation* current_operation = NULL;
    //Operation* _operations[MAX_OPERATION_COUNT] = { nullptr, nullptr, nullptr };


    if (_operations == nullptr) {
        printf("Using MatrixSample.txt as the matrix input file\n");
		strcat(filename, "MatrixSample.txt"); // set to Default filename
        return EXIT_FAILURE; // Handle error
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
				//current_operation = create_operation(NULL, nullptr, nullptr, nullptr);
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
            //current_matrix = create_matrix(matrix_id, operation_id, row_count, col_count, matrix_type);

            if (current_operation->operand1 == nullptr) {
                current_operation->operation_id = operation_id;
                //current_operation->operand1 = current_matrix; // set first operand
            }
            else if (current_operation->operand2 == nullptr) {
                //current_operation->operand2 = current_matrix; // set second operand
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
            //current_matrix = create_matrix(matrix_id, operation_id, row_count, col_count, matrix_type);
            if (current_operation->result == nullptr) {
				// assert (current_operation->operation_id == nullptr && current_operation->operand2 != nullptr);
                //current_operation->result = current_matrix; // set first operand
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
                //matrix_set_ushort(current_matrix, current_row_counter, counter, values[counter]); // set first value as an example
            }
            current_row_counter++;
            f_ree(values);
            continue;
        }

        else if (state == STATE_IN_RESULT_MATRIX) {
            // here current matrix already exists
            unsigned int* values = NULL;
            values = extract_result_matrix_row(line, col_count, values);
            for (int counter = 0; counter < col_count; counter++) {
                //matrix_set_uint(current_matrix, current_row_counter, counter, values[counter]); // set first value as an example
            }
            f_ree(values);
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