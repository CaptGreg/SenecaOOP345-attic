// http://stackoverflow.com/questions/4484767/how-to-iterate-over-a-priority-queue

#include <queue>
#include <cstdlib>
#include <iostream>
using namespace std;

template <class T, class S, class C>
S& Container(priority_queue<T, S, C>& q) {
    struct HackedQueue : private priority_queue<T, S, C> {
        static S& Container(priority_queue<T, S, C>& q) {
            return q.*&HackedQueue::c;
        }
    };
    return HackedQueue::Container(q);
}

int main()
{
    priority_queue<int> pq;
    vector<int> &tasks = Container(pq);

    cout<<"Putting numbers into the queue"<<"\n";
    for(int i=0;i<20;i++){
        int temp=rand();
        cout<<temp<<"\n";
        pq.push(temp);
    }

    cout<<"\n"<<"Reading numbers in the queue"<<"\n";
    for(vector<int>::iterator i=tasks.begin();i!=tasks.end();i++)
        cout<<*i<<"\n";

    cout<<"\n"<<"Taking numbers out of the queue"<<"\n";
    while(!pq.empty()){
        int temp=pq.top();
        pq.pop();
        cout<<temp<<"\n";
    }

    return 0;
}
