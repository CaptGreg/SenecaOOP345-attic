// web.eecs.utk.edu/~leparker/Courses/CS302-Fall06/Notes/.../priority.cpp

// http://www.google.ca/url?sa=t&rct=j&q=&esrc=s&source=web&cd=5&cad=rja&uact=8&ved=0CD0QFjAE&url=http%3A%2F%2Fweb.eecs.utk.edu%2F~leparker%2FCourses%2FCS302-Fall06%2FNotes%2FPQueues%2Fpriority.cpp&ei=w8g4Vf32NKaIsQS-ooCIAg&usg=AFQjCNEv07UGHSYyOrFkGv2NAmmJjdsiEA&sig2=vj4Spk86mxdbREupq3Y3kg&bvm=bv.91427555,d.cWc

/* STL queue and priority queue examples */

#include <iostream>
#include <queue>

using namespace std;

/* simple queue example */
void functionA()
{
  queue <int> q;                  //q is a queue of integers

  q.push(2);                      //put 2, 5, 3, 2 into the queue
  q.push(5);
  q.push(3);
  q.push(1);
  cout<<"q contains " << q.size() << " elements.\n";

  while (!q.empty()) {
    cout << q.front() << "\n";    //print out the first element in the queue
    q.pop();                      //remove the first element of the queue
  }
}

/* simple priority queue example */
void functionB()
{
  priority_queue <int> pq;      //pq is a priority queue of integers

  pq.push(2);                   //put 2, 5, 3, 1 into the priority queue
  pq.push(5);
  pq.push(3);
  pq.push(1);
  cout<<"pq contains " << pq.size() << " elements.\n";

  while (!pq.empty()) {
    cout << pq.top() << "\n";   //print out the highest priority element
    pq.pop();                   //remove the highest priority element
  }
}

/* example of priority queue where lower integers have higher priority */
void functionC()
{
  priority_queue <int, vector<int>, greater<int> > pq;   //pq is a priority queue of integers that uses
                                                         //a vector of integers for storage and uses >
                                                         //to determine priority. This means that if a > b,
                                                         //a has *lower* priority than b.

  pq.push(2);                      //put 2, 5, 3, 2 into the queue
  pq.push(5);
  pq.push(3);
  pq.push(1);
  cout<<"pq contains " << pq.size() << " elements.\n";

  while (!pq.empty()) {
    cout << pq.top() << "\n";      //print out the highest priority element in the queue
    pq.pop();                      //remove the highest priority element
  }
}

/* define a Height class */
class Height
{
  public:
    Height() {};                                      //default constructor
    Height(int x, int y) { feet = x; inches = y; }    //constructor
    bool operator<(const Height&) const;              //overloaded < operator

    int get_feet() const { return feet; }             //accessor methods
    int get_inches() const { return inches; }

  private:
    int feet, inches;                                 //data fields
};

/* overload the less-than operator so priority queues know how to compare two Height objects */
bool Height::operator<(const Height& right) const
{
  return feet*12 + inches < right.feet*12 + right.inches;
}

/* example of a priority queue using the Height class */
void functionD()
{
  priority_queue <Height> pq;      //pq is a priority queue of Height objects

  Height x;

  x = Height(10,20);               //put 10'20", 11'0", 8'25", and 9'4" into the priority queue
  pq.push(x);
  x = Height(11,0);
  pq.push(x);
  x = Height(8,25);
  pq.push(x);
  x = Height(9,4);
  pq.push(x);

  cout<<"pq contains " << pq.size() << " elements.\n";

  while (!pq.empty()) {
    cout << pq.top().get_feet()                             //print out the feet and inches of the highest
         << "' " << pq.top().get_inches() << "\"" << "\n";  //priority Height object in the priority queue
    pq.pop();                                               //remove the highest priority element
  }
}

int main()
{
  cout << "calling functionA...\n";
  functionA();
  cout << "calling functionB...\n";
  functionB();
  cout << "calling functionC...\n";
  functionC();
  cout << "calling functionD...\n";
  functionD();

  return 0;
}

/* g++ -o priority priority.cpp */


/*****  OUTPUT *****

cphillip:hydra8 ~/test> ./priority
calling functionA...
q contains 4 elements.
2
5
3
1
calling functionB...
pq contains 4 elements.
5
3
2
1
calling functionC...
pq contains 4 elements.
1
2
3
5
calling functionD...
pq contains 4 elements.
10' 20"
11' 0"
8' 25"
9' 4"

*/
