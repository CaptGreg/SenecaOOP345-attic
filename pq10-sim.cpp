// http://www.math.ucla.edu/~wittman/10a.1.10w/ccc/ch23/index.html

// GB priority_queue simulation example

// 23.5.1 Priority Queue - Example

    // Event loop - the heart of the simulation
    // It pulls the next event (smallest time) from the PQ
    // Events are removed in sequence, regardless of insertion
    // As each event is removed it is executed, and the memory recovered
    // The loop runs until the the queue is exhausted

// event.h 

#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <queue>
#include <iostream>
#include <cstdlib>     // GB rand

using std::priority_queue;
using std::vector;

inline int rand_int(int min, int max) { return rand() % (max-min) + min; } // GB

/**
   A single event for a discrete event driven simulation.
*/
class Event 
{
public:
   Event(int t);

   /**
      Perform one event in the simulation.
   */
   virtual void do_event() = 0;

protected:
   friend class EventComparison;
   int time;
};

/**
   Compare two events based on their time.
*/
class EventComparison 
{
public:
   bool operator()(const Event* left, const Event* right) const;
};

inline Event::Event(int t) : time(t) {}

inline bool EventComparison::operator()
   (const Event* left, const Event* right) const
{
   return left->time > right->time;
}

/**
   Simulation framework for event driven simulation.
*/
class Simulation 
{
public:
   /**
      Add new event to simulation.
      @param new_event the event to add
   */
   void schedule_event(Event* new_event);

   /**
      Run the simulation through all events.
   */
   void run();
private:
   priority_queue<Event*, vector<Event*>, EventComparison> event_queue;
};

inline void Simulation::schedule_event(Event* new_event)
{ 
   event_queue.push(new_event); 
}

#endif

// event.cpp

// #include "event.h"
using namespace std;
void Simulation::run()
{
  while(!event_queue.empty()) {
    Event* next_event = event_queue.top();
    event_queue.pop();
    next_event->do_event();
    delete next_event;
  }
}

/**
   Simulation of a hot dog stand with limited seating
*/
class HotDogStand : public Simulation 
{
public:
   HotDogStand(int s);
   /**
      Test to see if new customer can be seated.
      @return true if customer is seated
   */
   bool can_seat();

   /**
      Satisfied customer leaves, having eaten.
   */
   void customer_leaves();
private:
   int free_seats;
};

HotDogStand::HotDogStand(int s) : free_seats(s) {}

bool HotDogStand::can_seat()
{
   if (free_seats > 0) {
      free_seats--;
      return true;
   } else
      return false;
}

void HotDogStand::customer_leaves() 
{
   free_seats++;
}

HotDogStand freds(3);

/**
   Arrival event for simulation.
   Either customer is seated, or leaves without eating.
*/
class ArriveEvent : public Event 
{
public:
   ArriveEvent(int t);
   virtual void do_event();
};

/**
   Leave event for simulation.
   Satisfied customer leaves and releases seat.
*/
class LeaveEvent : public Event 
{
public:
   LeaveEvent(int t);
   virtual void do_event();
};

ArriveEvent::ArriveEvent(int t) : Event(t) {}

void ArriveEvent::do_event()
{
   if (freds.can_seat()) {
      cout << "time " << time << " Customer is seated\n";
      freds.schedule_event(new LeaveEvent(time + rand_int(1, 5)));
   }
   else
      cout << "time " << time 
         << " Customer is unable to find a seat, leaves\n";
}

LeaveEvent::LeaveEvent(int t) : Event(t) {}

void LeaveEvent::do_event()
{
   cout << "time " << time << " Customer finishes eating, leaves\n";
   freds.customer_leaves();
}

int main()
{
   for (int i = 0; i < 50; i++)
      freds.schedule_event(new ArriveEvent(rand_int(1, 60)));
   freds.run();
   return 0;
}
