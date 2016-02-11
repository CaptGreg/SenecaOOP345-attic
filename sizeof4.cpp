#include <iostream>
using namespace std;

#define  SIZE 10000000

int main(int argc, char ** argv)
{
	short *big_buf = new short [SIZE];

	cout << sizeof(big_buf) << "\n";
	cout << big_buf << "\n";

	if(big_buf) delete [] big_buf;
}

