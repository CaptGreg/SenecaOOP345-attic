/* ---------------------------------------------------------------- */
/* PROGRAM  sig-1.cpp:                                              */
/* GB: converted to cpp                                             */
/*    This is a simple program that illustrates the use of signal   */
/* handler and the setjmp/longjmp pair.  It catches Ctrl-C.         */
/* ---------------------------------------------------------------- */

#include  <iostream>
using namespace std;
#include  <csignal>
#include  <csetjmp>
#include  <unistd.h>   // sleep, exit

jmp_buf  JumpBuffer;                    // a jump buffer            //


// ---------------------------------------------------------------- //
// FUNCTION  INThandler:                                            //
//    This function handles the SIGINT (Ctrl-C) signal.             //
// ---------------------------------------------------------------- //

void  INThandler(int  sig)
{
     char  c;

     if(sig == SIGINT) {
           // signal(sig, SIG_IGN);              /* ignore Ctrl-C for now    */
           cout << " OUCH, you hit Ctrl-C\n";
           cout << "Do you really want to quit? [y/n] ";
           cin >> c;
           if (c == 'y' || c == 'Y') {
                cout << "INThandler calling exit(0)\n";
                exit(0);
           }
           signal(sig, INThandler);   /* reinstall before return  */
           longjmp(JumpBuffer, 1);    /* return to the setjmp mark*/
      }
}


// ---------------------------------------------------------------- //
//                   main program starts here                       //
// ---------------------------------------------------------------- //

int  main(int argc, char**argv)
{
     signal(SIGINT, INThandler);        // install Ctrl-C handler

     cout << "Hit Ctrl-C at anytime ... \n";
     while (1) {                        // loop & wait Ctrl-C
          if (setjmp(JumpBuffer) == 0) {     // set a return mark
             sleep(1);
          } else {
              cout << "while loop continuing\n";
          }
     }
}
