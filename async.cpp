// http://www.stroustrup.com/C++11FAQ.html#std-future

// GB does not compile, pound on it to coerce it to compile ...

#include <iostream>
#include <cmath>         // random()

#include <vector>
#include <thread>
#include <algorithm> // accumulate

using namespace std;

template<class T, class V> 
struct Accum  {	// simple accumulator function object
	T* b;
	T* e;
	V val;
	Accum(T* bb, T* ee, const V& vv) : b{bb}, e{ee}, val{vv} {}
	V operator() () { return std::accumulate(b,e,val); }
};

#if 1
struct AccumDoubleVector  {	// simple accumulator function object
	vector<double>* b;
	vector<double>* e;
	double val;
	AccumDoubleVector(vector<double>* bb, vector<double>* ee, const double& vv) : b{bb}, e{ee}, val{vv} {}
	double operator() () { return 1; /* std::accumulate(b,e,val); */ }
};
#endif


double comp(vector<double>& v, unsigned nThreads)
	// spawn many tasks if v is large enough
{
	if (v.size()<10000) return std::accumulate(v.begin(),v.end(),0.0);

#if 0
	auto f0 = [v] {  return async(
                                Accum<vector<double>,double> ( &v[0], &v[v.size()/4], 0.0 ) 
                                ) 
                      };
#endif
//      auto f0 {async(Accum{&v[0],&v[v.size()/4],0.0})};
//      auto f1 {async(Accum{&v[v.size()/4],&v[v.size()/2],0.0})};
//      auto f2 {async(Accum{&v[v.size()/2],&v[v.size()*3/4],0.0})};
//      auto f3 {async(Accum{&v[v.size()*3/4],&v[v.size()],0.0})};
// error: Accum needs template info

//	auto f0 {  async( Accum<vector<double>,double> {&v[0],             &v[v.size()/4],    0.0} )  };
//	auto f1 {  async( Accum<vector<double>,double> {&v[v.size()/4],    &v[v.size()/2],    0.0} )  };
//	auto f2 {  async( Accum<vector<double>,double> {&v[v.size()/2],    &v[v.size()*3/4],  0.0} )  };
//	auto f3 {  async( Accum<vector<double>,double> {&v[v.size()*3/4],  &v[v.size()],      0.0} )  };

	// return f0.get()
            ; //+f1.get()+f2.get()+f3.get();
}

int main(int argc, char**argv)
{
  // std::cout << std::thread::hardware_concurrency() << " hardware cores\n";
  unsigned coreCount {std::thread::hardware_concurrency()};
  cout << coreCount << " hardware cores\n";

  vector<double> v;
  for(int i = 0 ; i < 8000 ; i++) 
    v.push_back(random());

  Accum<vector<double>, double> compiles();
  // Accum<vector<double>, double> compiles2 (v.begin(), v.end(), 0.0);
  Accum<vector<double>, double> compiles2 (&v[0], &v[v.size()], 0.0);
  // warning: empty parentheses interpreted as a function declaration [-Wvexing-parse]
  //   Accum<vector<double>, double> compiles();
  //                                         ^~
  // note: replace parentheses with an initializer to declare a variable
  //   Accum<vector<double>, double> compiles();
  //                                         ^~
  //                                         {}


  // cout <<  "inline declaration=" << Accum<vector<double>, double> (&v[0], &v[v.size()], 0.0) << "\n";

  // Accum<vector<double>, double> fo{&v[0], &v[v.size()], 0.0};
  // error: no matching constructor for initialization of 'Accum<vector<double>, double>'
  //   Accum<vector<double>, double> fo{&v[0], &v[v.size()], 0.0};
  //                                 ^ ~~~~~~~~~~~~~~~~~~~~~~~~~~
  // note: candidate constructor not viable: no known conversion from 'value_type *' (aka 'double *') 
  // to 'std::vector<double, std::allocator<double> > *' for 1st argument
  //         Accum(T* bb, T* ee, const V& vv) : b{bb}, e{ee}, val{vv} {}
  //         ^
  // note: candidate constructor (the implicit copy constructor) not viable: requires 1 argument, but 3 were provided
  // struct Accum  { // simple accumulator function object
  //        ^
  // note: candidate constructor (the implicit move constructor) not viable: requires 1 argument, but 3 were provided

  // Accum<vector<double>, double> fo(&v[0], &v[v.size()], 0.0);
  // error: no matching constructor for initialization of 'Accum<vector<double>, double>'
  //   Accum<vector<double>, double> fo(&v[0], &v[v.size()], 0.0);
  //                                 ^  ~~~~~~~~~~~~~~~~~~~~~~~~
  // note: candidate constructor not viable: no known conversion from 'value_type *' (aka 'double *') 
  // to 'std::vector<double, std::allocator<double> > *' for 1st argument
  //         Accum(T* bb, T* ee, const V& vv) : b{bb}, e{ee}, val{vv} {}
  //         ^
  // note: candidate constructor (the implicit copy constructor) not viable: requires 1 argument, but 3 were provided
  // struct Accum  { // simple accumulator function object
  //        ^
  // note: candidate constructor (the implicit move constructor) not viable: requires 1 argument, but 3 were provided

  cout << "comp=" << comp(v, coreCount) << "\n";;
}
