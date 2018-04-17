#include <iostream>
#include <fstream>
#include <string>
#include <thread>
using namespace std;

int main(int argc, char*argv[])
{
  if(argc != 4) {
    cerr << "Usage " << argv[0] << ": in out key\n";
    return 1;
  }
  string in  = argv[1];
  string out = argv[2];
  char   key = argv[3][0];

  fstream i(in, ios::in | ios::binary | ios::ate); // ios::ate open AT-THE_END
  if(i.is_open()) {
    streamsize size = i.tellp();
    cout << "file size = " << size << " bytes\n";
    char *b = new char[size];
    i.seekp(0);  // position the file at the start (rewind the file)
    i.read(b, size);
    cout << i.gcount() << " bytes read\n";
    i.close();

 #define THREADS
 #ifndef THREADS
    for(streamsize i = 0; i < size; i++)
      b[i] ^= key;
 
 #else // use threads
    auto doWork = [&b, key] (streamsize start, streamsize end)
    {
      for(streamsize i = start; i < end; i++)
        b[i] ^= key;
    };
    // doWork(0, size); // For testing only.  Don't want to run XOR twice

    // Thread using thread/join
    cout << "\nProblem threaded thread/join void doWork\n";
    int NUM_THREADS = thread::hardware_concurrency();

    thread tid[NUM_THREADS];

    size_t chunk = (size + NUM_THREADS-1) / NUM_THREADS;  // round up
    for(int t = 0; t < NUM_THREADS; t++) {
      streamsize start  = t * chunk;
      streamsize end    = start + chunk;
      if(t == NUM_THREADS -1 )
        end = size;

      tid[t] = thread(doWork, start, end);
    }
    for(auto &e : tid)
      e.join();
   #endif
 
    fstream o(out, ios::out | ios::trunc | ios::binary);
    if(o.is_open()) {
      o.write(b, size);
      if(not o.fail()) 
        cout << size << " bytes written\n";
      o.close();
    } else {
      cerr << "Cannot create file '" << out << "'\n";
    }
    delete [] b;
  } else {
    cerr << "Cannot open file '" << in << "'\n";
  }
}
