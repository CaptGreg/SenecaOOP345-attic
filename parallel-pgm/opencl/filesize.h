#ifndef FILESIZE_H__
#define FILESIZE_H__

#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * Get size of file.
 * Uses Linux library function stat, so may not be portable.
 * Returns size of file *filename in bytes, or -1 on error 
 */
long filesize(const char * filename) {
    struct stat status;
    if (stat(filename, &status) == -1) {
        return -1;
    }
    else {
        return (long) status.st_size;
    }
}
#endif
