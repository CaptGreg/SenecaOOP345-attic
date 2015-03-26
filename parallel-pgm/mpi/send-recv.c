/*
 * nearly the simplest possible message-passing program:
 *
 * process 0 sends a message, and process 1 receives and prints it.
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
    int source;
    int dest;
    MPI_Status status;

    /* initialize for MPI */
    MPI_Init(&argc, &argv);

    /* get number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    if (nprocs != 2) 
        error_exit("number of processes must be 2\n");

    /* get this process's number (ranges from 0 to nprocs - 1) */
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (myid == 0) {
        /* in process 0:  send message to process 1 */
        buff[0] = 10;
        buff[1] = 20;
        dest = 1;
        MPI_Send(buff, BUFFLEN, MPI_INT, dest, MYTAG, MPI_COMM_WORLD);
        printf("process 0 sent %d %d\n", buff[0], buff[1]);
    }
    else {
        /* in process 1:  receive message from process 0 and print */
        source = 0;
        MPI_Recv(buff, BUFFLEN, MPI_INT, source, MYTAG, MPI_COMM_WORLD, 
                &status);
        printf("process 1 received %d %d\n", buff[0], buff[1]);
    }

    /* clean up for MPI */
    MPI_Finalize();

    return EXIT_SUCCESS;
}
