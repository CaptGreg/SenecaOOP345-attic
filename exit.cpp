#include <iostream>
#include <cstdlib>  // atexit, on_exit
#include <csignal>  // SIG???? + signal()
#include <cstring>  // strsignal()
#include <iomanip>

void atexitCallback(void)
{
  std::cout << "atexitCallBack\n";
}

void on_exitCallback(int i, void*p)
{
  std::cout << "on_exitCallBack " << i;
  if(p) std::cout << ":\"" << (const char*) p << "\"";
  std::cout << "\n";
}

void sigCallback(int sig)
{
  std::cout << "sigCallback caught signal " << std::setw(2) << sig;
  char* str = strsignal(sig);
  if(str) std::cout << ":" << str;
  std::cout << "\n";
}

int main()
{
  std::cout << "main start\n";

  signal(SIGABRT, sigCallback);
  signal(SIGUSR1, sigCallback);
  signal(SIGUSR2, sigCallback);

  // normal exit callbacks
  // not called if "abort" or "terminate" generates exit
  atexit(atexitCallback);
  on_exit(on_exitCallback, (void*) "time to go");

  // throw nullptr;  // un-caught throw generates terminate
  // terminate called after throwing an instance of 'decltype(nullptr)'

  // throw "hurl";  // un-caught throw generates terminate
  // terminate called after throwing an instance of 'char const*'


  kill(0,SIGABRT);         // send signal
  kill(0,SIGUSR1);         // send signal
  kill(0,SIGUSR2);         // send signal
  // abort(); // raises SIGABRT kill(0,SIGABRT) then exits, no exit callbacks

  std::cout << "main finish\n";

  // exit(123); // arg passed to on_exit callback

  return 456; // 456 passed to on_exit callback
}
