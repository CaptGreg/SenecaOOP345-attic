// https://visualstudiomagazine.com/articles/2012/10/19/circular-references/listing4.aspx

#include <memory>

using namespace std;

template <typename T>
class circular_ptr;

// this header only offers the prototype of this template function.
// Implementors of a concrete types T should provide specific versions, and make this
// function friend to T (see class person in Listing 5).
template <typename T>
circular_ptr<T> *get_circular_ptr(const shared_ptr<T>&);

template <typename T>
class circular_ptr {
public:
  circular_ptr() : strong_{nullptr}, weak_{} {}

  // copy constructor
  circular_ptr(shared_ptr<T> &t) : strong_{nullptr}, weak_{} {
    set(t);
  }

  // copy assignment operator
  circular_ptr& operator=(shared_ptr<T> &t) {
    return set(t);
  }

  shared_ptr<T> get() const {
    if (strong_!=nullptr)
      return strong_;
    else
      return weak_.lock();
  }

  // whether circular_ptr uses a weak link to refer to a T instance.
  bool is_weak() {
    return !(weak_.expired());
  }

  // releases its link, whichever it is (strong or weak).
  bool reset() {
    // looks for a weak link that directly or not points to the this circular_ptr
    circular_ptr *weak_one = find_weak_link_reaching_this();

    // if found, means that there was a circular reference. That circular reference is
    // being broken in this reset. Consequently, the weak ptr must become strong.
    if (weak_one) {
        weak_one->strong_ = weak_one->weak_.lock();
        weak_one->weak_.reset();
      }

    strong_.reset();
    weak_.reset();

    return (weak_one);
  }

  bool operator==(const circular_ptr &other) const {
    return get()==other.get();
  }

  bool operator!=(const circular_ptr &other) const {
    return !(*this==other);
  }
protected:
  // this function is O(n) in non-deterministic cases. A deterministic circularity
  // scenario may override this function to get constant order of complexity.
  virtual bool is_this_reachable_from(const shared_ptr<T> &start) const {
    shared_ptr<T> current = start, currents_next;

    // it basically performs a walkthrough from start argument to see whether it reaches
    // this instance or null. If the former, returns true. False otherwise.
    if (current!=nullptr) {
        do {
            currents_next = get_circular_ptr(current)->get();
            if ((currents_next!=nullptr)&&(this==get_circular_ptr(currents_next))) {
                return true;
              }

            current = currents_next;
          }
        while ((current!=nullptr)&&(current!=start));
      }

    return false;
  }

  // same comment: if circularity is deterministic, this function could be overridden and
  // thus optimized
  virtual circular_ptr *find_weak_link_reaching_this() {
    circular_ptr *current = this, *weak_one = nullptr;
    // provided that there's no strong circular reference, either the chain ends in nullptr
    // or we reach a member that is weak
    while (current!=nullptr) {
        if (current->is_weak()) {
            weak_one = current;
            // now, it must be confirmed that *this is reachable from weak_one.
            do {
                if ((current = get_circular_ptr(current->get()))==this)
                  return weak_one;
              }
            while (current!=weak_one);
            // provided that weak_one is reachable from current, as weak_one is weak
            // because participates in a loop.
            break;
          }

        if (shared_ptr<T> t = current->get()) current = get_circular_ptr(t);
        else break;
      }

    return nullptr;
  }
private:
  shared_ptr<T> strong_;
  weak_ptr<T> weak_;

  circular_ptr& set(shared_ptr<T> &t) {
    if (t==nullptr) {
        reset();
      }
    else {
        bool cycle_already_detected = false;

        if (get()!=nullptr) {
            cycle_already_detected = reset();
          }

        // checking for cycle_already_detected helps avoid an extra round
        if ((cycle_already_detected)||( is_this_reachable_from(t)))
          weak_ = t;
        else
          strong_ = t;
      }

    return *this;
  }
};

int main()
{
}
