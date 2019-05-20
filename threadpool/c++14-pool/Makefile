# Author:  Greg Blair
# Written: November 22, 2017

CXX      = g++-7
CXXFLAGS = -std=c++17 

# makefile variables
# $@ target
# $^ all dependencies, regardless of file suffx


ALL = test

all : $(ALL)

# test.cpp threadpool.hpp threadsafequeue.hpp

test: test.cpp 
	$(CXX) $(CXXFLAGS) $^ -o $@ -pthread

clean :
	-rm $(ALL)
