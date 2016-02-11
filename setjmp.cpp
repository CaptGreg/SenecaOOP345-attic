#include <iostream>
using namespace std;
#include <csetjmp>

jmp_buf jb;

void cppFun(void)
{
    std::cout << "This is cpp function cppFun\n";
    longjmp(jb,-99);
}

int main(int argc, char**argv)
{
    int r;
    if(r = setjmp(jb)) {
	std::cout << "setjmp returned " << r << "\n";
	return 1;
    }
    cppFun();
    return 0;
}
