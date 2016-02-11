#include <vector>
#include <cmath>
#include <iostream>
#include <functional>          // std::function<...(...)>

using namespace std;

 // ascending order comparison
 bool ascending(double a, double b) {
     return a > b;
 }

 // descending order comparison
 bool descending(double a, double b) {
     return a < b;
 }

 // bubble sort
 void sortBubble(vector<double>& a, int n, 
  // bool (*comp)(double, double)               // c style
  std::function<bool(double,double)> comp       // C++ style
 ) 
 { 
     for (int i = n - 1; i > 0; i--) {
         for (int j = 0; j < i; j++) {
             if (comp(a[j], a[j+1])) {
                 double temp = a[j];
                 a[j] = a[j+1];
                 a[j+1] = temp;
             }
         }
     }
 }

void sort_bubble(vector<double>& array, int n) 
{
  // http://www.cplusplus.com/forum/general/127295/
  double swap;
  for (int c = 0 ; c < ( n - 1 ); c++)
  {
    for (int d = 0 ; d < n - c - 1; d++)
    {
      if (array[d] > array[d+1]) /* For decreasing order use < */
      {
        swap       = array[d];
        array[d]   = array[d+1];
        array[d+1] = swap;
      }
    }
  }
}

template <typename T>
void template_sort_bubble_ascending(vector<T>& array, int n) 
{
  // http://www.cplusplus.com/forum/general/127295/
  for (int c = 0 ; c < ( n - 1 ); c++) {
    for (int d = 0 ; d < n - c - 1; d++) {
      // if (array[d] < array[d+1]) /* descending order */
      if (array[d] > array[d+1]) /* ascending order */
      { /* For decreasing order use < */
        T swap       = array[d];
        array[d]   = array[d+1];
        array[d+1] = swap;
      }
    }
  }
}

template <typename T>
void template_sort_bubble_descending(vector<T>& array, int n) 
{
  // http://www.cplusplus.com/forum/general/127295/
  for (int c = 0 ; c < ( n - 1 ); c++) {
    for (int d = 0 ; d < n - c - 1; d++) {
      if (array[d] < array[d+1]) /* descending order */
      // if (array[d] > array[d+1]) /* ascending order */
      { /* For decreasing order use < */
        T swap       = array[d];
        array[d]   = array[d+1];
        array[d+1] = swap;
      }
    }
  }
}

template <typename T>
void template_sort_bubble(vector<T>& array, int n, std::function<bool(T,T)> comp)
{
  // http://www.cplusplus.com/forum/general/127295/
  for (int c = 0 ; c < ( n - 1 ); c++) {
    for (int d = 0 ; d < n - c - 1; d++) {
      if ( comp(array[d], array[d+1]) ) {
        T swap     = array[d];
        array[d]   = array[d+1];
        array[d+1] = swap;
      }
    }
  }
}

int main(int argc, char**argv)
{
  int NUM = 10;
  if(argc > 1) NUM = atoi(argv[1]);

  vector<double> v(NUM);

  // use a basic for loop
  for(int i = 0; i < v.size(); i++)
    v[i] = 1 + random() % NUM;

  // or use a range-based for loop
  for(auto& e : v)
    e = 1 + random() % NUM;

  std::cout << "initial value\n";
  for(auto e : v) std::cout << e << " "; std::cout << "\n";

  sortBubble(v, NUM, ascending);
  std::cout << "after sortBubble(..., ascending)\n";
  for(auto e : v) std::cout << e << " "; std::cout << "\n";

  sortBubble(v, NUM, descending);
  std::cout << "after sortBubble(..., descending)\n";
  for(auto e : v) std::cout << e << " "; std::cout << "\n";

  sort_bubble(v, NUM);
  std::cout << "after sort_bubble\n";
  for(auto e : v) std::cout << e << " "; std::cout << "\n";

  template_sort_bubble_descending(v, NUM);
  std::cout << "after template_sort_bubble_descending\n";
  for(auto e : v) std::cout << e << " "; std::cout << "\n";

  template_sort_bubble_ascending(v, NUM);
  std::cout << "after template_sort_bubble_ascending\n";
  for(auto e : v) std::cout << e << " "; std::cout << "\n";

  std::function<bool(double,double)> comp;
  comp = [] (double a,double b) {return a > b; };
  template_sort_bubble(v, NUM, comp);
  std::cout << "after template_sort_bubble lambda a>b\n";
  for(auto e : v) std::cout << e << " "; std::cout << "\n";

  comp = [] (double a,double b) {return a < b; };
  template_sort_bubble(v, NUM, comp);
  std::cout << "after template_sort_bubble lambda a<b\n";
  for(auto e : v) std::cout << e << " "; std::cout << "\n";


  // try a lambda as an argument 
  // template_sort_bubble(v, NUM, 
    // [] (double a,double b) {return a < b; } );
  // error: no matching function for call to ‘template_sort_bubble(std::vector<double>&, int&, main(int, char**)::<lambda(double, double)>)’
  // [] (double a,double b) {return a < b; } );

  // std::cout << "after template_sort_bubble lambda a<b\n";
  // for(auto e : v) std::cout << e << " "; std::cout << "\n";

}
