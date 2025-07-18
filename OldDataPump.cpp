#include <iostream>
#include <assert.h>

#include "Parameters.h"
#include "MemTrack.h" // Memory tracking header
#include "Matrix.h"
#include "common.h"

#include "Persistence.h"
#include "MatrixAllocation.h"
#include "ProgramArguments.h"



int test(int argc, char* argv[]) {
   
    int result = 0;
    int operation_count = 0;

    ProgramArguments* arguments = initialize_program_arguments();
    result = parse_arguments(argc, argv, arguments);
	
    Operation* operations = (Operation*)c_alloc(MAX_OPERATION_COUNT, sizeof(Operation*), "main() operations list", 0);  // debug memory leaks
	
    if (arguments->generate_matrix_flag == 1) {
        
        return EXIT_SUCCESS;
       
    }
    else if (arguments->l_flag == 1) {
        // Load matrix from file
        char* matrix_filename = arguments->filename;
        if (DEBUG_PERSISTENCE) {
            printf("Loading matrix from file: %s\n", matrix_filename);
        }

        result = test_persistence(matrix_filename, operations);
        if (result != 0) {
            fprintf(stderr, "Error loading operations from file.\n");
            return -1; // Exit with error code
        }
        
        for (int counter = 0; counter < operation_count; counter++) {
            operation_print_info(&operations[counter]);
            // clean up memory allocations
            deallocate_operation(&operations[counter]);
        }
	}
    else {
		return EXIT_FAILURE; // No valid operation mode specified
    }
    
	result = 0;

    report_leaks("Final point");
    return result;
} 
