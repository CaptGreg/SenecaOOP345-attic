/*
 * OpenMP Mandelbrot program using loop-splitting and critical section
 * that plots one row at a time.
 * 
 * This program computes and displays all or part of the Mandelbrot 
 * set.  By default, it examines all points in the complex plane
 * that have both real and imaginary parts between -2 and 2.  
 * Command-line parameters allow zooming in on a specific part of
 * this range.
 * 
 * Usage:
 *   mandelbrot maxiter [x0 y0 size]
 * where 
 *   maxiter denotes the maximum number of iterations at each point
 *   x0, y0, and size specify the range to examine (a square 
 *     centered at x0 + iy0 of size 2*size by 2*size -- by default, 
 *     a square of size 4 by 4 centered at the origin)
 * 
 * Input:  none, except the optional command-line arguments
 * Output: a graphical display as described in Wilkinson & Allen,
 *   displayed using the X Window system, plus text output to
 *   standard output showing the above parameters, plus execution
 *   time in seconds.
 * 
 * 
 * Code originally code obtained from Web site for Wilkinson and Allen's
 * text on parallel programming:
 * http://www.cs.uncc.edu/~abw/parallel/par_prog/
 * 
 * Reformatted and revised by B. Massingill.
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <float.h>

#include "timer.h"
#include "cmdline.h"

/* Default values for things. */
#define N           2           /* size of problem space (x, y from -N to N) */
#define NPIXELS     800         /* size of display window in pixels */

/* Structure definition for complex numbers */
typedef struct {
    double real, imag;
} complex;

/* Functions for GUI */
#include "mandelbrot-gui.h"     /* has setup(), interact() */
  
/* ---- Main program ---- */

int main(int argc, char *argv[]) {

    int maxiter;
    double real_min = -N;
    double real_max = N;
    double imag_min = -N;
    double imag_max = N;
    int width = NPIXELS;         /* dimensions of display window */
    int height = NPIXELS;
    Display *display;
    Window win;
    GC gc;
    long min_color = 0, max_color = 0;
    double scale_real, scale_imag, scale_color;
    double start_time, end_time;
    int setup_return;
    int nthreads;

    /* Process command-line arguments */
    char *usage_msg = "usage:  %s maxiter [x0 y0 size]\n";
    maxiter = get_integer_arg(argc, argv, 1, 1, "maxiter", usage_msg);
    if (argc > 2) {
        double x0 = get_floating_arg(argc, argv, 2, -DBL_MAX, "x0", usage_msg);
        double y0 = get_floating_arg(argc, argv, 3, -DBL_MAX, "y0", usage_msg);
        double size = get_floating_arg(argc, argv, 4, 0, "size", usage_msg);
        real_min = x0 - size;
        real_max = x0 + size;
        imag_min = y0 - size;
        imag_max = y0 + size;
    }

    /* Initialize for graphical display */
    setup_return = 
        setup(width, height, &display, &win, &gc, &min_color, &max_color);
    if (setup_return != EXIT_SUCCESS) {
        fprintf(stderr, "Unable to initialize display, continuing\n");
    }
    /* (if not successful, continue but don't display results) */

    /* Start timing */
    /* could use omp_get_wtime but get_time is what sequential code uses */
    start_time = get_time();

    /* Calculate and draw points */

    /* Compute factors to scale computational region to window */
    scale_real = (double) (real_max - real_min) / (double) width;
    scale_imag = (double) (imag_max - imag_min) / (double) height;

    /* Compute factor for color scaling */
    scale_color = (double) (max_color - min_color) / (double) (maxiter - 1);

    /* Calculate points and display */

    #pragma omp parallel 
    {
        long *colors = (malloc) (width * sizeof(*colors));

        #pragma omp single
        {
            nthreads = omp_get_num_threads();
        }
        /* comment out / in as appropriate */
        /* #pragma omp for schedule(dynamic) */
        /* #pragma omp for schedule(static,1) */
        #pragma omp for schedule(runtime)
        for (int row = 0; row < height; ++row)  {

            for (int col = 0; col < width; ++col) {

                complex z, c;

                z.real = z.imag = 0;

                /* Scale display coordinates to actual region  */
                c.real = real_min + ((double) col * scale_real);
                c.imag = imag_min + ((double) (height-1-row) * scale_imag);
                                        /* height-1-row so y axis displays
                                         * with larger values at top
                                         */

                /* Calculate z0, z1, .... until divergence or maximum iterations */
                int k = 0;
                double lengthsq, temp;
                do  {
                    temp = z.real*z.real - z.imag*z.imag + c.real;
                    z.imag = 2*z.real*z.imag + c.imag;
                    z.real = temp;
                    lengthsq = z.real*z.real + z.imag*z.imag;
                    ++k;
                } while (lengthsq < (N*N) && k < maxiter);

                /* Scale color and save */
                colors[col] = (long) ((k-1) * scale_color) + min_color;
            }
            /* Display row */
            #pragma omp critical 
            {
                for (int col = 0; col < width; ++col) {
                    if (setup_return == EXIT_SUCCESS) {
                        XSetForeground (display, gc, colors[col]);
                        XDrawPoint (display, win, gc, col, row);
                    }
                }
            }
        }
        free (colors);
    }

    /* Be sure all output is written */
    if (setup_return == EXIT_SUCCESS) {
        XFlush (display);
    }

    /* End timing  */
    end_time = get_time();

    /* Produce text output  */
    fprintf(stdout, "\n");
    fprintf(stdout, "OpenMP program (by rows)\n");
    fprintf(stdout, "number of threads = %d\n", nthreads);
    fprintf(stdout, "center = (%g, %g), size = %g\n",
            (real_max + real_min)/2, (imag_max + imag_min)/2,
            (real_max - real_min)/2);
    fprintf(stdout, "maximum iterations = %d\n", maxiter);
    fprintf(stdout, "execution time in seconds = %g\n", end_time - start_time);
    fprintf(stdout, "\n");

    /* Wait for user response, then exit program */
    if (setup_return == EXIT_SUCCESS) {
        interact(display, &win, width, height,
                real_min, real_max, imag_min, imag_max);
    }
    return EXIT_SUCCESS;
}
