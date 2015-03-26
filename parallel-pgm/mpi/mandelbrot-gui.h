/*
 * GUI-related functions for Mandelbrot program (C version).
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
/*
 * Initialize for graphical display. 
 * Width, height are dimensions of display, in pixels.
 */
int setup(int width, int height,
        Display **display, Window *win, GC *gc, 
        long *min_color, long *max_color) {

    /* Variables for graphical display */
    int x = 0, y = 0;                  /* window position */
    int border_width = 4;              /* border width in pixels */
    int disp_width, disp_height;       /* size of screen */
    int screen;                        /* which screen */

    char *window_name = "Mandelbrot Set", *disp_name = NULL;
    long valuemask = 0;
    XGCValues values;

    long white, black; /* white, black pixel values */

    XEvent report;

    /* Connect to Xserver */
    if ( (*display = XOpenDisplay (disp_name)) == NULL ) {
        fprintf(stderr, "Cannot connect to X server %s\n",
                XDisplayName(disp_name));
        return EXIT_FAILURE;
    }

    /* Initialize for graphical display  */
    screen = DefaultScreen (*display);
    disp_width = DisplayWidth (*display, screen);
    disp_height = DisplayHeight (*display, screen);
    *win = XCreateSimpleWindow (*display, RootWindow (*display, screen),
            x, y, width, height, border_width, 
            BlackPixel (*display, screen), 
            WhitePixel (*display, screen));
    XStoreName(*display, *win, window_name);
    *gc = XCreateGC (*display, *win, valuemask, &values); /* graphics context */
    white = WhitePixel (*display, screen);       /* color value for white */
    black = BlackPixel (*display, screen);       /* color value for black */
    XSetBackground (*display, *gc, white);
    XSetForeground (*display, *gc, black);
    XMapWindow (*display, *win);
    XSync(*display, False);

    /* Get min and max for range of color values -- assumed to 
     *   be defined by "white", "black" 
     */
    *min_color = (white > black) ? black : white;
    *max_color = (white > black) ? white : black;

    /* Wait for keyboard input before starting program */
    fprintf(stderr, 
            "Press any key (with focus in display) to start the program\n");
    fflush(stderr);

    /*  Choose which events we want to handle   */
    XSelectInput(*display, *win, KeyPressMask);

    /* Wait for event */
    XNextEvent(*display, &report);

    return EXIT_SUCCESS;
}

/*
 * Wait for user response before ending program.
 * Also allows user to discover coordinates of points.
 */
void interact(Display *display, Window *win, 
        int width, int height,
        double real_min, double real_max,
        double imag_min, double imag_max) {

    double scale_real, scale_imag; 
    XEvent report;
    Window root_return, child_return;
    int root_x_return, root_y_return;
    int win_x_return, win_y_return;
    unsigned int mask_return;

    fprintf(stderr, "\n\n");
    fprintf(stderr, 
            "Click on a point in the display to get its coordinates\n");
    fprintf(stderr, 
            "Press any key (with focus in display) to end the program\n");
    fflush(stderr);

    /*  Choose which events we want to handle   */
    XSelectInput(display, *win, KeyPressMask | ButtonPressMask);

    /* Compute scaling factors (for processing mouse clicks) */
    scale_real = (double) (real_max - real_min) / (double) width;
    scale_imag = (double) (imag_max - imag_min) / (double) height;

    /* Event loop */

    while (True) {

        XNextEvent(display, &report);

        switch ( report.type ) {

            case ButtonPress:

                XQueryPointer(display, *win, &root_return, &child_return,
                        &root_x_return, &root_y_return,
                        &win_x_return, &win_y_return,
                        &mask_return);
                fprintf(stderr, "coordinates = (%g, %g)\n",
                    real_min + ((double) win_x_return * scale_real),
                    imag_min + ((double) (height-1-win_y_return) * scale_imag));
                                        /* height-1-row so y axis displays
                                         * with larger values at top
                                         */
                fflush(stderr);
                break;

            case KeyPress:

                return;
        }
    }
}
