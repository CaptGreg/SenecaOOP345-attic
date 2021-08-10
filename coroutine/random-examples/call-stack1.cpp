// g++ -Wall -std=c++11  -ggdb -fopenmp call-stack1.cpp -o call-stack1

// For windows callstack code, try http://www.codeproject.com/Articles/11132/Walking-the-callstack

// http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes

/*
Appendix: __cxa_demangle Documentation

From the libstdc++ documentation (abi Namespace Reference):

char* abi::__cxa_demangle(const char* mangled_name,
                          char* output_buffer, size_t* length,
                          int* status)

Parameters:

    mangled_name
        A NUL-terminated character string containing the name to be demangled.
    output_buffer
        A region of memory, allocated with malloc, of *length bytes, into which the demangled name is stored. If output_buffer is not long enough, it is expanded using realloc. output_buffer may instead be NULL; in that case, the demangled name is placed in a region of memory allocated with malloc.
    length
        If length is non-NULL, the length of the buffer containing the demangled name is placed in *length.
    status
        *status is set to one of the following values:

            0: The demangling operation succeeded.
            -1: A memory allocation failure occurred.
            -2: mangled_name is not a valid name under the C++ ABI mangling rules.
            -3: One of the arguments is invalid.

Returns:
    A pointer to the start of the NUL-terminated demangled name, or NULL if the demangling fails. The caller is responsible for deallocating this memory using free.

RSS 2.0 Weblog Feed
*/

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <cxxabi.h>  // cxx name demangling code
#include <iostream>
#include <execinfo.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ucontext.h>
#include <unistd.h>

/* This structure mirrors the one found in /usr/include/asm/ucontext.h */
typedef struct _sig_ucontext {
 unsigned long     uc_flags;
 struct ucontext   *uc_link;
 stack_t           uc_stack;
 struct sigcontext uc_mcontext;
 sigset_t          uc_sigmask;
} sig_ucontext_t;

void crit_err_hdlr1(int sig_num, siginfo_t * info, void * ucontext) // GB works
{
 void *             array[50];
 void *             caller_address;
 char **            messages;
 int                size, i;
 sig_ucontext_t *   uc;

 uc = (sig_ucontext_t *)ucontext;

 /* Get the address at the time the signal was raised */
#if defined(__i386__) // gcc specific
 caller_address = (void *) uc->uc_mcontext.eip; // EIP: x86 specific
#elif defined(__x86_64__) // gcc specific
 caller_address = (void *) uc->uc_mcontext.rip; // RIP: x86_64 specific
#else
#error Unsupported architecture. // TODO: Add support for other arch.
#endif

 fprintf(stderr, "signal %d (%s), address is %p from %p\n", 
  sig_num, strsignal(sig_num), info->si_addr, 
  (void *)caller_address);

 size = backtrace(array, 50);

 /* overwrite sigaction with caller's address */
 array[1] = caller_address;

 messages = backtrace_symbols(array, size);

 /* skip first stack frame (points here) */
 for (i = 1; i < size && messages != NULL; ++i)
 {
  fprintf(stderr, "[bt]: (%d) %s\n", i, messages[i]);
 }

 free(messages);

 exit(EXIT_FAILURE);
}

int crash()
{
 char * p = NULL;
 *p = 0;
 return 0;
}

int foo4()
{
 crash();
 return 0;
}

int foo3()
{
 foo4();
 return 0;
}

int foo2()
{
 foo3();
 return 0;
}

int foo1()
{
 foo2();
 return 0;
}

// The following C++ Linux example uses the same signal handler as my other answer and demonstrates how c++filt can be used to demangle the symbols.
class foo
{
public:
    foo() { foo1(); }

private:
    void foo1() { foo2(); }
    void foo2() { foo3(); }
    void foo3() { foo4(); }
    void foo4() { crash(); }
public:
    void crash() { char * p = NULL; *p = 0; }
};

void crit_err_hdlr2(int sig_num, siginfo_t * info, void * ucontext) // GB works
{
    sig_ucontext_t * uc = (sig_ucontext_t *)ucontext;

    void * caller_address; //  = (void *) uc->uc_mcontext.eip; // x86 specific // GB fails on x86_64
#if defined(__i386__) // gcc specific
    caller_address = (void *) uc->uc_mcontext.eip; // EIP: x86 specific
#elif defined(__x86_64__) // gcc specific
    caller_address = (void *) uc->uc_mcontext.rip; // RIP: x86_64 specific
#else
    #error Unsupported architecture. // TODO: Add support for other arch.
#endif


    std::cerr << "signal " << sig_num 
              << " (" << strsignal(sig_num) << "), address is " 
              << info->si_addr << " from " << caller_address 
              << "\n\n";

    void * array[50];
    int size = backtrace(array, 50);

    array[1] = caller_address;

    char ** messages = backtrace_symbols(array, size);    

    // skip first stack frame (points here)
    for (int i = 1; i < size && messages != NULL; ++i) {
        char *mangled_name = 0, *offset_begin = 0, *offset_end = 0;

        // find parantheses and +address offset surrounding mangled name
        for (char *p = messages[i]; *p; ++p) {
            if (*p == '(') {
                mangled_name = p; 
            } else if (*p == '+') {
                offset_begin = p;
            } else if (*p == ')') {
                offset_end = p;
                break;
            }
        }

        // if the line could be processed, attempt to demangle the symbol
        if (mangled_name && offset_begin && offset_end && mangled_name < offset_begin) {
            *mangled_name++ = '\0';
            *offset_begin++ = '\0';
            *offset_end++ = '\0';

            int status;
            char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status); // GB syntax error

            // if demangling is successful, output the demangled function name
            if (status == 0) {    
                std::cerr << "[bt]: (" << i << ") " << messages[i] << " : " 
                          << real_name << "+" << offset_begin << offset_end 
                          << "\n";

            } else { // otherwise, output the mangled function name
                std::cerr << "[bt]: (" << i << ") " << messages[i] << " : " 
                          << mangled_name << "+" << offset_begin << offset_end 
                          << "\n";
            }
            free(real_name);
        } else { // otherwise, print the whole line
            std::cerr << "[bt]: (" << i << ") " << messages[i] << "\n";
        }
    }
    std::cerr << "\n";

    free(messages);

    exit(EXIT_FAILURE);
}

int main(int argc, char ** argv)
{
 struct sigaction sigact;

 sigact.sa_sigaction = crit_err_hdlr1;  // GB works
 sigact.sa_sigaction = crit_err_hdlr2;  // GB works
 sigact.sa_flags = SA_RESTART | SA_SIGINFO;

 if (sigaction(SIGSEGV, &sigact, (struct sigaction *)NULL) != 0)
 {
  fprintf(stderr, "error setting signal handler for %d (%s)\n",
    SIGSEGV, strsignal(SIGSEGV));

  exit(EXIT_FAILURE);
 }

 foo * f = new foo();
 f->crash();

 foo1();

 exit(EXIT_SUCCESS);
}
