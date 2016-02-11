#include <iostream>
using namespace std;

#include <math.h>

#define  SIZE 10000000

 class Pt {
public:
	// Pt (int x, int y) {_x = x; _y = y; }

	Pt (int x, int y) : _x(x), _y(y) { _z = sqrt(x*x + y*y); big_buf = new float [10000]; } // ctor (constructor) 1
	Pt (int x, int y, int z) : _x(x), _y(y), _z(z) { big_buf = new float [10000]; }         // ctor (constructor) 2
	Pt () { big_buf = new float [10000];  }

	~Pt() { if(big_buf) delete [] big_buf;  big_buf = (float*) 0; } // dtor (destructor)
	int   getX( ) { clearBigBuf(); return _x; }
	int   getY( ) { return _y; }
	float getZ( ) { return _z; }
private:
	int _x;
	int _y;
	float _z;
	float *big_buf;

	void clearBigBuf() { for (int i = 0; i < 10000; i++) big_buf[i]= 27.92; }
};

int main(int argc, char ** argv)
{
	Pt *big_buf;
	Pt p1;
	Pt p2;
	Pt p3;
	Pt p4(123,456);

	// p3.clearBigBuf(); // private func, cannot call.

	cout << "x,y,z = " << p4.getX() << "," << p4.getY() << "," << p4.getZ() << "\n";

#if 0
	cout << sizeof(big_buf) << "\n";

	for(int i = 1; 1; i++) {
		cout << i << ": allocating " << SIZE << " bytes\n";
		big_buf = new Pt [SIZE];

		cout << big_buf << "\n";
		if(!big_buf) {
			cout << "new failed!\n";
			break;
		}
		delete [] big_buf;
	}

#endif
}

