// Q: How were return values handled before async, futires, and promises?
// A: Return the value in a parameter argument.
//    Better yet, use a struct to pass parameters into and out of the thread 

#include <iostream>
#include <thread>

int main(int argc,char**argv)
{
  // Grade 9 linear line equation: y = mx + b

  // pass an address to a structure with parameters and return value
  struct parms_s{
    double m;
    double x;
    double b;
    double y;
  } parms ={ 4,2.2,1.1 };

  auto threadParms = [] (parms_s* p) -> void { p->y = p->m * p->x + p->b; };

  // run the thread
  std::thread t = std::thread( threadParms, &parms);  // start up  the thread
  t.join();                                           // wait for the thread to finish

  std::cout << "threadParms returned " << parms.y << ", y=mx+b, "<<parms.m<<"*"<<parms.x<<"+"<<parms.b<<" = "<<parms.y<<"\n";

  parms.x=1.01;
  std::thread( threadParms, &parms).join();
  std::cout << "threadParms returned " << parms.y << ", y=mx+b, "<<parms.m<<"*"<<parms.x<<"+"<<parms.b<<" = "<<parms.y<<"\n";
}
