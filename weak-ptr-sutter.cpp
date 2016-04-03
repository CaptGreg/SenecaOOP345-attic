// http://flyingfrogblog.blogspot.ca/2013/10/herb-sutters-favorite-c-10-liner.html

// a complete reference-counted object cache, or in other words a concurrent weak dictionary, specifically one with weak values.

// GB - not sure this works or is useful.


 #include <iostream>
 #include <map>
 #include <mutex>
 #include <memory>  // shared_ptr/weak_ptr

 using namespace std;

 class widget {
   int id;
 public:
   widget()      :id(-1) { cout << "widget() " << (void*) this << "\n"; }
   widget(int i) :id(i)  { cout << "widget(" << id << ") " << (void*) this << "\n"; }
   ~widget() { cout << "~widget() " << id << "," <<  (void*) this << "\n"; }
 };

// In a recently-posted video, Herb Sutter (a prominent C++ expert) describes his favorite C++ 10-liner as “a thread-safe reference-counted object cache”:
// https://channel9.msdn.com/Events/GoingNative/2013/My-Favorite-Cpp-10-Liner


// isocpp.org

widget& instance() {  // Scott Meyers singleton
  static widget w;
  return w;
}

shared_ptr<widget> load_widget(int id)
{

  // 1. allocate memory as a smart pionter
  shared_ptr<widget> sp(new widget(id));

  // 2. read widget id from backing store 
  // ...

  // 3. return smart pointer
  return sp;
}

shared_ptr<widget> get_widget(int id) {  // Sutter's fav 10 lines of code (auctually 7 lines)
  static map<int, weak_ptr<widget>> cache;  // thread safe static variable
  static mutex m;

  lock_guard<mutex> hold(m);
  auto sp = cache[id].lock();
  if (!sp) cache[id] = sp = load_widget(id);
  return sp;
}


// This example is very interesting. Firstly, it manages to pull in reference counting, weak references and a mutex which are all very rare in modern programming. Secondly, it contains a memory leak that is difficult to fix in C++ because APIs are burdened with memory management details and this API is incapable of expressing deterministic cleanup because there is no facility for a widget's destructor to remove its entry in the map. Finally, the correct name for this data structure is a concurrent weak dictionary, specifically one with weak values. You'll find correct implementations of this data structure are widely available for C#, F# and Java such as the one here.

// The obvious fix is to sweep stale entries from the map when get_widget is called but this leaves floating garbage in the map between calls to get_widget, is asymptotically less efficient and incurs unbounded pauses for an unbounded number of threads.

// Update: Matthew Avery (from the USA) suggests altering the API and semantics of the functions involved so load_widget returns a shared_ptr with a custom deleter that removes the stale map entry as soon as a widget is destructed. If this idea can be made to work then it would be the only deterministic solution to have been proposed to date.


int main()
{
  shared_ptr<widget>  sp;

  for(int i = 10; i < 15; i++) {
    sp = get_widget(i);
    cout << "main:sp=" << i << "," << (void*) sp.get() << "\n";
  }

  cout << "\n";
  for(int i = 10; i < 15; i++) {
    sp = get_widget(i);
    cout << "main:sp=" << i << "," << (void*) sp.get() << "\n";
  }
}
