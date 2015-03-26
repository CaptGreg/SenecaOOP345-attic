#include "Mutex.h"

int main(int argc,char**argv)
{
  Mutex mtx;

  mtx.lock();
  mtx.unlock();
  mtx.trylock();
}
