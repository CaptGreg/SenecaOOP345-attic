/**
 * Sequential program to test/time simple matrix-multiplication routine. 
 *
 * Command-line arguments:  
 *   size of matrices to multiply
 *   optional filename for output values (not printed if no filename is given)
 */
#include <stdio.h>
#include <stdlib.h>

#include "timer.h"
#include "cmdline.h"
#include "matrix-mult-utility.h"
#include "matrix-mult-initfortest.h"
#include "matrix-mult-print.h"

int main(int argc, char *argv[]) {

    double start_time, end_time;
    int N;                       /* input size */
    int dimN, dimP, dimM;           /* matrix dimensions */
    double *A, *B, *C;              /* matrices */
    FILE *outfile = NULL;

    /* command-line arguments */
    char *usage_msg = "usage is %s size [outfile]\n";
    N = get_integer_arg(argc, argv, 1, 1, "size", usage_msg);
    if (argc > 2) {
        char *outfilename = argv[2];
        outfile = fopen(outfilename, "w");
        if (outfile == NULL) {
            fprintf(stderr, "Cannot open outfile %s\n", outfilename);
            fprintf(stderr, usage_msg, argv[0]);
            exit(EXIT_FAILURE);
        }
    } 

    dimN = dimP = dimM = N;

    A = malloc(dimN*dimP*sizeof(*A));
    B = malloc(dimP*dimM*sizeof(*B));
    C = malloc(dimN*dimM*sizeof(*C));

    if ((A == NULL) || (B == NULL) || (C == NULL)) {
        fprintf(stderr, "unable to allocate space for matrices of size %d\n",
                dimN);
        return EXIT_FAILURE;
    }

    /* Initialize matrices */

    initialize(A, B, dimN, dimP, dimM);

    /* Do the multiply */

    start_time = get_time();
    matclear(C, dimN, dimM, dimM);
    matmul_add(A, B, C, dimN, dimP, dimM, dimP, dimM, dimM);
    end_time = get_time();

    /* Print results */

    if (outfile != NULL) {
        printMatrix(outfile, "A", A, dimN, dimP, dimP);
        printMatrix(outfile, "B", B, dimP, dimM, dimM);
        printMatrix(outfile, "A*B", C, dimN, dimM, dimM);
        fclose(outfile);
    }

    fprintf(stdout, "Simple program, sequential\n");
    fprintf(stdout, "Size = %d, time for multiplication = %g\n",
            N, end_time - start_time);

    return EXIT_SUCCESS;
}
