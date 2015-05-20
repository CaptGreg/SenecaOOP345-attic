all : example

example : example.cpp ctpl_stl.h Makefile
	g++ -std=c++11 example.cpp -o example -pthread

test :  example
	./example 
