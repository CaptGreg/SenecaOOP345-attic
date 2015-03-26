#
# override options for implicit rule(s)
#
# sample Makefile for gcc with POSIX threads
#
# usage examples:
#   "make hello" to compile "hello.c" to "hello" 
#   "make hello++" to compile "hello++.cpp" to "hello++" 
#
CSTD=-std=c99
CXXSTD=-std=c++98
# uncomment if using more-recent gcc
#CSTD=-std=c11
#CXXSTD=-std=c++11

OPT = -O3
CC = gcc
CFLAGS = $(OPT) -Wall -pedantic -pthread $(CSTD)

# untested
CXX = g++
CXXFLAGS = $(OPT) -Wall -pedantic -pthread $(CXXSTD)
