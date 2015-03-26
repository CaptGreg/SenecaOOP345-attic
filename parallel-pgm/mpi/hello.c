/*
 * "hello world" program
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>		/* MPI header file */

int main(int argc, char *argv[]) {

    int nprocs;
    int myid;

    /* initialize for MPI (must come before any other calls to MPI routines) */
    MPI_Init(&argc, &argv);

    /* get number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    /* get this process's number (ranges from 0 to nprocs - 1) */
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    /* print a greeting */
    printf("hello from process %d of %d\n", myid, nprocs);

    /* clean up for MPI (should come after all other calls to MPI routines) */
    MPI_Finalize();

    return EXIT_SUCCESS;
}
