# usage: make filename

CFLAGS = -std=c++11 -Wall 
CXXDFLAGS = -ggdb

C11FLAG = -std=c++11 
CXX = g++
CXX = clang++

ifeq ($(HOSTNAME),matrix)
  # Fall 2014 semester (2014-09-08), matrix g++ is 4.6.2, released October 26, 2011
  CXX = g++
  C11FLAG = -std=c++0x 
  # g++ 4.9
  CXX = /usr/local/gcc/gcc-cilk/bin/g++
  C11FLAG = -std=c++11 
endif

OPTCFLAGS = -fopenmp
CXXFLAGS = -Wall $(C11FLAG) $(CXXDFLAGS) $(OPTCFLAGS)

CXXFLAGSOCL = -I/opt/AMDAPP/include
LFLAGSOCL   = -L/opt/AMDAPP/lib/x86_64 -lamdocl64
OPTLFLAGS   = -lrt -pthread
LFLAGS      = $(OPTLFLAGS)

% : %.cpp Makefile
	$(CXX) $(CXXFLAGS) $< -o $@ $(LFLAGS)

camera : camera.cpp Makefile
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv`   $< `pkg-config --libs opencv` -o $@

canny : canny.cpp Makefile
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv`   $< `pkg-config --libs opencv` -o $@

find_contours : find_contours.cpp Makefile
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv`   $< `pkg-config --libs opencv` -o $@

hough : hough.cpp Makefile
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv`   $< `pkg-config --libs opencv` -o $@

moments : moments.cpp Makefile
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv`   $< `pkg-config --libs opencv` -o $@

ocl_c++11: ocl_c++11.cpp Makefile
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $< $(LFLAGSOCL) -o $@


gl3ctx : gl3ctx.cpp
	$(CXX) gl3ctx.cpp -o gl3ctx -lGL  -lX11 
	./gl3ctx

glVAO_VBO : glVAO_VBO.cpp
	$(CXX) $< -o $@ -lGL -lglut -lGLEW 
	./$@

pthread : pthread.c
	clang -Wall -pthread pthread.c -o pthread

boost-open : boost-open
	$(CXX) $< -o $@ -lboost_iostreams

jsoncpp : jsoncpp.cpp Makefile
	$(CXX) $(CXXFLAGS) `pkg-config --cflags jsoncpp`   $< `pkg-config --libs jsoncpp` -o $@
	#$(CXX) $(CXXFLAGS) $< -o $@ -ljsoncpp



# using clang++ to dump class layout
# http://eli.thegreenplace.net/2012/12/17/dumping-a-c-objects-memory-layout-with-clang/
# The article uses 'clang', not 'clang++'.
# clang -cc1 -fdump-record-layouts myfile.cpp
# or if std headers are needed, run cpp then clang++
# clang -E [your -I flags] myfile.cpp > myfile_pp.i
# clang -cc1 -fdump-record-layouts myfile.i
# (article uses clang -E [your -I flags] myfile.cpp > myfile_pp.cpp 
#  This will never work since the shell truncates myfile.cpp before invoking the compiler)
