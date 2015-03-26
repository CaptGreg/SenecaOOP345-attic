/**
 * Parallel program to test/time block-based matrix-multiplication routine
 * (distributed-memory version). 
 *
 * Command-line arguments:  
 *   size of matrices to multiply
 *   number of blocks (in each dimension -- e.g., 2 means split matrices 
 *     into 4 blocks each -- and number of blocks must match number of
 *     processes)
 *   optional base filename for output values (not printed if not
 *     given); filenames combine this value with process number
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define blockstart(M,i,j,rows_per_blk,cols_per_blk,stride) \
    (M + ((i)*(rows_per_blk))*(stride) + (j)*(cols_per_blk))

#include "timer.h"
#include "cmdline.h"
#include "utility.h"
#include "matrix-mult-par-utility.h"
#include "matrix-mult-par-initfortest.h"
#include "matrix-mult-par-print.h"

int main(int argc, char *argv[]) {

    double start_time, end_time;
    int N = 0;                      /* input size */
    int NB = 0;                     /* number of blocks */
    int dimN, dimP, dimM;           /* matrix dimensions */
    double *A, *B, *C;              /* matrices */
    int dimNb, dimPb, dimMb;        /* block dimensions */
    int myID, nprocs;
    int myID_i, myID_j;             /* block indices */
    double *Abuffer, *Bbuffer;      /* for blocks of A, B from other procs */
    MPI_Status status;
    FILE* outfile = NULL;

    /* MPI initialization */
    MPI_Init(&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myID);

    /* command-line arguments */
    char *usage_msg = "usage is %s size numblocks\n";
    N = get_integer_arg_extended(argc, argv, 1, 1,
            "size", usage_msg, true, 
            (void (*)(void))MPI_Finalize);
    NB = get_integer_arg_extended(argc, argv, 2, 1,
            "numblocks", usage_msg, true, 
            (void (*)(void))MPI_Finalize);
    if ((N % NB) != 0) {
        error_exit("numblocks must evenly divide size\n");
    }
    if ((NB*NB) != nprocs) {
        error_exit("numblocks squared must match number of processes\n");
    }
    if (argc > 3) {
        outfile = makeOutfile(argv[3], myID);
        if (outfile == NULL) {
            fprintf(stderr, usage_msg, argv[0]);
            error_exit(NULL);
        }
    } 

    dimN = dimP = dimM = N;
    dimNb = dimN / NB;
    dimPb = dimP / NB;
    dimMb = dimM / NB;
    myID_i = myID / NB;
    myID_j = myID % NB;

    A = malloc(dimNb*dimPb*sizeof(*A));
    B = malloc(dimPb*dimMb*sizeof(*B));
    C = malloc(dimNb*dimMb*sizeof(*C));
    Abuffer = malloc(dimNb*dimPb*sizeof(*Abuffer));
    Bbuffer = malloc(dimPb*dimMb*sizeof(*Bbuffer));

    if ((A == NULL) || (B == NULL) || (C == NULL) || 
            (Abuffer == NULL) || (Bbuffer == NULL)) {
        fprintf(stderr, "unable to allocate space for matrices of size %d\n",
                dimN);
        return EXIT_FAILURE;
    }

    /* Initialize matrices */

    initialize(A, B, dimNb, dimPb, dimMb, NB, myID_i, myID_j);
	
    /* Do the multiply */

    /* could use MPI_Wtime but get_time is what sequential code uses */
    start_time = get_time();

    matclear(C, dimNb, dimMb, dimMb);

    for (int kb=0; kb < NB; ++kb) {

        if (myID_j == kb) {
            /* send A to other processes in the same "row" */
            for (int jb=0; jb < NB; ++jb) {
                if (jb != myID_j)
                    MPI_Send(A, dimNb*dimPb, MPI_DOUBLE, myID_i*NB + jb, 0, 
                            MPI_COMM_WORLD);
            }
            /* copy A to Abuffer */
            memcpy(Abuffer, A, dimNb*dimPb*sizeof(*A));
        }
        else {
            /* receive block of A */
            MPI_Recv(Abuffer, dimNb*dimPb, MPI_DOUBLE, myID_i*NB + kb, 0,
                    MPI_COMM_WORLD, &status);
        }

        if (myID_i == kb) {
            /* send B to other processes in the same "column" */
            for (int ib=0; ib < NB; ++ib) {
                if (ib != myID_i)
                    MPI_Send(B, dimPb*dimMb, MPI_DOUBLE, ib*NB + myID_j, 0, 
                            MPI_COMM_WORLD);
            }
            /* copy B to Bbuffer */
            memcpy(Bbuffer, B, dimPb*dimMb*sizeof(*A));
        }
        else {
            /* receive block of A */
            MPI_Recv(Bbuffer, dimPb*dimMb, MPI_DOUBLE, kb*NB + myID_j, 0,
                    MPI_COMM_WORLD, &status);
        }

        /* compute product of block[ib][kb] of A and block[kb][jb] of B 
           and add to block[ib][jb] of C */
        matmul_add(
                Abuffer, Bbuffer, C,
                dimNb, dimPb, dimMb, dimPb, dimMb, dimMb
                );
    }

    end_time = get_time();

    /* Print results */

    if (outfile != NULL) {
        printMatrix(outfile, "A", A, dimNb, dimPb, dimPb, myID_i, myID_j);
        printMatrix(outfile, "B", B, dimPb, dimMb, dimMb, myID_i, myID_j);
        printMatrix(outfile, "A*B", C, dimNb, dimMb, dimMb, myID_i, myID_j);
        fclose(outfile);
    }

    /* Print timing results */

    if (myID == 0) {
    fprintf(stdout, "Block-based program, parallel with %d processes\n",
            nprocs);
    fprintf(stdout, "Size = %d, numblocks = %d, time for multiplication = %g\n",
            N, NB, end_time - start_time);
    }

    /* MPI cleanup */
    MPI_Finalize();

    return EXIT_SUCCESS;
}
