#include <iostream>
#include <assert.h>

#include "Parameters.h"

#include "MemTrack.h" // Memory tracking header
#include "Matrix.h"
#include "common.h"
#include "Persistence.h"
#include "MatrixAllocation.h"
#include "ProgramArguments.h"


constexpr int EXECUTE_TEST = 0; // Maximum number of operations to handle

//int old_main(int argc, char* argv[]) {
//
//        // main program parameters
//	char matrix_filename[64] = { 0 }; // Buffer for filename
//    int matrix1_row_count = 0;
//	int matrix1_col_count = 0;
//	int matrix2_row_count = 0;
//	int matrix2_col_count = 0;
//
//    int result = 0;
//    int operation_count = 0;
//
//
//    result = parse_arguments(argc, argv, arguments);
//	matrix1_row_count = arguments->mat1_row_count; // Get row count for matrix 1
//	matrix1_col_count = arguments->mat1_col_count; // Get column count for matrix 1
//	matrix2_row_count = arguments->mat2_row_count; // Get row count for matrix 2
//	matrix2_col_count = arguments->mat2_col_count; // Get column count for matrix 2
//
//    if (arguments->generate_matrix_flag == 1) {             // if the execution mode is generate
//
//        strcpy(matrix_filename, arguments->filename);       // Copy from arguments the filename to save the matrices 
//
//
//		operation_count = generate_matrix_test_data( /*arguments, operations, ,*/ operation_count,
//            arguments->mat1_row_count, arguments->mat1_col_count, arguments->mat2_row_count, arguments->mat2_col_count);
//
//        FILE* file = fopen(matrix_filename, "w");
//
//        if (file == nullptr) {
//            fprintf(stderr, "Error opening file %s for writing\n", matrix_filename);
//            return -1; // Exit with error code
//        }
//        else {
//            fclose(file);
//            if (DEBUG_PERSISTENCE) {
//                printf("\nSaving generated matrix data to file : %s containing %d operations\n", matrix_filename, operation_count);
//            }
//            for (int i = 0; i < operation_count; i++, operations++) {
//                operation_print_info(&operations[i]);
//                //int save_result = save_operation_to_file(matrix_filename, operations); //TODO: remove this print, just for testing
//                deallocate_operation(operations);
//            }
//
//        }
//    }
//    else if (arguments->load_file_flag == 1) {
//        // Load matrix from file
//        char* matrix_filename = arguments->filename;
//        printf("Loading matrix from file: %s\n", matrix_filename);
//        result = load_matrix_test_data(matrix_filename, operations);
//        if (result != 0) {
//            fprintf(stderr, "Error loading operations from file.\n");
//            return -1; // Exit with error code
//        }
//
//        for (int i = 0; i < operation_count; i++) {
//            operation_print_info(&operations[i]);
//        }
//    }
//
//    else {
//        fprintf(stderr, "No valid command line arguments provided.\n");
//        return EXIT_FAILURE;
//	}
//	return EXIT_SUCCESS;
//}

Operation* generate_single_operation_test_data (
	int _matrix1_row_count, int _matrix1_col_count,
	int _matrix2_row_count, int _matrix2_col_count) {

	int			matrix_id_counter = 0;
	int			operation_count = 0;
	short		head = 0; // Head of the linked list of operations
	Operation*	previous_operation = nullptr; // Pointer to the previous operation
	Operation*	operation_list_head = nullptr; // Head of the linked list of operations

	if (DEBUG_MATRIX) {
		printf("Generating matrix test data...\n");
		//printf("Generating %d operations with the following matrix sizes:\n", _arguments->operation_count);
		printf("Matrix 1: %d rows x %d columns\n", _matrix1_row_count, _matrix1_col_count);
		printf("Matrix 2: %d rows x %d columns\n", _matrix2_row_count, _matrix2_col_count);
	}

	int operation_id_counter = 0;

	// Create matrix operand 1 
	Matrix* matrix1 = create_matrix(SINGLE_INSTANCE, operation_id_counter, matrix_id_counter++,
		_matrix1_row_count, _matrix1_col_count, MATRIX_TYPE_OPERAND);
	if (matrix1) {
		matrix1 = init_matrix_random(matrix1); // Initialize matrix with random values
	}
	else {
		return nullptr;
	}

	// Create matrix operand 2
	Matrix* matrix2 = create_matrix(SINGLE_INSTANCE, operation_id_counter, matrix_id_counter++,
		_matrix1_row_count, _matrix1_col_count, MATRIX_TYPE_OPERAND);
	matrix2 = init_matrix_random(matrix2); // Initialize matrix with random values
	if (matrix2) {
		matrix2 = init_matrix_random(matrix2); // Initialize matrix with random values
	}
	else {
		return nullptr;
	}

	// Create result matrix

	Matrix* result_matrix = nullptr;
	result_matrix = allocate_matrix_with_matrix_data(operation_id_counter,
		matrix_id_counter++, matrix1->rows, matrix2->cols, MATRIX_TYPE_RESULT);

	// create operation
	Operation* current_operation = create_operation(operation_id_counter, matrix1, matrix2, result_matrix);

	if (!current_operation) {
		fprintf(stderr, "Operation allocation failed!\n");
		//deallocate_matrix(matrix1); // Free allocated memory for matrix 1
		//deallocate_matrix(matrix2); // Free allocated memory for matrix 2
		//deallocate_matrix(result_matrix); // Free allocated memory for result matrix
		return nullptr;
	}
	operation_list_head = current_operation; // Set the head of the operation list

	if (DEBUG_MATRIX) {
		printf(" Ins: operation count:%d, current_operation: %p\n", operation_id_counter, current_operation); // Debug print
	}

	return operation_list_head; // Return success

}

// Generate matrix test data and save to file
Operation* generate_matrix_test_data(/*ProgramArguments* _arguments,  Operation* _operations, */
	int _operation_count,
	int _matrix1_row_count, int _matrix1_col_count,
	int _matrix2_row_count, int _matrix2_col_count) {

	int matrix_id_counter = 0;
	int operation_count = 0;
	short head = 0; // Head of the linked list of operations
	Operation* previous_operation = nullptr; // Pointer to the previous operation
	Operation* operation_list_head = nullptr; // Head of the linked list of operations

	if (DEBUG_MATRIX) {
		printf("Generating matrix test data...\n");
		//printf("Generating %d operations with the following matrix sizes:\n", _arguments->operation_count);
		printf("Matrix 1: %d rows x %d columns\n", _matrix1_row_count, _matrix1_col_count);
		printf("Matrix 2: %d rows x %d columns\n", _matrix2_row_count, _matrix2_col_count);
	}

	for (int operation_id_counter = 0; operation_id_counter < _operation_count; operation_id_counter++) {

		int matrix_id_counter = 0;
		if (DEBUG_MEMORY_ALLOC) {
			printf("\n<------ Operation Id %d Creation ---------> \n", operation_id_counter);
		}
		// Create matrix operand 1 
		Matrix* matrix1 = create_matrix(SINGLE_INSTANCE, operation_id_counter, matrix_id_counter++,
			_matrix1_row_count, _matrix1_col_count, MATRIX_TYPE_OPERAND);
		if (matrix1) {
			matrix1 = init_matrix_random(matrix1); // Initialize matrix with random values
		}
		else {
			return nullptr;
		}

		// Create matrix operand 2
		Matrix* matrix2 = create_matrix(SINGLE_INSTANCE, operation_id_counter, matrix_id_counter++,
			_matrix2_row_count, _matrix2_col_count, MATRIX_TYPE_OPERAND);
		matrix2 = init_matrix_random(matrix2); // Initialize matrix with random values
		if (matrix2) {
			matrix2 = init_matrix_random(matrix2); // Initialize matrix with random values
		}
		else {
			return nullptr;
		}

		// Create result matrix

		Matrix* result_matrix = nullptr;
		result_matrix = allocate_matrix_with_matrix_data(operation_id_counter,
			matrix_id_counter++, matrix1->rows, matrix2->cols, MATRIX_TYPE_RESULT);

		result_matrix = calc_matrix_multiplication(matrix1, matrix2, result_matrix); // Calculate the result matrix
		
		// create operation
		Operation* current_operation = create_operation(operation_id_counter, matrix1, matrix2, result_matrix);
		
		if (!current_operation) {
			fprintf(stderr, "Operation allocation failed!\n");
			//deallocate_matrix(matrix1); // Free allocated memory for matrix 1
			//deallocate_matrix(matrix2); // Free allocated memory for matrix 2
			//deallocate_matrix(result_matrix); // Free allocated memory for result matrix
			return nullptr;
		}


		if (head == 0) {									// if this is the first operation
			head = 1;										// Set head to 1 if this is the first operation
			operation_list_head = current_operation;		// Set the head of the operation list
			previous_operation = current_operation;			// Set the previous operation to the current one, only done once
		}
		else {												// if this is not the first operation
			previous_operation->next = current_operation;	// Link the previous operation to the current one
			previous_operation = current_operation;			// Update the previous operation to the current one
		}
		
		if (DEBUG_MEMORY_ALLOC) {
			printf("\n<------ Operation Id %d Creation End  ---------> \n\n", operation_id_counter);
		}
		////assert(_operations != NULL); // Ensure _operations is not null
		////_operations[operation_count] = *current_operation; // Store the current operation in the _operations array

		if (DEBUG_MATRIX) {
			printf(" Ins: operation count:%d, current_operation: %p\n", operation_id_counter, current_operation); // Debug print
		}


		operation_count++;
	}

	return operation_list_head; // Return success

}

int generate_mode(const ProgramArguments* _arguments, Operation* _operation_list_to_return) {

	const char* output_filename = _arguments->filename; // Get the filename from arguments
	printf("Executing in generate mode with filename: %s\n", output_filename);

	char* matrix_filename = (char*) c_alloc(MAX_FILENAME_LENGTH, sizeof(char), "generate_mode", 0); // Allocate memory for filename
    strcpy(matrix_filename, output_filename);       // Copy from arguments the filename to save the matrices 
	

    Operation* operation_list_head = generate_matrix_test_data( /*arguments, operations, , operation_count, */
		_arguments->operation_count,
        _arguments->mat1_row_count, _arguments->mat1_col_count, 
        _arguments->mat2_row_count, _arguments->mat2_col_count);

	// used for testing purposes only, to generate a single operation
	//Operation* operation_list_head = generate_single_operation_test_data(_arguments->mat1_row_count, _arguments->mat1_col_count,
	//	_arguments->mat2_row_count, _arguments->mat2_col_count);

	assert(operation_list_head != nullptr); // Ensure operation list is not null

	// clear previous file for writing, creating it if it doesn't exist
	FILE* _dummy_file_variable = open_matrix_file(matrix_filename, "w");
	
	// Open the file for writing in append mode
	const char* file_open_option = "ab"; // File operation option for writing
	FILE* _file_to_write_to = open_matrix_file(matrix_filename, file_open_option);
	if (_file_to_write_to == nullptr) {
		f_ree(matrix_filename); // Free allocated memory for filename
		deallocate_all_operations(operation_list_head); // Deallocate all operations in the list
		return EXIT_FAILURE; // Exit with failure code
	}

	// Save the operation list to file
	int save_result = save_matrix_operations(operation_list_head, _file_to_write_to, matrix_filename);
	//(assert(save_result == EXIT_SUCCESS)); // Ensure save operation was successful

	f_ree(matrix_filename); // Free allocated memory for filename
	//deallocate_all_operations(operation_list_head); // Deallocate all operations in the list
	_operation_list_to_return = operation_list_head;

    printf("Completed executing generate mode with filename: %s\n", output_filename);

    // save operation list to file
    return EXIT_SUCCESS; // Placeholder for generate mode logic
}

//int load_mode(const char* output_filename, const ProgramArguments* _arguments) {
Operation* load_mode(const char* output_filename, const ProgramArguments * _arguments) {

	printf("Executing in loading mode with filename: %s\n", _arguments->filename);

	// Load matrix from file
	char* matrix_filename = (char*) c_alloc(MAX_FILENAME_LENGTH, sizeof(char), "load_mode", 0); // Allocate memory for filename
	strcpy(matrix_filename, _arguments->filename); // Copy from arguments the filename to load the matrices

	const char* file_open_option = "r"; // File operation option for writing
	FILE* file_to_read_from = open_matrix_file(matrix_filename, file_open_option);
	if (file_to_read_from == nullptr) {
		f_ree(matrix_filename); // Free allocated memory for filename
		//return EXIT_FAILURE; // Exit with failure code
		return nullptr;
	}

	Operation* operation_list_head = nullptr;
	operation_list_head = load_matrix_test_data(file_to_read_from, operation_list_head);
	//int result = load_matrix_test_data(file_to_read_from, operation_list_head);
	if (operation_list_head == nullptr) {
		fprintf(stderr, "Error loading operations from file\n");
		f_ree(matrix_filename); // Free allocated memory for filename
		fclose(file_to_read_from); // Close the file
		//return EXIT_FAILURE; // Exit with error code
		return nullptr;
	}

	// clean up
	fclose(file_to_read_from); // Close the file after reading
	f_ree(matrix_filename); // Free allocated memory for filename

	// Print loaded operations for debugging
	if (DEBUG_PERSISTENCE) {
		Operation* current_operation = operation_list_head;
		while (current_operation != nullptr) {
			operation_print_info(current_operation); // Print operation info
			current_operation = current_operation->next; // Move to the next operation
		}
	}

	// --- section commented out since we are returning the operations list -- 
	//report_leaks("After loading operations from file\n"); // Report memory leaks after loading operations
	//// Deallocate all operations in the list
	//deallocate_all_operations(operation_list_head); // Deallocate all operations in the list
	// finish cleaning up
	//report_leaks("After deallocation\n"); // Report memory leaks at the end of the program
	// -- end commented out section

	printf("Completed executing load mode with filename: %s\n", output_filename);
	//return EXIT_SUCCESS; // Placeholder for load mode logic

	return operation_list_head;
}

int execute_test(int argc, char* argv[]) {

	return EXIT_FAILURE; // Placeholder for test execution logic
}

int execute_program(int argc, char* argv[], Operation* _operation_list_to_return) {

    // declare parameter variables
    char matrix_filename[64] = { 0 }; // Buffer for filename
    int matrix1_row_count = 0;
    int matrix1_col_count = 0;
    int matrix2_row_count = 0;
    int matrix2_col_count = 0;

    int result = 0;
    int operation_count = 0;

    ProgramArguments* arguments = initialize_program_arguments();
	Operation* operation_list_to_return = nullptr;

    // Initialize program arguments
    arguments = initialize_program_arguments();
    if (arguments == nullptr) {
        fprintf(stderr, "Failed to initialize program arguments.\n");
        return EXIT_FAILURE;
    }

    result = parse_arguments(argc, argv, arguments);
    if (arguments->generate_matrix_flag == 1) {             // if the execution mode is generate
		result = generate_mode(arguments, _operation_list_to_return); // Generate matrix test data and save to file
		if (result == EXIT_FAILURE) {
			fprintf(stderr, "Error generating operations\n");
			return EXIT_FAILURE; // Exit with error code
		}
    }
	else if (arguments->load_file_flag == 1) {
		
		//result = load_mode(arguments->filename, arguments); // Load matrix from file and process operations
		//if (result == EXIT_FAILURE) {
		//	fprintf(stderr, "Error loading operations from file\n");
		//	return EXIT_FAILURE; // Exit with error code
		//}

		// Load matrix from file
		_operation_list_to_return = load_mode(arguments->filename, arguments); // Load matrix from file and process operations
		if (!_operation_list_to_return) {
			fprintf(stderr, "Error loading operations from file\n");
			return EXIT_FAILURE; // Exit with error code
		}
	}
	else {
		fprintf(stderr, "No valid command line arguments provided\n");
		return EXIT_FAILURE;
	}

    return result;

}


// this is the main program
int main(int argc, char* argv[]) {

	int result = EXIT_FAILURE;
	Operation* operation_list = nullptr;

    if (EXECUTE_TEST) {
        result = execute_test(argc, argv); // Execute test if flag is set
	}
    else {

		result = execute_program(argc, argv, operation_list); // Execute main program logic
		// here do what is required
		//report_leaks("Final exit in main\n"); // Report memory leaks at the end of the program
    }

	return EXIT_SUCCESS;

}