// GB TODO: test --- not sure this is working code

// http://stackoverflow.com/questions/4484767/how-to-iterate-over-a-priority-queue

// I had the same problem, where I wanted to iterate over a priority queue without 
// dequeuing (hence destroying my queue). I made it work for me by recasting my 
// priority_queue pointer to a pointer to a vector (as my priority_queue uses 
// vector as its container). Here is how it looks like:

#include <iostream>       // std::cout
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater

class PriorityQueue {
  private:
    class Element {
      //Comparator function
      bool operator()(const Element* e1, const Element* e2) const {
          // Smallest deadline value has highest priority.
          return e1->x > e2->x;
      }
    public: // GB --- factory code had syntax error because x was private
      int x; 
    };

    std::priority_queue<Element*, std::vector<Element*>, Element> pq;

  public:
    PriorityQueue();
    ~PriorityQueue();
    //print the all the elements of the queue
    void print_queue_elements() {
        std::vector<PriorityQueue::Element*> *queue_vector;

        //recast the priority queue to vector
        queue_vector = reinterpret_cast<std::vector<PriorityQueue::Element*> *>(&pq);
        for(std::vector<PriorityQueue::Element*>::iterator it = (*queue_vector).begin(); it != (*queue_vector).end(); it++) {
            std::cout << "My Element " << (*it)->x << "\n";
            //other processing with queue elements
        }
    }
 };

int main()
{
}
