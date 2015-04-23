// http://en.cppreference.com/w/cpp/container/priority_queue

#include <functional>
#include <queue>
#include <vector>
#include <iostream>
 
template<typename T> void print_queue(T& q) {
    while(!q.empty()) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}
 
int main() {
    std::priority_queue<int> q;
 
    for(int n : {1,8,5,6,3,4,0,9,3,2})
        q.push(n);
 
    print_queue(q);

    // std::cout << q[2] << "\n"; 
    // error: type 'std::priority_queue<int>' does not provide a subscript operator

    // for( auto e : q ) std::cout << e; std::cout << "\n";
    // error: invalid range expression of type 'std::priority_queue<int, std::vector<int, std::allocator<int> >, std::less<int> >'; 
    //        no viable 'begin' function available
 
    std::priority_queue<int, std::vector<int>, std::greater<int> > q2;
 
    for(int n : {1,8,5,6,3,4,0,9,3,2})
        q2.push(n);
 
    print_queue(q2);
}

/***************** 
Output:

9 8 6 5 4 3 3 2 1 0 
0 1 2 3 3 4 5 6 8 9

*******************/
