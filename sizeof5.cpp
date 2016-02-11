#include <iostream>
using namespace std;

#define  SIZE 10000000

int main(int argc, char ** argv)
{
	short *big_buf;

	cout << sizeof(big_buf) << "\n";

	for(int i = 1; 1; i++) {
		cout << i << ": allocating " << SIZE << " bytes\n";
		big_buf = new short [SIZE];

		cout << big_buf << "\n";
		if(!big_buf) {
			cout << "new failed!\n";
			break;
		}
	}

}

