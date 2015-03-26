/*
 * "hello world" program, plus hostname and command-line arguments
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>		/* MPI header file */
#define HOSTNAMELENGTH 80

int main(int argc, char *argv[]) {

  int nprocs;
  int myid;
  char host[HOSTNAMELENGTH];
  int hostlength = HOSTNAMELENGTH;

  /* initialize for MPI (must come before any other calls to MPI routines) */
  MPI_Init(&argc, &argv);

  /* get number of processes */
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

  /* get this process's number (ranges from 0 to nprocs - 1) */
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  /* get hostname */
  MPI_Get_processor_name(host, &hostlength);

  /* print a greeting */
  printf("hello from process %d of %d on %s\n", myid, nprocs, host);

  /* show command-line arguments, if any */
  for (int i = 0; i < argc; ++i) 
    printf("in process %d, argument %d is %s\n", myid, i, argv[i]);

  /* clean up for MPI (should come after all other calls to MPI routines) */
  MPI_Finalize();

  return EXIT_SUCCESS;
}
