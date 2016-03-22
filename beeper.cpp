// beeper.cpp uses new/delete.
// beepersp.cpp uses shared_ptr.

#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>  // sort

using namespace std;

///////////////////////////////
// Simulation framework code //
///////////////////////////////

#define USE_VECTOR
// #define USE_PRIORITY_QUEUE

class Event
{
public:
   int time;                    // event time
   Event(int t) : time(t)  {}   // set event time to supplied parameter
   virtual void   action() = 0; // a derived class must define action()
   virtual        ~Event() {}   // virtual destructor - call derived class destructors first
   bool operator< (const Event& rhs) const { return time < rhs.time; }
};

// We need a queue with entries sorted by time.
// Sorted queues exist in STL.  They are STL priority_queues.  
// Priority_queues by design maintain the queue in a sorted state.
// Priority_queues are mentioned in the course notes.
// See http://www.cplusplus.com/reference/queue/priority_queue/ 
// for more info on priority_queues.

// The simulation needs to extract from the event queue the event with the 
// nearest time.  The queue is sorted on time.  The next event to be executed 
// is the event at the top of the queue. It is the event with the nearest time.

// Create an EventQueue class which provides the priority_queue services.
// We can implement the details either inheriting from a std::priority_queue or
// building a DIY priority_queue inheriting from a std::vector.  The underlying 
// details can be switched a priority_queue to our DIY priority_queue.

// This isolates the application from what-ever is used as the base for sorted queues.

// A DIY priority_queue can be built using vector as a base class.
// The vector must be sorted after every insert.
// Popping from the front requires all other entries to move up one position.
// Pop from the back.  It is efficient.  No moves are required.
// A descending sort will place the smallest time event at the back of the vector.
// Create a EventQueue class that wraps our DIY std::vector based priority_queue.

// Vectors are used extensively in the course. Priority_queues are mentioned but never used.

#ifdef USE_VECTOR
// Create a EventQueue derived class inheriting from a std::vector base class.
class EventQueue : public std::vector<Event*> { // a DIY priority_queue
public:
  void   pop() { pop_back(); }     // smallest value at the end of the vector
  Event* top() { return back(); }
  void   push(Event* e)
  { 
    push_back(e);
    // Sorting a list of pointers is fast.
    sort(begin(), end());  // uses Event::operator< for compare function
  }
};
#endif

#ifdef USE_PRIORITY_QUEUE
// Create a EventQueue derived class inheriting from a std::priority_queue base class.
class EventQueue : public std::priority_queue<Event*, std::vector<Event*> > 
{
  // Nothing to do. Absolutely everything is inherited from the std::priority_queue base class.
  // NOTE: uses Event::operator< for compare
};
#endif

class Simulation 
{
  EventQueue eventQ;
public:
  void scheduleEvent(Event* event) 
  { 
    eventQ.push(event);  // queues uses push
  }
  void print()
  {
  #ifdef USE_VECTOR
    // priority_queues do not support iterators
    for(auto it = eventQ.begin(); it < eventQ.end(); it++) {
      cout << (*it)->time << "\n";
    }
  #endif
  }
  void run()
  {
    while(!eventQ.empty()) {
      Event* event = eventQ.top(); //  queues uses top, pop
      eventQ.pop();
      event->action();
      delete event;
    }
  }
};

//////////////////////////////////////
// End of simulation framework code //
//////////////////////////////////////


////////////////////////////////////////
// Application simulation starts here //
////////////////////////////////////////

Simulation beeper;

class BeeperOn : public Event                    // defines BeeperOn
{
public:
  BeeperOn(int t) : Event(t) {
    // cout << "BeeperOn(" << time << ")\n"; 
  }
  void action();
  ~BeeperOn()   { 
    // cout << "~BeeperOn(), time=" << time << "\n"; 
  }
};

class BeeperOff : public Event                    // defines BeeperOff
{
public:
  BeeperOff(int t) : Event(t) {
    // cout << "BeeperOff(" << time << ")\n"; 
  }
  void action();
  ~BeeperOff()   { 
     // cout << "~BeeperOff(), time=" << time << "\n"; 
   }
};

// Now BeeperOn::action and BeeperOff::action can reference each other

void BeeperOn::action()
{
   cout << "BeeperOn::action: ON at " << time << "\n";
   beeper.scheduleEvent(new BeeperOff(time + 2)); // BeeperOff circular reference
}

void BeeperOff::action()
{
   cout << "BeeperOff::action: OFF at " << time << "\n";
   if(time < 30)
     beeper.scheduleEvent(new BeeperOn(time + 1)); // BeeperOn circular reference
}

int main()
{
   // Start the simulation at time=8.
   // That is schedule the first event at time=8.
   beeper.scheduleEvent(new BeeperOn(8));
   // beeper.scheduleEvent(new BeeperOn(2));
   // beeper.scheduleEvent(new BeeperOn(1));
   // beeper.print();
   beeper.run();
   return 0;
}
