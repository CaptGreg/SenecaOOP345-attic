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

    cout<<"Putting numbers into the queue"<<endl;
    for(int i=0;i<20;i++){
        int temp=rand();
        cout<<temp<<endl;
        pq.push(temp);
    }

    cout<<endl<<"Reading numbers in the queue"<<endl;
    for(vector<int>::iterator i=tasks.begin();i!=tasks.end();i++)
        cout<<*i<<endl;

    cout<<endl<<"Taking numbers out of the queue"<<endl;
    while(!pq.empty()){
        int temp=pq.top();
        pq.pop();
        cout<<temp<<endl;
    }

    return 0;
}
