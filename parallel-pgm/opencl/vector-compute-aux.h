//#define FOOBAR_N 100000
#define FOOBAR_N 10000
#define FOOBAR_X 0.2
/* function to apply */
float foobar(float in) {
    float tmp = 0;
    for (int i = 0; i < FOOBAR_N; ++i) {
        tmp += pow(FOOBAR_X, i);
    }
    return tmp;
}
