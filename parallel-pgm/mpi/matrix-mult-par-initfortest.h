#define AVAL 3.0
#define BVAL 5.0

void initialize(double *A, double *B, int dimNb, int dimPb, int dimMb,
        int NB, int myID_i, int myID_j) {
    for (int i=0; i<dimNb; ++i)
        for (int j=0; j<dimPb; ++j)
            A[i*dimNb+j] = AVAL+global(myID_i,dimNb,i)+global(myID_j,dimPb,j);
    for (int i=0; i<dimPb; ++i)
        for (int j=0; j<dimMb; ++j)
            B[i*dimPb+j] = BVAL+global(myID_i,dimNb,i)+global(myID_j,dimPb,j);
}
