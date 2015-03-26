#include <string.h>
#define OUTFILENAMEDIGITS 4
#define OUTFILENAMEFMT "%s.%04d" /* must be consistent with OUTFILENAMEDIGITS */

FILE* makeOutfile(char* basename, int myID) {
    char name[strlen(basename)+1+OUTFILENAMEDIGITS+1];
    snprintf(name, sizeof(name), OUTFILENAMEFMT, basename, myID);
    FILE *outfile = fopen(name, "w");
    if (outfile == NULL) {
        fprintf(stderr, "Cannot open outfile %s\n", name);
    }
    return outfile;
}

/* print A
   A is dimN by dimM (FIX THIS)
   strideA is as described for matmul 
 */
void printMatrix(FILE *stream, char *name, double *A, int dimNb, int dimMb, 
        int strideA, int myID_i, int myID_j) {
    for (int i = 0; i < dimNb; ++i) 
        for (int j = 0; j < dimMb; ++j) {
            /* print A[i][j] */
            fprintf(stream, "%s[%04d][%04d] = %g\n", name, 
                    global(myID_i,dimNb,i), global(myID_j,dimMb,j), 
                    A[i*strideA + j]);
        }
}
