#include <cmath>         // random
#include <vector>
#include <iostream>
using namespace std;


template <class T>
T min(vector<T> v)
{
  T minValue = v[0];

  for(int i = 1; i < v.size(); i++)
    if(v[i] < minValue)
       minValue = v[i];

  return minValue;
}

template <class T>
T max(vector<T> v)
{
  T maxValue = v[0];

  for(int i = 1; i < v.size(); i++)
    if(v[i] > maxValue)
       maxValue = v[i];

  return maxValue;
}

template <class T>
void minmax(vector<T> v, T&min, T& max)
{
  min =  max = v[0];

  for(int i = 1; i < v.size(); i++) {
    if(v[i] > max)
       max = v[i];
    if(v[i] < min)
       min = v[i];
  }
}

template <class T>
void minmax2(vector<T> v, T& min, T& max)
{
  min =  max = v[0];

  for(int i = 0; i < v.size(); i += 2) {
    T v1 = v[i];
    T v2 = v[i+1];

    if( v1 < v2 ) {
      if(v1 < min) min = v1;
      if(v2 > max) max = v2;
    } else {
      if(v2 < min) min = v2;
      if(v1 > max) max = v1;
    }
  }
}


int main(int argc, char**argv)
{
  int NUM = 1000;
  if(argc > 1) NUM = atoi(argv[1]);

  vector<double> v(NUM);

  for(int i = 0; i < v.size(); i++)
    v[i] = 1 + random();

  cout << "min is " << min(v) << "\n";
  cout << "max is " << max(v) << "\n";

  double min, max;

  minmax(v, min, max);
  cout << "minmax: min is " << min << "\n";
  cout << "minmax: max is " << max << "\n";

  minmax2(v, min, max);
  cout << "minmax2: min is " << min << "\n";
  cout << "minmax2: max is " << max << "\n";
}
