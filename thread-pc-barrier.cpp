// GB barier approach to single-producer, single-consumer queue
// GB this code doesn't work.
// GB try to make it work

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <future>
#include <iostream>


// https://duckduckgo.com/?q=c%2B%2B11++barriers&t=canonical&atb=v85-5&ia=qa&iax=qa
class BarrierX {
public:
    explicit BarrierX(size_t iCount) {
        mThreshold  = iCount;
        mCount      = iCount;
        mGeneration = 0;
    }

    void Wait() {
    // asm volatile("": : :"memory"); // doesn't lock
        std::unique_lock<std::mutex> lLock{m};
        auto lGen = mGeneration;
        if (!--mCount) {
            mGeneration++;
            mCount = mThreshold;
            cv.notify_all();
        } else {
            cv.wait(lLock, [this, lGen] { return lGen != mGeneration; });
        }
    }

private:
    std::mutex m;
    std::condition_variable cv;
    size_t mThreshold;
    size_t mCount;
    size_t mGeneration;
};

// https://www.daniweb.com/programming/software-development/threads/498822/c-11-thread-equivalent-of-pthread-barrier
#if 0 // looks similar to other Barrier class
class Barrier
{
public:
  Barrier(const Barrier&) = delete;
  Barrier& operator=(const Barrier&) = delete;
  explicit Barrier(unsigned int count) :
    m_count(count), m_generation(0), 
    m_count_reset_value(count)
  {
  }
  void Wait()
  {
    std::unique_lock< std::mutex > lock(m);
    unsigned int gen = m_generation;
    if (--m_count == 0)
    {
      m_generation++;
      m_count = m_count_reset_value;
      cv.notify_all();
      return;
    }
    while (gen == m_generation)
      cv.wait(lock);
  }
private:
  std::mutex m;
  std::condition_variable cv;
  unsigned int m_count;
  unsigned int m_generation;
  unsigned int m_count_reset_value;
};
#endif

class Barrier // a spinlock_barrier
{
public:
  Barrier(const Barrier&) = delete;
  Barrier& operator=(const Barrier&) = delete;
  explicit Barrier(unsigned int count) :
    m_count(count), 
    m_generation(0), 
    m_count_reset_value(count)
  {
  }
  void Wait()
  {
    unsigned int gen = m_generation.load();
    if (--m_count == 0)
    {
      if (m_generation.compare_exchange_weak(gen, gen + 1))
      {
        m_count = m_count_reset_value;
      }
      return;
    }
    while ((gen == m_generation) && (m_count != 0))
      std::this_thread::yield();
  }
private:
  std::atomic<unsigned int> m_count;
  std::atomic<unsigned int> m_generation;
  unsigned int m_count_reset_value;
};
// https://software.intel.com/en-us/articles/single-producer-single-consumer-queue/
/*

Unbounded single-producer/single-consumer node-based queue. Internal non-reducible cache of nodes is used. Dequeue operation is always wait-free. Enqueue operation is wait-free in common case (when there is available node in the cache), otherwise enqueue operation calls ::operator new(), so probably not wait-free. No atomic RMW operations nor heavy memory fences are used, i.e. enqueue and dequeue operations issue just several plain loads, several plain stores and one conditional branching. Cache-conscious data layout is used, so producer and consumer can work simultaneously causing no cache-coherence traffic.

Single-producer/single-consumer queue can be used for communication with thread which services hardware device (wait-free property is required), or when there are naturally only one producer and one consumer. Also N single-producer/single-consumer queues can be used to construct multi-producer/single-consumer queue, or N^2 queues can be used to construct fully-connected system of N threads (other partially-connected topologies are also possible).

Hardware platform: x86-32/64

Compiler: Intel C++ Compiler

*/
Barrier barrier(2);
// load with 'consume' (data-dependent) memory ordering
template<typename T>
T load_consume(T const* addr)
{
  // hardware fence is implicit on x86
  T v = *const_cast<T const volatile*>(addr);
  // GB __memory_barrier(); // compiler fence
  // error: there are no arguments to ‘__memory_barrier’ that depend on a template parameter, 
  // so a declaration of ‘__memory_barrier’ must be available [-fpermissive]
  barrier.Wait();

  return v;
}

// store with 'release' memory ordering
template<typename T>
void store_release(T* addr, T v)
{
  // hardware fence is implicit on x86
  // GB __memory_barrier(); // compiler fence
  // error: there are no arguments to ‘__memory_barrier’ that depend on a template parameter, 
  // so a declaration of ‘__memory_barrier’ must be available [-fpermissive]
  barrier.Wait();

  *const_cast<T volatile*>(addr) = v;
}

// cache line size on modern x86 processors (in bytes)
size_t const cache_line_size = 64;

// single-producer/single-consumer queue
template<typename T>
class spsc_queue
{
public:
  spsc_queue()
  {
      node* n = new node;
      n->next_ = 0;
      tail_ = head_ = first_= tail_copy_ = n;
  }

  ~spsc_queue()
  {
      node* n = first_;
      do
      {
          node* next = n->next_;
          delete n;
          n = next;
      }
      while (n);
  }

  void enqueue(T v)
  {
      node* n = alloc_node();
      n->next_ = 0;
      n->value_ = v;
      store_release(&head_->next_, n);
      head_ = n;
  }

  // returns 'false' if queue is empty
  bool dequeue(T& v)
  {
      if (load_consume(&tail_->next_))
      {
          v = tail_->next_->value_;
          store_release(&tail_, tail_->next_);
          return true;
      }
      else
      {
          return false;
      }
  }

private:
  // internal node structure
  struct node
  {
      node* next_;
      T value_;
  };

  // consumer part
  // accessed mainly by consumer, infrequently be producer
  node* tail_; // tail of the queue

  // delimiter between consumer part and producer part,
  // so that they situated on different cache lines
  char cache_line_pad_ [cache_line_size];

  // producer part
  // accessed only by producer
  node* head_; // head of the queue
  node* first_; // last unused node (tail of node cache)
  node* tail_copy_; // helper (points somewhere between first_ and tail_)

  node* alloc_node()
  {
      // first tries to allocate node from internal node cache,
      // if attempt fails, allocates node via ::operator new()

      if (first_ != tail_copy_)
      {
          node* n = first_;
          first_ = first_->next_;
          return n;
      }
      tail_copy_ = load_consume(&tail_);
      if (first_ != tail_copy_)
      {
          node* n = first_;
          first_ = first_->next_;
          return n;
      }
      node* n = new node;
      return n;
  }

  spsc_queue(spsc_queue const&);
  spsc_queue& operator = (spsc_queue const&);
};

// usage example
int main()
{
  spsc_queue<int> q;
  std::cout << "enqueue 1\n";
  q.enqueue(1);
  q.enqueue(1);

  std::cout << "enqueue 2\n";
  q.enqueue(2);

  {
  int v;
  bool b = q.dequeue(v);
  b = q.dequeue(v);
  std::cout << "dequeued " << v << "\n";
  }

  std::cout << "enqueue 3\n";
  q.enqueue(3);

  std::cout << "enqueue 4\n";
  q.enqueue(4);

  b = q.dequeue(v);
  std::cout << "dequeued " << v << "\n";

  b = q.dequeue(v);
  std::cout << "dequeued " << v << "\n";

  b = q.dequeue(v);
  std::cout << "dequeued " << v << "\n";
}

