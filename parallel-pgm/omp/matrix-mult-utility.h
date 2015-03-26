/* compute C = C + (A * B) (matrix multiplication)
   A is dimN by dimP
   B is dimP by dimM
   C is dimN by dimM
   also, each matrix M has a "stride" strideM that gives the distance from
      M[i][0] to M[i+1][0]; this allows us to pass sections of a larger
      matrix to this routine.
 */
void matmul_add(double *A, double *B, double *C, int dimN, int dimP, int dimM,
        int strideA, int strideB, int strideC) {
    for (int i = 0; i < dimN; ++i) {
        for (int j = 0; j < dimM; ++j) {
            double tmp = C[i*strideC + j];
            for(int k = 0; k < dimP; ++k){
                /* C[i][j] = sum(over k) A[i][k] * B[k][j] */
                tmp += A[i*strideA + k] *  B[k*strideB + j];
            }
            C[i*strideC + j] = tmp;
        }
    }
}

/* clear A (set all elements to zero)
   A is dimN by dimM
   strideA is as described for matmul 
 */
void matclear(double *A, int dimN, int dimM, int strideA) {
    int i, j;
    for (i = 0; i < dimN; ++i) 
        for (j = 0; j < dimM; ++j) {
            /* A[i][j] = 0 */
            *(A + (i*strideA + j)) = 0.0;
        }
}
