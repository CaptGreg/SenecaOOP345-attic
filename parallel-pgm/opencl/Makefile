#
# sample Makefile for gcc with OpenCL (AMD implementation)
#
# usage examples:
#   "make hello" to compile "hello.c" to "hello" 
#   "make hello++" to compile "hello++.cpp" to "hello++" 
#
# note that you must first set up to include AMD files 
#   (command "module load amdapp")

CSTD=-std=c99
CXXSTD=-std=c++98
# uncomment if using more-recent gcc
#CSTD=-std=c11
#CXXSTD=-std=c++11

OPT = -O3
CC = gcc
CXX = g++
OPENCL_CFLAGS = -DAMD_OS_LINUX -I$(AMDAPPSDKROOT)/include
EXTRAS = 
CFLAGS = $(OPT) $(CSTD) -Wall -pedantic $(OPENCL_CFLAGS) $(EXTRAS)
CXXFLAGS = $(OPT) $(CXXSTD) -Wall -pedantic $(OPENCL_CCFLAGS) $(EXTRAS)
OPENCL_LFLAGS = -lOpenCL -L$(AMDAPPSDKROOT)/lib/x86_64
LDFLAGS = $(OPENCL_LFLAGS) -lm

