#define AVAL 3.0
#define BVAL 5.0

void initialize(double *A, double *B, int dimN, int dimP, int dimM) {
    for (int i=0; i<dimN; ++i)
        for (int j=0; j<dimP; ++j)
            A[i*dimN+j] = AVAL+i+j;
    for (int i=0; i<dimP; ++i)
        for (int j=0; j<dimM; ++j)
            B[i*dimP+j] = BVAL+i+j;
}
