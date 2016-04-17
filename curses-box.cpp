// The following program renders a "filled" rectangle by filling it from the outside borders and moving inward in a clockwise direction.

/* README README README README README
 *
 * Must be compiled with the "-lcurses" option 
 *
 */

#include <curses.h>
#include <string.h>
#include <thread>
#include <chrono>

#define DELAY 25000

int usleep(unsigned usec) // A DIY C++11 std::chrono platform independent usleep()
{
  // NAME
  //      usleep - suspend execution for microsecond intervals
  // SYNOPSIS
  //      #include <unistd.h>
  //      int usleep(useconds_t usec);
  // RETURN VALUE
  //      The usleep() function returns 0 on success.  On error, -1 is returned, with errno set to indicate the cause of the error.
  // ERRORS
  //      EINTR  Interrupted by a signal; see signal(7).
  //      EINVAL usec is not smaller than 1000000.  (On systems where that is considered an error.)
  // NOTES
  //     The  type  useconds_t  is  an  unsigned  integer  type capable of holding 
  //     integers in the range [0,1000000].  Programs will be more portable if 
  //     they never mention this type explicitly.  Use
  //        #include <unistd.h>
  //        ...
  //            unsigned int usecs;
  //        ...
  //            usleep(usecs);

  std::this_thread::sleep_for (std::chrono::microseconds(usec));  // void
  return 0;
}

/* The following program shows how to use the curses library */
int main (int argc, char *argv[]) {
    WINDOW *win;
    int row, col, k, n;
    int dr, dc;
    int width, height;
    char *arrows = ">V<^";

    win = initscr (); /* must call this before any other curses functions */
    getmaxyx (win, height, width);  /* get maximum screen dimension */
    width /= 2;
    height /= 2;
    row = 0;
    col = 0;
    dr = +1;
    dc = +1;
    n = 0;

    /* The following loop draws a "filled" rectangle, starting from the
     * outside borders, moving inward until it hits the "center"
     */
    do {
        /* move horizontally (right or left) */
        for (k = 0; k < width; k++, col += dc) {
            move (row, col);
            echochar (arrows[n]);
            // usleep (DELAY);
            std::this_thread::sleep_for(std::chrono::microseconds(DELAY));
        }
        n++; n %= 4;   /* cycle through the next arrowhead */
        width--;       /* in the next cycle the width is shorter */
        col -= dc;
        dc *= -1;      /* reverse direction right <=> left */
        row += dr;

        /* move vertically (down or up) */
        for (k = 0; k < height; k++, row += dr) {
            move (row, col);
            echochar (arrows[n]);
            // usleep (DELAY);
            std::this_thread::sleep_for(std::chrono::microseconds(DELAY));
        }
        n++; n %= 4;   /* cycle through the next arrowhead */
        height--;      /* in the next cycle the height is shorter */
        row -= dr;
        dr *= -1;      /* reverse direction down <=> up */
        col += dc;
        refresh();
    } while (width > 0 || height > 0);

    /* get the maximum dimension again */
    getmaxyx (win, height, width);
    move (3, 5);       /* place a text inside the box */
    addstr ("Press any key");

    getch ();  /* wait for user response */
    endwin (); /* must call this before we exit */
    return 0;
}
