// GB https://stdcxx.apache.org/doc/stdlibug/11-3.html
// GB code from https://github.com/pathscale/stdcxx

#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>       // random_shuffle used by irand
#include <cstdlib>         // rand

/**************************************************************************
 *
 * icecream.cpp - Priority queue example program.
 *
 * $Id$
 *
 ***************************************************************************
 *
 * Licensed to the Apache Software  Foundation (ASF) under one or more
 * contributor  license agreements.  See  the NOTICE  file distributed
 * with  this  work  for  additional information  regarding  copyright
 * ownership.   The ASF  licenses this  file to  you under  the Apache
 * License, Version  2.0 (the  "License"); you may  not use  this file
 * except in  compliance with the License.   You may obtain  a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the  License is distributed on an  "AS IS" BASIS,
 * WITHOUT  WARRANTIES OR CONDITIONS  OF ANY  KIND, either  express or
 * implied.   See  the License  for  the  specific language  governing
 * permissions and limitations under the License.
 *
 * Copyright 1994-2006 Rogue Wave Software.
 * 
 **************************************************************************/

// #include <examples.h> // GB - not needed.  work-arounds for questionable compilers


// Execution event in a discrete event driven simulation.
class event {
public:
    const unsigned int time;
    event (unsigned int t) : time (t) { } // Constructor sets time of event.

    virtual void processEvent () = 0; // Execute event by invoking this method.
    // bool operator> ( const event* evt_ ) { return time > evt_->time; } // GB not used
};

struct eventComparator {
    bool operator() (const event * left, const event * right) const { return left->time > right->time; }
};


// Framework for discrete event-driven simulations.
class simulation {
protected:
    std::priority_queue<event*,
                        std::vector<event *, std::allocator<event*> >,
                        eventComparator > eventQueue;
public:
    unsigned int time;
    simulation () : time (0), eventQueue () 
        {}
    void  scheduleEvent (event * newEvent) {
        eventQueue.push (newEvent);
    }
    void run () {
      while (! eventQueue.empty ()) {

          event * nextEvent = eventQueue.top ();
          eventQueue.pop ();
          time = nextEvent->time;
          nextEvent->processEvent ();
          delete nextEvent;
      }
    }
};


//  Ice cream store simulation.
class storeSimulation : public simulation {
public:
    storeSimulation () : simulation (), freeChairs (35), profit (0.0)
        { }
    bool canSeat (unsigned int numberOfPeople);
    void order   (unsigned int numberOfScoops);
    void leave   (unsigned int numberOfPeople);
    // Data fields.
    unsigned int freeChairs;
    double       profit;  
} theSimulation;


class arriveEvent : public event {
public:
    arriveEvent (unsigned int t, unsigned int groupSize)
        : event (t), size (groupSize)
        { }
    virtual void processEvent ();
private:
    unsigned int size;
};


class orderEvent : public event {
public:
    orderEvent (unsigned int t, unsigned int groupSize)
        : event (t), size (groupSize)
        { }
    virtual void processEvent ();
private:
    unsigned int size;
};


class leaveEvent : public event
{
public:
    leaveEvent (unsigned int t, unsigned int groupSize)
        : event (t), size (groupSize)
        { }
    virtual void processEvent ();
private:
    unsigned int size;
};


// returns a random integer between 0 and n with n <= 32767 <= INT_MAX
int irand (int n)
{
    static int seq[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf
    };

    std::random_shuffle (seq, seq + sizeof seq / sizeof *seq);

    const int rnd = ((seq [0] << 11) | (seq [1] << 8) | (seq [2] << 4)) + seq [3];

    return rnd % n;
}


void arriveEvent::processEvent () {

    if (theSimulation.canSeat (size))
        theSimulation.scheduleEvent
            (new orderEvent (time + 1 + irand (4), size));
}


void orderEvent::processEvent () {

    // Each person orders some number of scoops.
    for (unsigned int i = 0; i < size; i++)
        theSimulation.order (1 + irand (4));

    // Then we schedule the leave event.
    theSimulation.scheduleEvent
        (new leaveEvent (time + 1 + irand (10), size));
}


void leaveEvent::processEvent () {

    theSimulation.leave (size);
}


// If sufficient room then seat customers.
bool storeSimulation::canSeat (unsigned int numberOfPeople) {

    std::cout << "Time: " << time;
    std::cout << " group of " << numberOfPeople << " customers arrives";

    if (numberOfPeople < freeChairs) {
        std::cout << " is seated\n";
        freeChairs -= numberOfPeople;
        return true;
    }
    else {
        std::cout << " no room, they leave\n";
        return false;
    }
}


// Service icecream, compute profits.
void storeSimulation::order (unsigned int numberOfScoops) {

    std::cout << "Time: " << time << " serviced order for "
              << numberOfScoops << '\n';
    profit += 0.35 * numberOfScoops;
}


// People leave, free up chairs.
void storeSimulation::leave (unsigned int numberOfPeople) {

    std::cout << "Time: " << time << " group of size "
              << numberOfPeople << " leaves\n";
    freeChairs += numberOfPeople;
}


int main () {

    std::cout << "Ice Cream Store simulation from Chapter 9\n";

    // Load queue with some number of initial events.
    for (unsigned t = 0; t < 20; t += irand (6)) {

        std::cout << "pumping queue with event " << t << '\n';
        theSimulation.scheduleEvent (new arriveEvent (t, 1 + irand (4)));
    }

    // Run the simulation.
    theSimulation.run ();

    std::cout << "Total profits " << theSimulation.profit
              << "\nEnd of ice cream store simulation\n";

    return 0;
}
