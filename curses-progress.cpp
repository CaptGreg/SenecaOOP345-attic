// The following C program renders a progress bar while animating the tip of the bar to spin prior to move a step further.

/* README README README README README
 *
 * Must be compiled with the "-lcurses" option 
 *
 */

#include <curses.h>
#include <cstring>
#include <thread>
#include <chrono>

#define WIDTH 40
#define DELAY 50000

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

void anim (int, int);
/* The following program shows how to use the curses library */
int main (int argc, char *argv[]) {
    WINDOW *win;
    int col;

    win = initscr (); /* must call this before any other curses functions */
    for (col = 0; col < WIDTH; col ++) {
        anim (1, col);
        /* move the cursor and print */
        mvprintw (1, WIDTH + 2, "%6.2f%%", 100.0 * (col+1) / WIDTH);
        refresh();
    }
    /* another way to do "mvprintw" */
    move (3, 5);
    addstr ("Press any key");

    getch ();  /* wait for user response */
    endwin (); /* must call this before we exit */
    return 0;
}

void anim (int r, int c)
{
    static char* symbols = "-\\|/-\\|/-=#";
    int k;
    for (k = 0; k < strlen(symbols); k++)
    {
        move (r,c);
        echochar (symbols[k]);
        // usleep (DELAY);
        std::this_thread::sleep_for(std::chrono::microseconds(DELAY));
    }
}
