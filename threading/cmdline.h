#ifndef CMDLINE_H__
#define CMDLINE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

long get_integer_arg_extended(int argc, char* argv[], int arg_index, 
        long min_val, const char* description, const char* usage_msg,
        bool print_flag, void (*cleanup) (void) );
double get_floating_arg_extended(int argc, char* argv[], int arg_index, 
        double min_val, const char* description, const char* usage_msg,
        bool print_flag, void (*cleanup) (void) );
long get_integer_environment_extended(const char* varname, long min_val, 
        const char* description, bool print_flag, void (*cleanup) (void) );

/* 
 * Get integer from command-line argument.
 * Prints error message(s) and exits program on error.
 * Parameter "usage_msg" should include a "%s" for the program name.
 */
long get_integer_arg(int argc, char* argv[], int arg_index, 
        long min_val, const char* description, const char* usage_msg)
{
    return get_integer_arg_extended(argc, argv, arg_index, 
        min_val, description, usage_msg, true, NULL);
}

/* 
 * Get floating-point number from command-line argument.
 * Prints error message(s) and exits program on error.
 * Parameter "usage_msg" should include a "%s" for the program name.
 */
double get_floating_arg(int argc, char* argv[], int arg_index, 
        double min_val, const char* description, const char* usage_msg)
{
    return get_floating_arg_extended(argc, argv, arg_index, 
        min_val, description, usage_msg, true, NULL);
}

/* 
 * Get integer from environment variable.
 * Prints error message(s) and exits program on error.
 */
long get_integer_environment(const char* varname, long min_val, 
        const char* description)
{
    return get_integer_environment_extended(varname, min_val, 
            description, true, NULL);
}

/* 
 * Version of get_integer_arg for MPI programs, allowing for printing
 * only in one process and execution of a cleanup function before exit.
 */
long get_integer_arg_extended(int argc, char* argv[], int arg_index, 
        long min_val, const char* description, const char* usage_msg,
        bool print_flag, void (*cleanup) (void) )
{
    if (arg_index >= argc) {
        if (print_flag)
            fprintf(stderr, usage_msg, argv[0]);
        if (cleanup != NULL) cleanup();
        exit(EXIT_FAILURE);
    }
    char* end;
    long rval = strtol(argv[arg_index], &end, 10);
    if (*end != '\0') {
        if (print_flag) {
            fprintf(stderr, "%s must be integer\n", description);
            fprintf(stderr, usage_msg, argv[0]);
        }
        if (cleanup != NULL) cleanup();
        exit(EXIT_FAILURE);
    }
    if (rval < min_val) {
        if (print_flag) {
            fprintf(stderr, "%s must be at least %ld\n", description, min_val);
            fprintf(stderr, usage_msg, argv[0]);
        }
        if (cleanup != NULL) cleanup();
        exit(EXIT_FAILURE);
    }
    return rval;
}

/* 
 * Version of get_floating_arg for MPI programs, allowing for printing
 * only in one process and execution of a cleanup function before exit.
 */
double get_floating_arg_extended(int argc, char* argv[], int arg_index, 
        double min_val, const char* description, const char* usage_msg,
        bool print_flag, void (*cleanup) (void) )
{
    if (arg_index >= argc) {
        if (print_flag)
            fprintf(stderr, usage_msg, argv[0]);
        if (cleanup != NULL) cleanup();
        exit(EXIT_FAILURE);
    }
    char* end;
    double rval = strtod(argv[arg_index], &end);
    if (*end != '\0') {
        if (print_flag) {
            fprintf(stderr, "%s must be numeric\n", description);
            fprintf(stderr, usage_msg, argv[0]);
        }
        if (cleanup != NULL) cleanup();
        exit(EXIT_FAILURE);
    }
    if (rval < min_val) {
        if (print_flag) {
            fprintf(stderr, "%s must be at least %f\n", description, min_val);
            fprintf(stderr, usage_msg, argv[0]);
        }
        if (cleanup != NULL) cleanup();
        exit(EXIT_FAILURE);
    }
    return rval;
}

/* 
 * Version of get_integer_environment for MPI programs, allowing for printing
 * only in one process and execution of a cleanup function before exit.
 */
long get_integer_environment_extended(const char* varname, long min_val, 
        const char* description, bool print_flag, void (*cleanup) (void) )
{
    char* svalue = getenv(varname);
    if (svalue == NULL) {
        if (print_flag)
            fprintf(stderr, "%s (%s) must be defined\n", varname, description);
        if (cleanup != NULL) cleanup();
        exit(EXIT_FAILURE);
    }
    char* end;
    long rval = strtol(svalue, &end, 10);
    if (*end != '\0') {
        if (print_flag) {
            fprintf(stderr, "%s (%s) must be integer\n", varname, description);
        }
        if (cleanup != NULL) cleanup();
        exit(EXIT_FAILURE);
    }
    if (rval < min_val) {
        if (print_flag) {
            fprintf(stderr, "%s (%s) must be at least %ld\n", varname, 
                    description, min_val);
        }
        if (cleanup != NULL) cleanup();
        exit(EXIT_FAILURE);
    }
    return rval;
}

#endif
