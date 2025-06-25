#include <iostream>

#include "Matrix.h"
#include "common.h"

#include "Persistence.h"

struct ProgramArguments {

    int     g_flag = 0;            // generate matrix test data file
	int     l_flag = 0;            // load and read matrix test data file
    int     n_flag = 0;
    int     operation_count = 0;   //default_operation to generate

	int     r1_flag = 0;           // matrix 1 row
    int     c1_flag = 0;           // matrix 1 column 

    int     mat1_row_count = 0;    // row count
	int     mat1_col_count = 0;    // column count

    int     r2_flag = 0;           // matrix 2 row
    int     c2_flag = 0;           // matrix 1 column

    int     mat2_row_count = 0;    // row count
    int     mat2_col_count = 0;    // column count

    int     f_flag = 0;            // Filename flag
    char*   filename = nullptr;    // Filename

};

ProgramArguments* create_program_arguments() {

    ProgramArguments* args = new ProgramArguments();
	char _filename[] = "matrix_data.txt"; // Default filename
    args->g_flag = 0;
    args->l_flag = 0;
    args->n_flag = 0;       
    args->operation_count = MAX_OPERATION_COUNT;   // default operation count
    args->r1_flag = 0;
    args->c1_flag = 0;
    args->r2_flag = 0;
    args->c2_flag = 0;
	args->mat1_row_count = 25;      // Default row count for matrix 1
	args->mat1_col_count = 20;      // Default column count for matrix 1
	args->mat2_row_count = 20;      // Default row count for matrix 2
	args->mat2_col_count = 40;      // Default column count for matrix 2
    args->f_flag = 0;
    args->filename = _filename;
    return args;
}

int parse_arguments(int argc, char* argv[], ProgramArguments* _arguments) {

    char*   filename = NULL;
    int     g_flag = 0, l_flag = 0, n_flag = 0, r1_flag = 0, c1_flag = 0, r2_flag = 0, c2_flag = 0, f_flag = 0; // local flag variables
   
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-g") == 0) {           // generate matrix test data file
			g_flag = 1;
			_arguments->g_flag = g_flag;
            printf("g flag set\n");
        }

		else if (strcmp(argv[i], "-l") == 0) {      // Load matrix from file
			l_flag = 1;
			_arguments->l_flag = l_flag;
            printf("l flag set\n");
        }

        else if (strcmp(argv[i], "-n") == 0) {      // Load matrix from file
            n_flag = 1;
            printf("n flag set\n");
            _arguments->r1_flag = r1_flag;
            _arguments->operation_count = atoi(argv[++i]); // Get the next argument as row count
            printf("Operation count set to: %d\n", _arguments->operation_count);
        }

		else if (strcmp(argv[i], "-r1") == 0) {     // Matrix 1 row count
            r1_flag = 1;
            printf("r1 flag set\n");
			_arguments->r1_flag = r1_flag;
			_arguments->mat1_row_count = atoi(argv[++i]); // Get the next argument as row count
			printf("Matrix 1 row count set to: %d\n", _arguments->mat1_row_count);
        }

        else if (strcmp(argv[i], "-c1") == 0) {                         // Matrix 1 column count
			c1_flag = 1;
            printf("c1 flag set\n");
			_arguments->c1_flag = c1_flag;
            _arguments->mat1_col_count = atoi(argv[++i]);               // Get the next argument as row count
            printf("Matrix 1 column count set to: %d\n", _arguments->mat1_col_count);
        }

		else if (strcmp(argv[i], "-r2") == 0) {                         // Matrix 2 row count
            r2_flag = 1;
            printf("r2 flag set\n");
			_arguments->r2_flag = r2_flag;
            _arguments->mat2_row_count = atoi(argv[++i]);               // Get the next argument as row count
            printf("Matrix 2 row count set to: %d\n", _arguments->mat2_row_count);
        }

        else if (strcmp(argv[i], "-c2") == 0) {                 // Matrix 2 column count
			c2_flag = 1;
            printf("c2 flag set\n");
			_arguments->c2_flag = c2_flag;
            _arguments->mat2_col_count = atoi(argv[++i]);               // Get the next argument as row count
            printf("Matrix 2 column count set to: %d\n", _arguments->mat2_col_count);
        }

        else if (strcmp(argv[i], "-f") == 0) {                  // Load matrix from file
            f_flag = 1;
            _arguments->f_flag = f_flag;
            printf("l flag set\n");
            if (i + 1 < argc) {                                 // Check if the next argument is a filename
                filename = argv[++i];                           // Get the next argument as filename
                printf("Filename going to be set from %s to: %s\n", _arguments->filename, filename);
                _arguments->filename = filename;
			}
			else {
                fprintf(stderr, "Error: No filename provided after -f flag\n");
                return -1; // Error if no filename is provided
            }
        }
        
        else if ( (argc == 2) && 
                    (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) ) { // Help command
            printf("Usage: %s [options]\n", argv[0]);
            printf("Options:\n");
            printf("  -g          Generate matrix test data file\n");
            printf("  -l          Load matrix from file\n");
            printf("  -r1 <num>   Set matrix 1 row count\n");
            printf("  -c1 <num>   Set matrix 1 column count\n");
            printf("  -r2 <num>   Set matrix 2 row count\n");
            printf("  -c2 <num>   Set matrix 2 column count\n");
            printf("  -f <file>   Specify filename for loading/saving matrices\n");
            printf("  -h, --help  Show this help message\n");
			return 0; // Exit after showing help
        }

        else {
            printf("Unknown command: %s\n", argv[i]);
        }
    }
    
    int mat_check = r1_flag + c1_flag + r2_flag + c2_flag;
	// sanity check for flags, either all or none of the matrix flags should be set, i.e. either == 0 or == 4
	if ( (mat_check != 0) && (mat_check != 4) ) {
        fprintf(stderr, "Error: Invalid matrix flags. Use -r1, -c1, -r2, -c2 to specify matrix dimensions.\n");
        return -1;
    }
    
    if ( ( l_flag == 1) && 
        ( (g_flag == 1 ) || (mat_check != 0) ) ) {
        fprintf(stderr, "Error: Read from file option (-l) cannot be used together with -g and/or parameterized matrix sizes\n");
        return -1;
	}

    // Validate filename was provided
    if ((f_flag == 1) && (filename == NULL)) {
        fprintf(stderr, "Error: Filename is required\n");
        return -1;
    }

	return 0;
}

int test_matrix_multiplication() {
    // Test matrix multiplication logic here
    // This is a placeholder for actual test code
    std::cout << "Testing matrix multiplication..." << std::endl;
    int mat1_row_count = 20;
    int mat1_col_count = 25;

    int mat2_row_count = 25;
    int mat2_col_count = 20;


    //int mat1_row_count = 2;
    //int mat1_col_count = 4;

    //int mat2_row_count = 4;
    //int mat2_col_count = 3;
    unsigned short matrix_id = 0;
    unsigned short operation_id = 0;

    // Create 2x3 matrix (initialized to zeros)
    Matrix* mat1 = create_matrix(matrix_id, operation_id, mat1_row_count, mat1_col_count, MATRIX_TYPE_OPERAND);
    if (!mat1) {
        std::cerr << "Matrix allocation failed!" << std::endl;
        return -1;
    }

    Matrix* mat2 = create_matrix(matrix_id, operation_id, mat2_row_count, mat2_col_count, MATRIX_TYPE_OPERAND);
    if (!mat2) {
        std::cerr << "Matrix allocation failed!" << std::endl;
        return -1;
    }

    Matrix* mat_result = create_matrix(matrix_id, operation_id, mat1_row_count, mat2_col_count, MATRIX_TYPE_RESULT);
    if (!mat_result) {
        std::cerr << "Matrix allocation failed!" << std::endl;
        return -1;
    }

    unsigned short result;

    result = init_matrix_operand1(mat1);
    result = init_matrix_operand2(mat2);

    // Perform matrix multiplication
    unsigned int mul_result = multiply_matrices(mat1, mat2, mat_result);

    // Read values
    std::cout << "Matrix Result:\n";
    matrix_print_info(mat_result);

    // Free memory
    free_matrix(mat1);
    free_matrix(mat2);
    free_matrix(mat_result);
    return 0;
}


int generate_matrix_test_data(ProgramArguments* _arguments, Operation* _operations) {

    int operation_count = 0;
    int matrix_id_counter = 0;

    // Generate matrix test data and save to file
    printf("Generating matrix test data...\n");
	printf("Generating %d operations with the following matrix sizes:\n", _arguments->operation_count);
    printf("Matrix 1: %d rows x %d columns\n", _arguments->mat1_row_count, _arguments->mat1_col_count);
    printf("Matrix 2: %d rows x %d columns\n", _arguments->mat2_row_count, _arguments->mat2_col_count);

    for (int operation_id_counter = 0; operation_id_counter < _arguments->operation_count; operation_id_counter++) {

		// Create matrix operand 1 
        Matrix* matrix_operand_1 = create_matrix(matrix_id_counter++, operation_id_counter, _arguments->mat1_row_count, _arguments->mat1_col_count, MATRIX_TYPE_OPERAND);
        if (!matrix_operand_1) {
            std::cerr << "Matrix allocation failed!" << std::endl;
            return -1;
        }
		init_matrix_operand1(matrix_operand_1);

        // Create matrix operand 2
        Matrix* matrix_operand_2 = create_matrix(matrix_id_counter++, operation_id_counter, _arguments->mat2_row_count, _arguments->mat2_col_count, MATRIX_TYPE_OPERAND);
        if (!matrix_operand_2) {
            std::cerr << "Matrix allocation failed!" << std::endl;
            return -1;
        }
		init_matrix_operand2(matrix_operand_2);

        // Create matrix result
        Matrix* matrix_result = create_matrix(matrix_id_counter++, operation_id_counter, _arguments->mat1_row_count, _arguments->mat2_col_count, MATRIX_TYPE_RESULT);
        if (!matrix_result) {
            std::cerr << "Matrix allocation failed!" << std::endl;
            return -1;
        }
        
		if (multiply_matrices(matrix_operand_1, matrix_operand_2, matrix_result) != MATX_OP_SUCCESS ) {
            std::cerr << "Matrix multiplication failed!" << std::endl;
            free_matrix(matrix_operand_1);
            free_matrix(matrix_operand_2);
            free_matrix(matrix_result);
            return -1;
        }

		// create operation
		Operation* current_operation = (Operation*)malloc(sizeof(Operation)); // Allocate memory for the operation
        if (!current_operation) {
            std::cerr << "Operation allocation failed!" << std::endl;
            free_matrix(matrix_operand_1);
            free_matrix(matrix_operand_2);
            free_matrix(matrix_result);
            return -1;
        }
 
        current_operation->operation_id = operation_id_counter; // Set operation ID
        current_operation->operand1 = matrix_operand_1; // Set operand 1
        current_operation->operand2 = matrix_operand_2; // Set operand 2
        current_operation->result = matrix_result; // Set result

        _operations[operation_count] = *current_operation; // Store the current operation in the _operations array

        operation_count++;
        
    }

    return operation_count; // Return success

}

int main(int argc, char* argv[]) {
   
    int result = 0;
    int operation_count = 0;

    ProgramArguments* arguments = create_program_arguments();
    result = parse_arguments(argc, argv, arguments);
	
    Operation* operations = (Operation*) malloc(MAX_OPERATION_COUNT * sizeof(Operation*));

	//int result = test_persistence(operations);
	
    if (arguments->g_flag == 1) {
        operation_count = generate_matrix_test_data(arguments, operations);

        char* matrix_filename = arguments->filename;
        //printf("Operation 3 result matrix value: %d\n", operations[3].result->uint_data);

        FILE* file = fopen(matrix_filename, "w");
        if (file == nullptr) {
            fprintf(stderr, "Error opening file %s for writing.\n", matrix_filename);
            return -1; // Exit with error code
        }
        else {
            fwrite("\n", sizeof(char), strlen("\n"), file);
            fclose(file);
            for (int i = 0; i < operation_count; i++) {

                if (i == 9) {
                    printf("Operation %d:\n", i);
                }
                operation_print_info(&operations[i]);
                int save_result = save_operation_to_file(matrix_filename, operations);
            }
        }
    }
    else if (arguments->l_flag == 1) {
        // Load matrix from file
        char* matrix_filename = arguments->filename;
        printf("Loading matrix from file: %s\n", matrix_filename);
        result = test_persistence(operations);
        if (result != 0) {
            fprintf(stderr, "Error loading operations from file.\n");
            return -1; // Exit with error code
        }
        
        for (int i = 0; i < operation_count; i++) {
            operation_print_info(&operations[i]);
        }
	}

	result = 0;

	//int result = test_matrix_multiplication();

    return result;
} 