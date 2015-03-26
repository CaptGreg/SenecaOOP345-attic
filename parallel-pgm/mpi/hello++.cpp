/*
 * "hello world" program, C++ version
 */
#include <iostream>
#include <cstdlib>
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
    std::cout << "hello from process " << myid << " of " << nprocs << std::endl;

    /* clean up for MPI (should come after all other calls to MPI routines) */
    MPI_Finalize();

    return EXIT_SUCCESS;
}
