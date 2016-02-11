// http://www.stroustrup.com/C++11FAQ.html#std-future

// GB does not compile, pound on it to coerce it to compile ...

#include <iostream>
#include <cmath>         // random()

#include <vector>
#include <thread>
#include <algorithm> // accumulate
#include <future>    // async

using namespace std;

template<class T, class V> 
struct MyAccum  {	// simple accumulator function object
	T* b;
	T* e;
	V val;
	MyAccum(T* bb, T* ee, const V& vv) : b{bb}, e{ee}, val{vv} {}
	V operator() () { return std::accumulate(b,e,val); }
};

struct AccumDoubleVector  {	// simple accumulator function object
	vector<double>* b;
	vector<double>* e;
	double val;
	AccumDoubleVector(vector<double>* bb, vector<double>* ee, const double& vv) : b{bb}, e{ee}, val{vv} {}
	double operator() () { return 1; /* std::accumulate(b,e,val); */ }
};


double comp(vector<double>& v, unsigned nThreads)
	// spawn many tasks if v is large enough
{
	if (v.size()<10000) return std::accumulate(v.begin(),v.end(),0.0);


	// auto f0 = [&] {  return async( MyAccum< vector<double>, double >, &v[0], &v[v.size()/4], double(0) ); };
  std::function<double(double*, double*, double)> f = MyAccum< vector<double> ,double>;
	auto f0 = [&] {  return async( f, &v[0], &v[v.size()/4], double(0) ); };
  // auto f0 = [&] {return async(MyAccum, &v[0],&v[v.size()/4],0.0);};
  // auto f1 = [&] {return async(MyAccum, &v[v.size()/4],&v[v.size()/2],0.0);};
  // auto f2 = [&] {return async(MyAccum, &v[v.size()/2],&v[v.size()*3/4],0.0);};
  // auto f3 = [&] {return async(MyAccum, &v[v.size()*3/4],&v[v.size()],0.0);};
  // error: MyAccum needs template info

  //	auto f0 {  async( MyAccum<vector<double>,double> {&v[0],             &v[v.size()/4],    0.0} )  };
  //	auto f1 {  async( MyAccum<vector<double>,double> {&v[v.size()/4],    &v[v.size()/2],    0.0} )  };
  //	auto f2 {  async( MyAccum<vector<double>,double> {&v[v.size()/2],    &v[v.size()*3/4],  0.0} )  };
  //	auto f3 {  async( MyAccum<vector<double>,double> {&v[v.size()*3/4],  &v[v.size()],      0.0} )  };

	return f0.get();
	// return f0.get()+f1.get()+f2.get()+f3.get();
}

int main(int argc, char**argv)
{
  // std::cout << std::thread::hardware_concurrency() << " hardware cores\n";
  unsigned coreCount {std::thread::hardware_concurrency()};
  cout << coreCount << " hardware cores\n";

  vector<double> v;
  for(int i = 0 ; i < 8000 ; i++) 
    v.push_back(random());

  MyAccum<vector<double>, double> compiles();
  // MyAccum<vector<double>, double> compiles2 (v.begin(), v.end(), 0.0);
  // MyAccum<vector<double>, double> compiles3 (&v[0], &v[v.size()], 0.0);
  // warning: empty parentheses interpreted as a function declaration [-Wvexing-parse]
  //   MyAccum<vector<double>, double> compiles();
  //                                         ^~
  // note: replace parentheses with an initializer to declare a variable
  //   MyAccum<vector<double>, double> compiles();
  //                                         ^~
  //                                         {}


  // cout <<  "inline declaration=" << MyAccum<vector<double>, double> (&v[0], &v[v.size()], 0.0) << "\n";

  // MyAccum<vector<double>, double> fo{&v[0], &v[v.size()], 0.0};
  // error: no matching constructor for initialization of 'MyAccum<vector<double>, double>'
  //   MyAccum<vector<double>, double> fo{&v[0], &v[v.size()], 0.0};
  //                                 ^ ~~~~~~~~~~~~~~~~~~~~~~~~~~
  // note: candidate constructor not viable: no known conversion from 'value_type *' (aka 'double *') 
  // to 'std::vector<double, std::allocator<double> > *' for 1st argument
  //         MyAccum(T* bb, T* ee, const V& vv) : b{bb}, e{ee}, val{vv} {}
  //         ^
  // note: candidate constructor (the implicit copy constructor) not viable: requires 1 argument, but 3 were provided
  // struct MyAccum  { // simple accumulator function object
  //        ^
  // note: candidate constructor (the implicit move constructor) not viable: requires 1 argument, but 3 were provided

  // MyAccum<vector<double>, double> fo(&v[0], &v[v.size()], 0.0);
  // error: no matching constructor for initialization of 'MyAccum<vector<double>, double>'
  //   MyAccum<vector<double>, double> fo(&v[0], &v[v.size()], 0.0);
  //                                 ^  ~~~~~~~~~~~~~~~~~~~~~~~~
  // note: candidate constructor not viable: no known conversion from 'value_type *' (aka 'double *') 
  // to 'std::vector<double, std::allocator<double> > *' for 1st argument
  //         MyAccum(T* bb, T* ee, const V& vv) : b{bb}, e{ee}, val{vv} {}
  //         ^
  // note: candidate constructor (the implicit copy constructor) not viable: requires 1 argument, but 3 were provided
  // struct MyAccum  { // simple accumulator function object
  //        ^
  // note: candidate constructor (the implicit move constructor) not viable: requires 1 argument, but 3 were provided

  cout << "comp=" << comp(v, coreCount) << "\n";;
}
