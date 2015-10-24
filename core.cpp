#include <thread>
#include <iostream>
#include <sys/utsname.h>     // uname

using namespace std;

/**
 * CPUID assembler instruction
 */
static inline void cpuid(uint32_t op, uint32_t *eax, uint32_t *ebx,
  uint32_t *ecx, uint32_t *edx)
{
  asm("cpuid" : "=a" (*eax), "=b" (*ebx), "=c" (*ecx), "=d" (*edx) : "a" (op) : "cc");
}
static inline uint32_t cpuid(uint32_t op, uint32_t r[4]) 
{
  asm volatile("cpuid":"=a"(*r),"=b"(*(r+1)), "=c"(*(r+2)),"=d"(*(r+3)):"a"(op));
  return (uint32_t)r[0];
}

/**
 * retrieve CPU Vender from CPU hardware
 */
char *CPUVendor(void)
{
   uint32_t eax, ebx, ecx, edx;
   static uint32_t Vendor[4];

   eax = 0; cpuid(eax, &eax, &ebx, &ecx, &edx);
   Vendor[0] = ebx; Vendor[1] = edx; Vendor[2] = ecx; Vendor[3] = 0;
   return (char *) Vendor;
}

/**
 * retrieve CPU Name from CPU hardware
 */
char *CPUName(void)
{
  uint32_t eax;
  static uint32_t Name[13];
  //                           &eax      &ebx      &ecx      &edx
  eax = 0x80000002; cpuid(eax, Name + 0, Name + 1, Name + 2, Name + 3);
  eax = 0x80000003; cpuid(eax, Name + 4, Name + 5, Name + 6, Name + 7);
  eax = 0x80000004; cpuid(eax, Name + 8, Name + 9, Name +10, Name +11);
  Name[12] = 0;

  return (char *) Name;
}


/**
 * print machine information
 */
void myMachineInfo()
{
  cout << "CPU Vender = " << CPUVendor() << "\n";
  cout << "CPU Model  = " << CPUName()   << "\n";

  cout << "\nLinux details:\n";

  char *cmd = (char*) "/usr/bin/lsb_release -a"; // -i ID, -d description, -r release, -c code name, or -a for all
  FILE *pipe = popen(cmd, "r");
  if(pipe) {
    char b[1000];
    while(fgets(b, sizeof(b)-1, pipe)) cout << b;
    pclose(pipe);
  }

  struct utsname uname_pointer;
  uname(&uname_pointer);
  cout << uname_pointer.sysname << " " 
       << uname_pointer.machine 
       << " kernel:" << uname_pointer.release
       << " (" << uname_pointer.version << ")"
       << "\n";

  cout << "application (file " << __FILE__ << ") "
       <<  "compiled: " << __DATE__<< " "
       << __TIME__
       << "\n";
}


int main(int argc, char ** argv)
{
    auto cores = thread::hardware_concurrency();

    cout << cores << " cores\n\n";

    myMachineInfo();
}
