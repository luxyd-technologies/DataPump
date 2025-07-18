#include <iostream>
#include <assert.h>

#include "Matrix.h"
#include "common.h"
#include "Persistence.h"
#include "MatrixAllocation.h"
#include "ProgramArguments.h"


ProgramArguments* initialize_program_arguments(void) {

    ProgramArguments* args = new ProgramArguments();
    char _filename[] = "matrix_data.txt"; // Default filename
    args->generate_matrix_flag = 0;
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
    int     generate_matrix_flag = 0, l_flag = 0, n_flag = 0, r1_flag = 0, c1_flag = 0, r2_flag = 0, c2_flag = 0, f_flag = 0; // local flag variables

    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-g") == 0) {           // generate matrix test data file
            generate_matrix_flag = 1;
            _arguments->generate_matrix_flag = generate_matrix_flag;
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

        else if ((argc == 2) &&
            (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)) { // Help command
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
    if ((mat_check != 0) && (mat_check != 4)) {
        fprintf(stderr, "Error: Invalid matrix flags. Use -r1, -c1, -r2, -c2 to specify matrix dimensions.\n");
        return -1;
    }

    if ((l_flag == 1) &&
        ((generate_matrix_flag == 1) || (mat_check != 0))) {
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

int test_initialize_program_arguments() {
    return EXIT_FAILURE;
}

int test_parse_arguments() {
	return EXIT_FAILURE;
}