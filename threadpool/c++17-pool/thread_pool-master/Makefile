# Author:  Greg Blair
# Written: November 22, 2017

CXX = g++-7
CXXFLAGS = -std=c++17 

# makefile variables
# $@ target
# $^ all dependencies, regardless of file suffx


ALL = test-thread_pool test_yield-thread_pool test-priority_thread_pool test_yield-priority_thread_pool

all : $(ALL)

test-thread_pool : test.cpp  thread_pool.cpp 
	$(CXX) $(CXXFLAGS) $^ -o $@ -pthread

test_yield-thread_pool : test_yield.cpp  thread_pool.cpp 
	$(CXX) $(CXXFLAGS) $^ -o $@ -pthread

test-priority_thread_pool : test.cpp  priority_thread_pool.cpp 
	$(CXX) $(CXXFLAGS) $^ -o $@ -pthread

test_yield-priority_thread_pool : test_yield.cpp  priority_thread_pool.cpp 
	$(CXX) $(CXXFLAGS) $^ -o $@ -pthread

clean :
	-rm $(ALL)
