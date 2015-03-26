/**
 * Sequential program to test/time block-based matrix-multiplication routine. 
 *
 * Command-line arguments:  
 *   size of matrices to multiply
 *   number of blocks (in each dimension -- e.g., 2 means split matrices 
 *     into 4 blocks each)
 *   optional filename for output values (not printed if no filename is given)
 */
#include <stdio.h>
#include <stdlib.h>

#define blockstart(M,i,j,rows_per_blk,cols_per_blk,stride) \
    (M + ((i)*(rows_per_blk))*(stride) + (j)*(cols_per_blk))

#include "timer.h"
#include "cmdline.h"
#include "matrix-mult-utility.h"
#include "matrix-mult-initfortest.h"
#include "matrix-mult-print.h"

int main(int argc, char *argv[]) {

    double start_time, end_time;
    int N = 0;                      /* input size */
    int NB = 0;                     /* number of blocks */
    int dimN, dimP, dimM;           /* matrix dimensions */
    double *A, *B, *C;              /* matrices */
    int dimNb, dimPb, dimMb;        /* block dimensions */
    FILE *outfile = NULL;

    /* command-line arguments */
    char *usage_msg = "usage is %s size numblocks\n";
    N = get_integer_arg(argc, argv, 1, 1, "size", usage_msg);
    NB = get_integer_arg(argc, argv, 2, 1, "numblocks", usage_msg);
    if ((N % NB) != 0) {
        fprintf(stderr, "numblocks must evenly divide size\n");
        return EXIT_FAILURE;
    }
    if (argc > 3) {
        char *outfilename = argv[3];
        outfile = fopen(outfilename, "w");
        if (outfile == NULL) {
            fprintf(stderr, "Cannot open outfile %s\n", outfilename);
            fprintf(stderr, usage_msg, argv[0]);
            exit(EXIT_FAILURE);
        }
    } 

    dimN = dimP = dimM = N;
    dimNb = dimN / NB;
    dimPb = dimP / NB;
    dimMb = dimM / NB;

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

    for (int ib=0; ib < NB; ++ib) {
        for (int jb=0; jb < NB; ++jb) {
            /* find block[ib][jb] of C */
            double * blockPtr = blockstart(C, ib, jb, dimNb, dimMb, dimM);
            /* clear block[ib][jb] of C (set all elements to zero) */
            matclear(blockPtr, dimNb, dimMb, dimM);
            for (int kb=0; kb < NB; ++kb) {
                /* compute product of block[ib][kb] of A and block[kb][jb] of B 
                   and add to block[ib][jb] of C */
                matmul_add(
                        blockstart(A, ib, kb, dimNb, dimPb, dimP),
                        blockstart(B, kb, jb, dimPb, dimMb, dimM),
                        blockPtr, dimNb, dimPb, dimMb, dimP, dimM, dimM
                        );
            }
        }
    }

    end_time = get_time();

    /* Print results */

    if (outfile != NULL) {
        printMatrix(outfile, "A", A, dimN, dimP, dimP);
        printMatrix(outfile, "B", B, dimP, dimM, dimM);
        printMatrix(outfile, "A*B", C, dimN, dimM, dimM);
        fclose(outfile);
    }

    fprintf(stdout, "Block-based program, sequential\n");
    fprintf(stdout, "Size = %d, numblocks = %d, time for multiplication = %g\n",
            N, NB, end_time - start_time);

    return EXIT_SUCCESS;
}
