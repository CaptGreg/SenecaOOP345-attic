/*
 * utility functions for use in MPI examples
 */
/* msg is message to display, or NULL to just exit */
void error_exit(char* msg) {
    if (msg != NULL) {
        fprintf(stderr, "%s", msg);
    }
    MPI_Finalize();
    exit(EXIT_FAILURE);
}
