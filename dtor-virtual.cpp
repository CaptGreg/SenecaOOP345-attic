#include <iostream>
#include <string>
using namespace std;

class B {
  string name;
public:
		B() {cout << "B(" << name << ")\n"; }
		B(string s) : name(s) {cout << "B(" << name << ")\n"; }
		~B() {cout << "~B(" << name << ")\n";}
};

class D : public B {
  string name;
public:
		// D() {cout << "D(" << name << ")\n"; }
		D(string s) : B("B base for " + s), name(s) {cout << "D(" << name << ")\n"; }
		~D() {cout << "~D(" << name << ")\n";}
};

class BV {
  string name;
public:
		BV() {cout << "BV(" << name << ")\n"; }
		BV(string s) : name(s) {cout << "BV(" << s << ")\n"; }
		virtual ~BV() {cout << "~BV(" << name << ")\n";}
};

class DV : public BV {
  string name;
public:
		DV(string s) : BV("BV base for " + s), name(s) {cout << "DV(" << name << ")\n"; }
		virtual ~DV() {cout << "~DV(" << name << ")\n";}
};


int main()
{
#ifdef __VERSION__ 
	cout << "compiled with g++-" << __VERSION__ << "\n\n";
#endif

	{
		cout << "B scope block\n";
		B b("b");
	}
	cout << "\n";

	{
		cout << "D scope block\n";
		D d("d");
    B* bp = new D("bp");
	  cout << "  NOTE: D(bp) called, expect to see ~D(bp) called.\n";
    delete bp;
	}
	cout << "  BUT ~D(bp) never called!!!\n";
	cout << "\n";

	{
		cout << "BV scope block\n";
		BV bv("bv");
	}
	cout << "\n";

	{
		cout << "DV scope block\n";
		DV dv("dv");
    BV* bvp = new DV("bvp");
	  cout << "  NOTE: DV(bvp) called, expect to see ~DV(bvp) called.\n";
    delete bvp;
	}
	cout << "  NOTE: ~DV(bvp) IS called!!!\n";
	cout << "\n";

	return 0;
}
