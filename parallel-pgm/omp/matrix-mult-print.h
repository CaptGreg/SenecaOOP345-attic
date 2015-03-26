/* print A
   A is dimN by dimM
   strideA is as described for matmul 
 */
void printMatrix(FILE *stream, char *name, double *A, int dimN, int dimM, 
        int strideA) {
    for (int i = 0; i < dimN; ++i) 
        for (int j = 0; j < dimM; ++j) {
            /* print A[i][j] */
            fprintf(stream, "%s[%04d][%04d] = %g\n", name, i, j, 
                    A[i*strideA + j]);
        }
}
