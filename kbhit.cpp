#include <termios.h>     // POSIX Terminal Definitions  tcgetattr, tcsetattr, ...
#include <sys/ioctl.h>   // ioctl          Definitions  
#include <sys/select.h>  // select(2)

class KBHit { // usage: KBHit kbhit;  if(kbhit()) {...}
public:
    KBHit()  { enable_raw_mode(); }

    ~KBHit() { disable_raw_mode(); }

    bool operator() (void) { return kbhitIoctl(); }

private:
    void enable_raw_mode() // https://stackoverflow.com/questions/29335758/using-kbhit-and-getch-on-linux
    {
        termios term;
        tcgetattr(0, &term);
        term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
        tcsetattr(0, TCSANOW, &term);
    }

    void disable_raw_mode() // https://stackoverflow.com/questions/29335758/using-kbhit-and-getch-on-linux
    {
        termios term;
        tcgetattr(0, &term);
        term.c_lflag |= ICANON | ECHO;
        tcsetattr(0, TCSANOW, &term);
        tcflush(0, TCIFLUSH); // Clear stdin to prevent characters appearing on prompt
    }

    bool kbhitIoctl() // https://stackoverflow.com/questions/29335758/using-kbhit-and-getch-on-linux
    {
        int byteswaiting;
        ioctl(0, FIONREAD, &byteswaiting);
        return byteswaiting > 0;
    }

    bool kbhitSelect() // select(2) method 
    {
      fd_set input;
      FD_ZERO(&input);
      FD_SET(0, &input); // FD 0

      struct timeval timeout;
      timeout.tv_sec  = 0; // no delay
      timeout.tv_usec = 0;

      int nfds = select(nfds = 1, &input, nullptr, nullptr, &timeout); // trashes timeout
      //                            output   ignore
      // Some pre-c++11 code which has nanosecond chrono code calls select() with all three sets empty, 
      // nfds zero, and a non-NULL timeout as a fairly portable way to sleep with subsecond precision.

      if(nfds == -1) {
        perror("kbhit select failed");
        return false;
      } else if (nfds == 0) {
        puts("kbhit TIMEOUT\n");
        return false;
      } else { // We have input
        return true;
        // if(FD_ISSET(0, &input)) // was it FD 0? YES, select on sole file descriptor
          // return true;
      }
      return false;
    }
};

