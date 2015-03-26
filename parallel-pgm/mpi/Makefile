#
# sample Makefile for MPI (OpenMPI implementation)
#
# first make sure mpicc references an MPI compiler 
#
# usage examples:
#   "make hello" to compile "hello.c" to "hello" 
#   "make hello++" to compile "hello++.cpp" to "hello++"
#

CSTD=-std=c99
CXXSTD=-std=c++98 -Wno-long-long
# uncomment if using more-recent gcc
#CSTD=-std=c11
#CXXSTD=-std=c++11 -Wno-deprecated-declarations

OPT = -O3
CC = mpicc
CFLAGS = $(OPT) -Wall -pedantic $(CSTD)

CXX = mpicxx
CCFLAGS = $(OPT) -Wall $(CXXSTD)

