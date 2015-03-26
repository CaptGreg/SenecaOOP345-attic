#
# override options for implicit rule(s)
#
# sample Makefile for gcc with C++11 threads
#
# first make sure gcc references the newer version 
#
# usage examples:
#   "make hello" to compile "hello.c" to "hello" 
#   "make hello++" to compile "hello++.cpp" to "hello++" 
#
CXXSTD=-std=c++11

OPT = -O3
CXX = g++
CXXFLAGS = $(OPT) -Wall -pedantic -pthread $(CXXSTD)
