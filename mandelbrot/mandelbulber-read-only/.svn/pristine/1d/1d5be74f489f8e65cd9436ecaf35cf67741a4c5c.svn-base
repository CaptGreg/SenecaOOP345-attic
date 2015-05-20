// Tiny wrappers around allocated memory to manage lifetime.
#ifndef SMARTPTR_H_
#define SMARTPTR_H_

#include <cstddef>

template <class T> class smart_ptr {
	public:
		smart_ptr(T* ptr = NULL) : ptr_(ptr) {}
		~smart_ptr() { delete ptr_; }
		T* ptr() const { return ptr_; }
		T* operator->() const { return ptr_; }
		T& operator*() const { return *ptr_; }
		void reset(T* ptr = NULL) {
			if (ptr != ptr_) {
				delete ptr_;
				ptr_ = ptr;
			}
		}
	private:
		T* ptr_;
};

template <class T> class smart_array {
	public:
		smart_array(T* ptr = NULL) : ptr_(ptr) {}
		~smart_array() { delete[] ptr_; }
		T& operator[](std::ptrdiff_t i) const { return ptr_[i]; }
		T* ptr() const { return ptr_; }
		void reset(T* ptr = NULL) {
			if (ptr != ptr_) {
				delete[] ptr_;
				ptr_ = ptr;
			}
		}
	private:
		T* ptr_;
};

#endif
