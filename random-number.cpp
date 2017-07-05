// http://www.cplusplus.com/reference/random/uniform_int_distribution/

#if 0
++++++++++++++++++++++++++++++++++++++++++++++++++++===
Uniform discrete distribution
Random number distribution that produces integer values according to a uniform discrete 
distribution, which is described by the following probability mass function:

This distribution produces random integers in a range [a,b] where each possible value has 
an equal likelihood of being produced. This is the distribution function that appears on 
many trivial random processes (like the result of rolling a die).

The distribution parameters, a and b, are set on construction.

To produce a random value following this distribution, call its member function operator().

For a discrete distribution that can have different probabilities for each possible value, 
see discrete_distribution.

++++++++++++++++++++++++++++++++++++++++++++++++++++===
http://www.cplusplus.com/reference/random/uniform_int_distribution/operator()/
(1)	template<class URNG> std::uniform_int_distribution::operator() 
(2)	template<class URNG> result_type operator()(URNG& g);          

template<class URNG> result_type operator()(URNG& g, const param_type& parm);

Generate random number
Returns a new random number that follows the distribution's parameters associated to the 
object (version 1) or those specified by parm (version 2).

The generator object (g) supplies uniformly-distributed random integers through its operator() 
member function. The uniform_int_distribution object transforms the values obtained this way 
so that successive calls to this member function with the same arguments produce values that 
follow a uniform distribution within the appropriate range.

Parameters

g
    A uniform random number generator object, used as the source of randomness.
    URNG shall be a uniform random number generator type, such as one of the standard generator 
    classes.
parm
    An object representing the distribution's parameters, obtained by a call to member function param.
    param_type is a member type.


Return value
A new random number.
result_type is a member type, defined as an alias of the first class template parameter (IntType).
++++++++++++++++++++++++++++++++++++++++++++++++++++===
#endif

// uniform_int_distribution
#include <iostream>
#include <random>
#include <iomanip>
#include <numeric>      // std::accumulate

int main()
{
  const int nrolls = 1000*1000*1000; // number of experiments
  const int nstars = 95;    // maximum number of stars to distribute

  std::default_random_engine generator;                 // GB baseline RNG used to create a RN for a given distribution.
  std::uniform_int_distribution<int> distribution(0,9); // GB a uniform distribution with range [0,9].

  int histo[10]={};                                     // GB was variable 'p'.  It is a histogram so call it 'histo'.

  for (int i=0; i<nrolls; ++i) {
    int number = distribution(generator); // GB 'distribution' creates a RN with the correct statistical properties using 'generator'
    ++histo[number];
  }

  std::cout << "uniform_int_distribution (0,9):" << std::endl;
  // GB - print histo counts
  for (int i=0; i<10; ++i)
    std::cout << std::setw(10) << histo[i] <<" " <<  i << ": " << std::string(histo[i]*nstars/nrolls,'*') << std::endl;

  size_t s =  sizeof(histo)/sizeof(histo[0]);
  std::cout << std::setw(10) << std::string(s,'+') << std::endl;
  std::cout << std::setw(10) << std::accumulate(histo, histo + s, 0 ) << std::endl;
  return 0;
}
