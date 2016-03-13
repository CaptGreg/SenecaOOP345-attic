#if 0
//////////////////////////////////////////////////////////////////////
// http://www.1024cores.net/home/lock-free-algorithms/tricks/fibers //
//////////////////////////////////////////////////////////////////////

#include <csetjmp>
#include <cstdlib>
#include <malloc.h>
#include <ucontext.h>

struct fiber_t
{
    ucontext_t  fib;
    jmp_buf     jmp;
};
 
struct fiber_ctx_t
{
    void(*      fnc)(void*);
    void*       ctx;
    jmp_buf*    cur;
    ucontext_t* prv;
};
 
static void fiber_start_fnc(void* p)
{
    fiber_ctx_t* ctx = (fiber_ctx_t*)p;
    void (*ufnc)(void*) = ctx->fnc;
    void* uctx = ctx->ctx;
    if (_setjmp(*ctx->cur) == 0)
    {
        ucontext_t tmp;
        swapcontext(&tmp, ctx->prv);
    }
    ufnc(uctx);
}
 
inline void create_fiber(fiber_t& fib, void(*ufnc)(void), void* uctx)
{
    getcontext(&fib.fib);
    size_t const stack_size = 64*1024;
    fib.fib.uc_stack.ss_sp = (::malloc)(stack_size);
    fib.fib.uc_stack.ss_size = stack_size;
    fib.fib.uc_link = 0;
    ucontext_t tmp;
    fiber_ctx_t ctx = {ufnc, uctx, &fib.jmp, &tmp}; // GB syntax error
    makecontext(&fib.fib, (void(*)())fiber_start_fnc, 1, &ctx);
    swapcontext(&tmp, &fib.fib);
}
 
inline void switch_to_fiber(fiber_t& fib, fiber_t& prv)
{
    if (_setjmp(prv.jmp) == 0)
        _longjmp(fib.jmp, 1);
}

int main()
{
}
#endif

//////////////////////////////////////////////
// https://en.wikipedia.org/wiki/Setcontext //
//////////////////////////////////////////////

#include <cstdio>
#include <cstdlib>
#include <iostream>
// replace system dependent sleep(nap) with c++11 OS independent this_thread::sleep_for(std::chono::second(nap))
// #include <unistd.h>         // sleep
#include <thread>
#include <chrono>
void sleep(int nap) { std::this_thread::sleep_for(std::chrono::seconds(nap)); }
#include <ucontext.h>

/* The three contexts:
 *    (1) main_context1 : The point in main to which loop will return.
 *    (2) main_context2 : The point in main to which control from loop will
 *                        flow by switching contexts.
 *    (3) loop_context  : The point in loop to which control from main will
 *                        flow by switching contexts. */
ucontext_t main_context1, main_context2, loop_context;

/* The iterator return value. */
volatile int i_from_iterator;

/* This is the iterator function. It is entered on the first call to
 * swapcontext, and loops from 0 to 9. Each value is saved in i_from_iterator,
 * and then swapcontext used to return to the main loop.  The main loop prints
 * the value and calls swapcontext to swap back into the function. When the end
 * of the loop is reached, the function exits, and execution switches to the
 * context pointed to by main_context1. */
void loop(
    ucontext_t *loop_context,
    ucontext_t *other_context,
    int *i_from_iterator)
{
    for (int i=0; i < 10; ++i) {
        /* Write the loop counter into the iterator return location. */
        *i_from_iterator = i;
        
        /* Save the loop context (this point in the code) into ''loop_context'',
         * and switch to other_context. */
        swapcontext(loop_context, other_context);
    }
    
    /* The function falls through to the calling context with an implicit
     * ''setcontext(&loop_context->uc_link);'' */
} 
 
int main1(void)
{
    /* The stack for the iterator function. */
    char iterator_stack[SIGSTKSZ];

    /* Flag indicating that the iterator has completed. */
    volatile int iterator_finished;

    getcontext(&loop_context);
    /* Initialise the iterator context. uc_link points to main_context1, the
     * point to return to when the iterator finishes. */
    loop_context.uc_link          = &main_context1;
    loop_context.uc_stack.ss_sp   = iterator_stack;
    loop_context.uc_stack.ss_size = sizeof(iterator_stack);

    /* Fill in loop_context so that it makes swapcontext start loop. The
     * (void (*)(void)) typecast is to avoid a compiler warning but it is
     * not relevant to the behaviour of the function. */
    makecontext(&loop_context, (void (*)(void)) loop,
        3, &loop_context, &main_context2, &i_from_iterator);
   
    /* Clear the finished flag. */      
    iterator_finished = 0;

    /* Save the current context into main_context1. When loop is finished,
     * control flow will return to this point. */
    getcontext(&main_context1);
  
    if (!iterator_finished) {
        /* Set iterator_finished so that when the previous getcontext is
         * returned to via uc_link, the above if condition is false and the
         * iterator is not restarted. */
        iterator_finished = 1;
       
        while (1) {
            /* Save this point into main_context2 and switch into the iterator.
             * The first call will begin loop.  Subsequent calls will switch to
             * the swapcontext in loop. */
            swapcontext(&main_context2, &loop_context);
            std::cout <<  i_from_iterator << "\n";
        }
    }
    
    auto c = [] {
      int i = 0;
      ucontext_t context;

      getcontext(&context);
      std::cout << "main1: Hello world " << ++i << " times through the loop\n";

      sleep(1); // or std::this_thread::sleep_for(std::chrono::seconds(1));
      if(i < 5) setcontext(&context);
    };
    c();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// http://www.gnu.org/software/libc/manual/html_mono/libc.html#System-V-contexts //
///////////////////////////////////////////////////////////////////////////////////
// Something which is harder to implement with setjmp and longjmp is to switch temporarily to a different execution path and then resume where execution was stopped.

#include <csignal>
#include <cstdlib>
#include <ucontext.h>
#include <sys/time.h>

/* Set by the signal handler. */
static volatile int expired;

/* The contexts. */
static ucontext_t uc[3];

/* We do only a certain number of switches. */
static int switches;


/* This is the function doing the work.  It is just a
   skeleton, real code has to be filled in. */
static void
f (int n)
{
  int m = 0;
  while (1)
    {
      /* This is where the work would be done. */
      if (++m % 100 == 0)
        {
          std::cout << '.';
        }

      /* Regularly the expire variable must be checked. */
      if (expired)
        {
          /* We do not want the program to run forever. */
          if (++switches == 20)
            return;

          std::cout <<  "\nswitching from " << n << " to " << 3-n << "\n";
          expired = 0;
          /* Switch to the other context, saving the current one. */
          swapcontext (&uc[n], &uc[3 - n]);
        }
    }
}

/* This is the signal handler which simply set the variable. */
void
handler (int signal)
{
  expired = 1;
}


int
main2 (void)
{
  struct sigaction sa;
  struct itimerval it;
  char st1[8192];
  char st2[8192];

  /* Initialize the data structures for the interval timer. */
  sa.sa_flags = SA_RESTART;
  sigfillset (&sa.sa_mask);
  sa.sa_handler = handler;
  it.it_interval.tv_sec = 0;
  it.it_interval.tv_usec = 1;
  it.it_value = it.it_interval;

  /* Install the timer and get the context we can manipulate. */
  if (sigaction (SIGPROF, &sa, NULL) < 0
      || setitimer (ITIMER_PROF, &it, NULL) < 0
      || getcontext (&uc[1]) == -1
      || getcontext (&uc[2]) == -1)
    abort ();

  /* Create a context with a separate stack which causes the
     function f to be call with the parameter 1.
     Note that the uc_link points to the main context
     which will cause the program to terminate once the function
     return. */
  uc[1].uc_link = &uc[0];
  uc[1].uc_stack.ss_sp = st1;
  uc[1].uc_stack.ss_size = sizeof st1;
  makecontext (&uc[1], (void (*) (void)) f, 1, 1);

  /* Similarly, but 2 is passed as the parameter to f. */
  uc[2].uc_link = &uc[0];
  uc[2].uc_stack.ss_sp = st2;
  uc[2].uc_stack.ss_size = sizeof st2;
  makecontext (&uc[2], (void (*) (void)) f, 1, 2);

  /* Start running. */
  swapcontext (&uc[0], &uc[1]);
  putchar ('\n');

  return 0;
}

// This an example how the context functions can be used to implement co-routines or cooperative multi-threading. All that has to be done is to call every once in a while swapcontext to continue running a different context. It is not recommended to do the context switching from the signal handler directly since leaving the signal handler via setcontext if the signal was delivered during code that was not asynchronous signal safe could lead to problems. Setting a variable in the signal handler and checking it in the body of the functions which are executed is a safer approach. Since swapcontext is saving the current context it is possible to have multiple different scheduling points in the code. Execution will always resume where it was left. 

/////////////////////////////////////////////////////
// http://www.linuxhowtos.org/C_C++/coroutines.htm //
/////////////////////////////////////////////////////

// Knuth's coroutines

// In The Art of Computer Programming, Donald Knuth presents a solution to this sort of problem. His answer is to throw away the stack concept completely. Stop thinking of one process as the caller and the other as the callee, and start thinking of them as cooperating equals.

// In practical terms: replace the traditional "call" primitive with a slightly different one. The new "call" will save the return value somewhere other than on the stack, and will then jump to a location specified in another saved return value. So each time the decompressor emits another character, it saves its program counter and jumps to the last known location within the parser - and each time the parser needs another character, it saves its own program counter and jumps to the location saved by the decompressor. Control shuttles back and forth between the two routines exactly as often as necessary.

// This is very nice in theory, but in practice you can only do it in assembly language, because no commonly used high level language supports the coroutine call primitive. Languages like C depend utterly on their stack-based structure, so whenever control passes from any function to any other, one must be the caller and the other must be the callee. So if you want to write portable code, this technique is at least as impractical as the Unix pipe solution.

// Stack-based coroutines


// So what we would really like is the ability to mimic Knuth's coroutine call primitive in C. We must accept that in reality, at the C level, one function will be caller and the other will be callee. In the caller, we have no problem; we code the original algorithm, pretty much exactly as written, and whenever it has (or needs) a character it calls the other function.

// The callee has all the problems. For our callee, we want a function which has a "return and continue" operation: return from the function, and next time it is called, resume control from just after the return statement. For example, we would like to be able to write a function that says

// int function(void) {  
    // int i;  
    // for (i = 0; i < 10; i++)  
        // return i;   /* won't work, but wouldn't it be nice */  
// }


// and have ten successive calls to the function return the numbers 0 through 9.

// How can we implement this? Well, we can transfer control to an arbitrary point in the function using a goto statement. So if we use a state variable, we could do this:

int main3(void) 
{
    static int i, state = 0;  
    switch (state) {  
        case 0: goto LABEL0;  
        case 1: goto LABEL1;  
    }  
    LABEL0: /* start of function */  
    for (i = 0; i < 10; i++) {  
        state = 1; /* so we will come back to LABEL1 */  
        return i;  
        LABEL1: /* resume control straight after the return */  
        ; // need a statement after a label
    }  
    return 0;
}

// This method works. We have a set of labels at the points where we might need to resume control: one at the start, and one just after each return statement. We have a state variable, preserved between calls to the function, which tells us which label we need to resume control at next. Before any return, we update the state variable to point at the right label; after any call, we do a switch on the value of the variable to find out where to jump to.

// It's still ugly, though. The worst part of it is that the set of labels must be maintained manually, and must be consistent between the function body and the initial switch statement. Every time we add a new return statement, we must invent a new label name and add it to the list in the switch; every time we remove a return statement, we must remove its corresponding label. We've just increased our maintenance workload by a factor of two. 

int main4(void) 
{
    static int i, state = 0;  
    switch (state) {  
        case 0: /* start of function */  
        for (i = 0; i < 10; i++) {  
            state = 1; /* so we will come back to "case 1" */  
            return i;  
            case 1: /* resume control straight after the return */  
            ; // need a statement after a label
        }  
    }  
    return 0;
}

#define crBegin static int state=0; switch(state) { case 0:  
#define crReturn(i,x) do { state=i; return x; case i:; } while (0)  
#define crFinish }  
int main5(void) 
{  
    static int i;  
    crBegin;  
    for (i = 0; i < 10; i++)  
        crReturn(1, i);  
    crFinish;  
    return 0;
}
// (note the use of do ... while(0) to ensure that crReturn does not need braces around it when it comes directly between if and else)

// This is almost exactly what we wanted. We can use crReturn to return from the function in such a way that control at the next call resumes just after the return. Of course we must obey some ground rules (surround the function body with crBegin and crFinish; declare all local variables static if they need to be preserved across a crReturn; never put a crReturn within an explicit switch statement); but those do not limit us very much.

// The only snag remaining is the first parameter to crReturn. Just as when we invented a new label in the previous section we had to avoid it colliding with existing label names, now we must ensure all our state parameters to crReturn are different. The consequences will be fairly benign - the compiler will catch it and not let it do horrible things at run time - but we still need to avoid doing it.

// Even this can be solved. ANSI C provides the special macro name __LINE__, which expands to the current source line number. So we can rewrite crReturn as

// #define crReturn(x) do { state=__LINE__; return x; case __LINE__:; } while (0)

// and then we no longer have to worry about those state parameters at all, provided we obey a fourth ground rule (never put two crReturn statements on the same line).

#define crBetterReturn(x) do { state=__LINE__; return x; case __LINE__:; } while (0)
int main6(void) 
{  
    static int i;  
    crBegin;  
    for (i = 0; i < 10; i++)  
        crBetterReturn(i);  
    crFinish;  
    return 0;
}


int main(void)
{
  main1();
  main2();

  for(int i = 0; i < 12 ; i++) std::cout << main3() << " "; std::cout << "\n";
  for(int i = 0; i < 14 ; i++) std::cout << main4() << " "; std::cout << "\n";
  for(int i = 0; i < 16 ; i++) std::cout << main5() << " "; std::cout << "\n";
  for(int i = 0; i < 18 ; i++) std::cout << main6() << " "; std::cout << "\n";

  return 0;
}
