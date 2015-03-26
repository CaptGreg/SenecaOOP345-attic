/* ugly nonportable hack to make millisleep() function compile with c99 */
#ifndef __USE_POSIX199309
#error need #define '#define _POSIX_C_SOURCE 199309L' in main program
#endif

#include <stdlib.h>
#include <time.h>

/* function to generate a random integer in [start, end] */
int random_in_range(int start, int end) {
    /* use method described in "rand" man page */
    return start + ((double) (end-start+1) * rand()/(RAND_MAX + 1.0));
}

/* function to sleep N millisecs -- not very portable */
void millisleep(int milliseconds) {
    struct timespec sleep_time = { 0, (long) milliseconds * 1000000 };
    nanosleep(&sleep_time, NULL);
}

