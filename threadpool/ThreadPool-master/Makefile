all : example example_2

example : example.cpp ThreadPool.hpp
	g++ -o $@ -std=c++11 -Wall $< -pthread

example_2 : example_2.cpp ThreadPool.hpp
	g++ -o $@ -std=c++11 -Wall $< -pthread
