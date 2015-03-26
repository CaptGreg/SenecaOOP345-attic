/*
 * nearly the simplest possible program to time message-passing:
 *
 * process 0 sends a message, process 1 receives it and sends it
 *   back, process 0 receives the result and prints timing info.
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>		/* MPI header file */
#define MYTAG 0
#define BUFFLEN 2
#include "utility.h"    /* has error_exit() */

/* main program */
int main(int argc, char *argv[]) {

    int nprocs;
    int myid;
    int buff[BUFFLEN];
    int other_id;
    MPI_Status status;
    double start_time, end_time;

    /* initialize for MPI */
    MPI_Init(&argc, &argv);

    /* get number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    if (nprocs != 2) 
        error_exit("number of processes must be 2\n");

    /* get this process's number (ranges from 0 to nprocs - 1) */
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (myid == 0) {
        /* in process 0:  send message to process 1 and wait for echo */
        /* (we could put code here to put meaningful data in buff) */
        other_id = 1;
        start_time = MPI_Wtime();
        MPI_Send(buff, BUFFLEN, MPI_INT, other_id, MYTAG, MPI_COMM_WORLD);
        MPI_Recv(buff, BUFFLEN, MPI_INT, other_id, MYTAG, MPI_COMM_WORLD, 
                &status);
        end_time = MPI_Wtime();
        printf("starting, ending times %g %g\n", start_time, end_time);
        printf("difference %g\n", end_time - start_time);
    }
    else {
        /* in process 1:  receive message from process 0 and send back */
        other_id = 0;
        MPI_Recv(buff, BUFFLEN, MPI_INT, other_id, MYTAG, MPI_COMM_WORLD, 
                &status);
        MPI_Send(buff, BUFFLEN, MPI_INT, other_id, MYTAG, MPI_COMM_WORLD);
    }

    /* clean up for MPI */
    MPI_Finalize();

    return EXIT_SUCCESS;
}
