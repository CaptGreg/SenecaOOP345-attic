// some code inspired by www.cs.bgu.ac.il/~sadetsky/openu/Signals.ppt

#include <iostream>
#include <chrono>
#include <csignal>    // signals: SIGXXXX
using namespace std;
using namespace std::chrono;

// POSIX calls
#include <unistd.h>   // signal, alarm, ualarm, sleep, ...
#include <sys/time.h> // setitimer

// NAME
//        alarm - set an alarm clock for delivery of a signal
// 
// SYNOPSIS
//        #include <unistd.h>
// 
//        unsigned int alarm(unsigned int seconds);
// 
// DESCRIPTION
//        alarm() arranges for a SIGALRM signal to be delivered to the calling process in seconds seconds.
// 
//        If seconds is zero, any pending alarm is canceled.
// 
//        In any event any previously set alarm() is canceled.
// 
// RETURN VALUE
//        alarm() returns the number of seconds remaining until any previously scheduled alarm was due to be delivered, or zero if there was no previously sched‐
//        uled alarm.

// NAME
//        ualarm - schedule signal after given number of microseconds
// 
// SYNOPSIS
//        #include <unistd.h>
// 
//        useconds_t ualarm(useconds_t usecs, useconds_t interval);
// 
// DESCRIPTION
//        The  ualarm() function causes the signal SIGALRM to be sent to the invoking process after (not less than) usecs microseconds.  The delay may be length‐
//        ened slightly by any system activity or by the time spent processing the call or by the granularity of system timers.
// 
//        Unless caught or ignored, the SIGALRM signal will terminate the process.
// 
//        If the interval argument is nonzero, further SIGALRM signals will be sent every interval microseconds after the first.
// 
// RETURN VALUE
//        This function returns the number of microseconds remaining for any alarm that was previously set, or 0 if no alarm was pending.
// 
// ERRORS
//        EINTR  Interrupted by a signal.
// 
//        EINVAL usecs or interval is not smaller than 1000000.  (On systems where that is considered an error.)
// 
// NOTES
//        This function is obsolete.  Use setitimer(2) or POSIX interval timers (timer_create(2), etc.)  instead.

// NAME
//        getitimer, setitimer - get or set value of an interval timer
// 
// SYNOPSIS
//        #include <sys/time.h>
// 
//        int getitimer(int which, struct itimerval *curr_value);
//        int setitimer(int which, const struct itimerval *new_value,
//                      struct itimerval *old_value);
// 
// DESCRIPTION
//        The  system provides each process with three interval timers, each decrementing in a distinct time domain.  When any timer expires, a signal is sent to
//        the process, and the timer (potentially) restarts.
// 
//        ITIMER_REAL    decrements in real time, and delivers SIGALRM upon expiration.
// 
//        ITIMER_VIRTUAL decrements only when the process is executing, and delivers SIGVTALRM upon expiration.
// 
//        ITIMER_PROF    decrements both when the process executes and when the system is executing on behalf of the process.  Coupled with ITIMER_VIRTUAL,  this
//                       timer is usually used to profile the time spent by the application in user and kernel space.  SIGPROF is delivered upon expiration.
// 
//        Timer values are defined by the following structures:
// 
//            struct itimerval {
//                struct timeval it_interval; /* next value */
//                struct timeval it_value;    /* current value */
//            };
// 
//            struct timeval {
//                time_t      tv_sec;         /* seconds */
//                suseconds_t tv_usec;        /* microseconds */
//            };
// 
//        The  function  getitimer()  fills the structure pointed to by curr_value with the current setting for the timer specified by which (one of ITIMER_REAL,
//        ITIMER_VIRTUAL, or ITIMER_PROF).  The element it_value is set to the amount of time remaining on the timer, or zero if the timer  is  disabled.   Simi‐
//        larly, it_interval is set to the reset value.
// 
//        The function setitimer() sets the specified timer to the value in new_value.  If old_value is non-NULL, the old value of the timer is stored there.
// 
//        Timers  decrement  from  it_value  to  zero,  generate a signal, and reset to it_interval.  A timer which is set to zero (it_value is zero or the timer
//        expires and it_interval is zero) stops.
// 
//        Both tv_sec and tv_usec are significant in determining the duration of a timer.
// 
//        Timers will never expire before the requested time, but may expire some (short) time afterward, which depends on the system timer resolution and on the
//        system  load;  see  time(7).   (But  see BUGS below.)  Upon expiration, a signal will be generated and the timer reset.  If the timer expires while the
//        process is active (always true for ITIMER_VIRTUAL) the signal will be delivered immediately when generated.  Otherwise the delivery will be offset by a
//        small time dependent on the system loading.
// 
// RETURN VALUE
//        On success, zero is returned.  On error, -1 is returned, and errno is set appropriately.

void signal_handler(int sig)
{
  switch(sig) {
  case SIGALRM:   // a real-time timer that counts clock time (alarm, ualarm, setitimer)
  case SIGVTALRM: // a virtual timer that counts CPU time used by the process (setitimer)
  case SIGPROF:   // a profiling timer that counts both CPU time used by the process, and CPU time spent in system calls on behalf of the process (setitimer )
    alarm(1); // re-issue alarm
    signal(SIGALRM, signal_handler);
    break;
  }
}


void timers()
{
    signal(SIGALRM, SIG_IGN);  // ignore a signal
    signal(SIGALRM, SIG_DFL);  // restore signal default action

    signal(SIGALRM, signal_handler);
    signal(SIGVTALRM, signal_handler);
    signal(SIGPROF, signal_handler);

    alarm(1);

    ualarm(1000000, 3000000); // <unistd>

    int which = ITIMER_REAL;
    struct itimerval value;
    struct itimerval old_value;
    int ret = setitimer ( which, &value, &old_value);

    struct itimerval itv;
    itv.it_interval.tv_sec = 2;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = 4;
    itv.it_value.tv_usec = 0;
    setitimer (ITIMER_VIRTUAL, &itv, NULL);
}

int main(int argc, char* argv[]) 
{

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    high_resolution_clock::time_point t1a             = high_resolution_clock::now(); // using namespace std::chrono;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // do stuff
    sleep(1);
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    high_resolution_clock::time_point t2a             = high_resolution_clock::now(); // using namespace std::chrono;

    std::time_t end_time = std::chrono::system_clock::to_time_t(t2);
    time_t end_timea     = system_clock::to_time_t(t2); // using namespace std::chrono;


    std::cout 
        << "finished computation at " << std::ctime(&end_time)
        << "elapsed time: " << std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1).count() << " seconds.\n"
        << "finished computation at " << ctime(&end_timea)
        << "elapsed time: " << duration_cast<duration<double>>(t2a - t1a).count() << " seconds.\n" // using namespace std::chrono;
        << "elapsed time: " << (t2 - t1).count() << " nanoseconds.\n"
        << "elapsed time: " << (t2a - t1a).count() << " nanoseconds.\n"
        ;
}
