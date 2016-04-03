// rrdtool has a good discussion on thread-safe functions.
// rrd = round robin database. a circular data structure

// rrd_is_thread_safe.h
`
#pragma GCC poison strtok asctime ctime gmtime localtime tmpnam strerror

#if 0
NOTES FOR RRD CONTRIBUTORS

Some precautions must be followed when developing RRD from now on:

    Only use thread-safe functions in library code. Many often used libc functions 
    aren't thread-safe. Take care in the following situations or when using the 
    following library functions:
        Direct calls to strerror() must be avoided: use rrd_strerror() instead, 
        it provides a per-thread error message.

        The getpw*, getgr*, gethost* function families (and some more get* 
        functions) are not thread-safe: use the *_r variants

        Time functions: asctime, ctime, gmtime, localtime: use *_r variants

        strtok: use strtok_r

        tmpnam: use tmpnam_r

        Many others (lookup documentation)

    A header file named rrd_is_thread_safe.h is provided that works with the 
    GNU C-preprocessor to "poison" some of the most common non-thread-safe 
    functions using the #pragma GCC poison directive. Just include this
    header in source files you want to keep thread-safe.

    Do not introduce global variables!

    If you really, really have to use a global variable you may add a new 
    field to the rrd_context structure and modify rrd_error.c, 
    rrd_thread_safe.c and rrd_non_thread_safe.c
    Do not use getopt or getopt_long in *_r (neither directly nor indirectly).

    getopt uses global variables and behaves badly in a multi-threaded 
    application when called concurrently. Instead provide a *_r function 
    taking all options as function parameters. You may provide argc and 
    **argv arguments for variable length argument lists. See rrd_update_r 
    as an example.

#endif

#include <cstring>    // strtok, strerr

  // char *strtok(char *str, const char *delim);
  // char *strtok_r(char *str, const char *delim, char **saveptr);
  // char *strerror(int errnum);

#include <cstdio>     // tmpnam
 // char *tmpnam(char *s);
   // Note: Avoid use of tmpnam(); use mkstemp(3) or tmpfile(3) instead.
   // The  tmpnam()  function  returns a pointer to a string that is a valid 
   // filename, and such that a file with this name did not exist at some 
   // point in time, so that naive programmers may think it a suitable name 
   // for a temporary file.  If the argument s is NULL, this name is generated 
   // in an internal static buffer and may be  overwritten  by  the next  call  
   // to  tmpnam().   If s is not NULL, the name is copied to the character 
   // array (of length at least L_tmpnam) pointed to by s and the value s is 
   // returned in case of success.

       // int mkstemp(char *template);
       // int mkostemp(char *template, int flags);
       // int mkstemps(char *template, int suffixlen);
       // int mkostemps(char *template, int suffixlen, int flags);

    // DESCRIPTION
       // The mkstemp() function generates a unique temporary filename from 
       // template, creates and opens the file, and returns an open file 
       // descriptor for the file.

       // The last six characters of template must be "XXXXXX" and these are 
       // replaced with a string that makes the filename unique.  Since it 
       // will be modified, template must not be a string constant, but should 
       // be declared as a character array.

       // The file is created with permissions 0600, that is, read plus 
       // write for owner only.  The returned file descriptor provides 
       // both read and write access to the file.  The file is opened 
       // with the open(2) O_EXCL flag, guaranteeing that the caller 
       // is the process that creates the file.

       // The  mkostemp() function is like mkstemp(), with the difference 
       // that the following bits—with the same meaning as for open(2)—may 
       // be specified in flags: O_APPEND, O_CLOEXEC, and O_SYNC.  Note 
       // that when creating the file, mkostemp() includes the values 
       // O_RDWR, O_CREAT, and O_EXCL in the flags argument given to 
       // open(2); including these values  in the flags argument given to
       // mkostemp() is unnecessary, and produces errors on some systems.

       // The  mkstemps() function is like mkstemp(), except that the 
       // string in template contains a suffix of suffixlen characters.  
       // Thus, template is of the form prefixXXXXXXsuffix, and the 
       // string XXXXXX is modified as for mkstemp().

       // The mkostemps() function is to mkstemps() as mkostemp() is to mkstemp().

    // RETURN VALUE
       // On success, these functions return the file descriptor of the temporary file.  On error, -1 is returned, and errno is set appropriately.

#include <ctime>      // asctime, ctime, gmtine, localtime, 

  // char *asctime(const struct tm *tm);
  // char *asctime_r(const struct tm *tm, char *buf);

  // char *ctime(const time_t *timep);
  // char *ctime_r(const time_t *timep, char *buf);

  // struct tm *gmtime(const time_t *timep);
  // struct tm *gmtime_r(const time_t *timep, struct tm *result);

  // struct tm *localtime(const time_t *timep);
  // struct tm *localtime_r(const time_t *timep, struct tm *result);

  // time_t mktime(struct tm *tm);

int main(int argc, char*argv[])
{
 char *tmp = tmpnam(NULL); 
}
