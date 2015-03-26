#include <stdio.h>

void print_values(FILE *outfile, float uk[], int nx) {
    for (int i = 0; i < nx+2; ++i) {
        fprintf(outfile, "uk[%010d] = %14.10f\n", i, uk[i]);
    }
}
