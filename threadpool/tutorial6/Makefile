all : MutexTest tutorial6

SRC = Consumer.cpp Mutex.cpp        Producer.cpp Thread.cpp tutorial6.cpp
INC = Consumer.h   Mutex.h   Pool.h Producer.h   Thread.h

tutorial6 : $(SRC) $(INC) Makefile
	g++ -std=c++11 $(SRC) -o tutorial6

MutexTest : MutexTest.cpp Mutex.cpp
	g++ -std=c++11 $< -o $@
