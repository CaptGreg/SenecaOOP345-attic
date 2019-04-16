# usage: make filename

# makefile recipe variables
# $@ target
# $^ all dependencies, regardless of file suffx. Include .h files if in dependency list

# g++ error flags
# -fmax-errors=N give up after N errors. Present in GCC 4.6 and later.
# -Wfatal-errors give up after one error. Present in GCC 4.0 and later.

CFLAGS    = -Wall -mmmx -msse -fmax-errors=1 -Wfatal-errors
CC        = gcc
CXXFLAGS  = -std=c++17 -Wall -mmmx -msse -fmax-errors=1 -Wfatal-errors
CXXDFLAGS = -ggdb

C11FLAG   = -std=c++17 
CXX       = clang++
# May 15, 2015, clang++ has missing/corrupt header files: won't compile fastforward, includes fail
# use g++ for fastforward
CXX       = g++

ifeq ($(HOSTNAME),raspberrypi)
  CC      = gcc
  CXX     = g++
endif

ifeq ($(HOSTNAME),amd1100t)
  CC      = gcc
  CXX     = clang++
  CXX     = g++
endif

ifeq ($(HOSTNAME),hp)
  CC      = gcc
  CXX     = g++
  # Ubuntu 16.04 is release 5.3.1
endif

ifeq ($(HOSTNAME),matrix)
  CXX     = g++
  # matrix g++ is 4.6.2, released October 26, 2011
  C11FLAG = std=c++0x 

  CXX     = /usr/local/gcc/7.2.0/bin/g++
  # 'g++ --version' reports g++ (GCC) 7.2.0
  # installed 2017 Sep 1 
  C11FLAG = -std=c++17 
endif

OPTCFLAGS = -fopenmp -fmax-errors=1 -Wfatal-errors 
CXXFLAGS = -Wall $(C11FLAG) $(CXXDFLAGS) $(OPTCFLAGS)

# NOTE Jan 22, 2019 These OpenCL flags compile
CXXFLAGSOCL = `pkg-config --cflags opencl`
LFLAGSOCL   = `pkg-config --libs opencl`

OPTLFLAGS   = -lrt -pthread
LFLAGS      = $(OPTLFLAGS)

% : %.cpp 
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LFLAGS)

# compile fastforward with g++.  Clang++ on matrix has header problems.
# compiles fine on matrix with /usr/local/gcc/gcc-cilk/bin/g++
# fastforward : fastforward.cpp
	# $(CXX) -std=c++0x  $^ -o $@ -pthread

# asan - address sanitization - new/delete error catcher
# needs  -fsanitize=address flag.  Compiles with g++ 4.8 or later.  asan_symbolize is part of clang

asan-global-buffer-overflow : asan-global-buffer-overflow.cpp
	g++ -O -g -fsanitize=address -Wall -std=c++17 $^ -o $@
asan-heap-buffer-overflow : asan-heap-buffer-overflow.cpp
	g++ -O -g -fsanitize=address -Wall -std=c++17 $^ -o $@
asan-heap-use-after-free : asan-heap-use-after-free.cpp
	g++ -O -g -fsanitize=address -Wall -std=c++17 $^ -o $@
asan-stack-buffer-overflow : asan-stack-buffer-overflow.cpp
	g++ -O -g -fsanitize=address -Wall -std=c++17 $^ -o $@

asan : asan-global-buffer-overflow asan-heap-buffer-overflow asan-heap-use-after-free asan-stack-buffer-overflow

camera : camera.cpp
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv`   $^ `pkg-config --libs opencv` -o $@

canny : canny.cpp
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv`   $^ `pkg-config --libs opencv` -o $@

find_contours : find_contours.cpp
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv`   $^ `pkg-config --libs opencv` -o $@

hough : hough.cpp
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv`   $^ `pkg-config --libs opencv` -o $@

moments : moments.cpp
	$(CXX) $(CXXFLAGS) `pkg-config --cflags opencv`   $^ `pkg-config --libs opencv` -o $@

ocl_c++11: ocl_c++11.cpp
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $^ $(LFLAGSOCL) -o $@ 

ocl_header: ocl_header.cpp
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $^ $(LFLAGSOCL) -o $@

ocl_ker_q: ocl_ker_q.cpp
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $^ $(LFLAGSOCL) -o $@

ocldemo: ocldemo.cpp
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $^ $(LFLAGSOCL) -o $@

ocldemo2: ocldemo2.cpp
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $^ $(LFLAGSOCL) -o $@

ocllistdev: ocllistdev.c
	$(CC) $(CXXFLAGS)  $(CXXFLAGSOCL) $^ $(LFLAGSOCL) -o $@

ocl-max-buf: ocl-max-buf.cpp
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $^ $(LFLAGSOCL) -o $@

oclvecadd: oclvecadd.cpp
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $^ $(LFLAGSOCL) -o $@


pathfind : pathfind.cpp
	$(CXX) $(CXXFLAGS)  $^ -o $@ -lGL -lglut -lGLEW 
	./$@

# C++ std::experimental::filesystem
dirtree : dirtree.cpp
	g++-8 $(CXXFLAGS)  $^ -o $@ -lstdc++fs

# C++ std::experimental::filesystem
filesystem : filesystem.cpp
	g++-8 $(CXXFLAGS)  $^ -o $@ -lstdc++fs

# C++ std::experimental::filesystem
filesize : filesize.cpp
	$(CXX) $(CXXFLAGS)  $^ -o $@ -lstdc++fs

# C++ std::experimental::filesystem
stdcopy : stdcopy.cpp
	$(CXX) $(CXXFLAGS)  -Ofast $^ -o $@ -lstdc++fs

gl3ctx : gl3ctx.cpp
	$(CXX) $^ -o $@ -lGL  -lX11 
	./$@ 

glx3 : glx3.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ -lGL  -lX11 
	./$@

glVAO_VBO : glVAO_VBO.cpp
	$(CXX) $(CXXFLAGS)  $^ -o $@ -lGL -lglut -lGLEW 
	./$@

# http://learnopengl.com/code_viewer.php?code=getting-started/shaders-interpolated
glfwShaderInterp : glfwShaderInterp.cpp
	$(CXX) $(CXXFLAGS)  $^ -o $@ -lGL -lGLEW -lglfw
	./$@

#  http://learnopengl.com/#!Getting-started/Shaders
glfwShaderUniform : glfwShaderUniform.cpp
	$(CXX) $(CXXFLAGS)  $^ -o $@ -lGL -lGLEW -lglfw
	./$@

xlib : xlib.cpp  
	$(CXX) $(CXXFLAGS) $^ -o $@ -lX11 
	./$@

Xlib-example : Xlib-example.c  
	$(CC) $(CFLAGS) $^ -o $@ -lX11 
	./$@

#### http://learnopengl.com/code_viewer.php?code=advanced/geometry_shader_explode_shaders
### more complicated: needs "Shader.h" "Camera.h" "Model.h" files, plus the model
### TODO: make code standalone
## glfwShaderGeometry : glfwShaderGeometry.cpp
##	$(CXX) $(CXXFLAGS)  $^ -o $@ -lGL -lGLEW -lglfw
##	./$@Xlib-example

pthread : pthread.c
	clang -Wall -pthread pthread.c -o pthread

boost-open : boost-open
	$(CXX) $^ -o $@ -lboost_iostreams

jsoncpp : jsoncpp.cpp
	$(CXX) $(CXXFLAGS) `pkg-config --cflags jsoncpp`   $^ `pkg-config --libs jsoncpp` -o $@
	#$(CXX) $(CXXFLAGS) $^ -o $@ -ljsoncpp

vectorization : vectorization.cpp
	@echo compiler $(CXX)
	# compiles fine on AMD1100t with Ubuntu 14.04 and g++ 4.9/5.0
	# fails to resolve instrinsics with clang++
	# needs the -mmmx -msse flags to compile instrinsics
	$(CXX) -std=c++17  -ggdb -Ofast -mmmx -msse -msse2  vectorization.cpp -o vectorization && ./vectorization 
	# compile again with auto vectorization turned on
	$(CXX) -std=c++17  -ggdb -Ofast -mmmx -msse -msse2  -ftree-vectorize -ftree-vectorizer-verbose=2 vectorization.cpp -o vectorization && ./vectorization 

# using clang++ to dump class layout
# http://eli.thegreenplace.net/2012/12/17/dumping-a-c-objects-memory-layout-with-clang/
# The article uses 'clang', not 'clang++'.
# clang -cc1 -fdump-record-layouts myfile.cpp
# or if std headers are needed, run cpp then clang++
# clang -E [your -I flags] myfile.cpp > myfile_pp.i
# clang -cc1 -fdump-record-layouts myfile.i
# (article uses clang -E [your -I flags] myfile.cpp > myfile_pp.cpp 
#  This will never work since the shell truncates myfile.cpp before invoking the compiler)

mandelbrot-345a : mandelbrot-345a.cpp
	g++ -std=c++17 -Ofast mandelbrot-345a.cpp -o mandelbrot-345a -ljpeg -ltiff -lpng -pthread

mandelbrot-345b : mandelbrot-345b.cpp
	g++ -std=c++17 -Ofast mandelbrot-345b.cpp tp.cpp -o mandelbrot-345b -ljpeg -ltiff -lpng -pthread

mandelbrot-345aphase2 : mandelbrot-345aphase2.cpp
	g++ -std=c++17 -Ofast mandelbrot-345aphase2.cpp -o mandelbrot-345aphase2 -ljpeg -ltiff -lpng -pthread

mandelbrot-345bphase2 : mandelbrot-345bphase2.cpp
	g++ -std=c++17 -Ofast mandelbrot-345bphase2.cpp -o mandelbrot-345bphase2 -ljpeg -ltiff -lpng -pthread

mandelbrot-345aphase3 : mandelbrot-345aphase3.cpp tp345a.cpp tp345a.h
	g++ -std=c++17 -Ofast mandelbrot-345aphase3.cpp tp345a.cpp -o mandelbrot-345aphase3 -lSDL -ljpeg -ltiff -lpng -pthread

mandelbrot-345bphase3 : mandelbrot-345bphase3.cpp tp345b.cpp tp345b.h
	g++ -std=c++17 -Ofast mandelbrot-345bphase3.cpp tp345b.cpp -o mandelbrot-345bphase3 -lSDL -ljpeg -ltiff -lpng -pthread

mandelbrot-candidate: mandelbrot-candidate.cpp tp.cpp tp.h
	g++ -std=c++17 -Ofast mandelbrot-candidate.cpp tp.cpp -o mandelbrot-candidate -ljpeg -ltiff -lpng -pthread

mandelbrot-candidate-sdl: mandelbrot-candidate-sdl.cpp tp.cpp tp.h
	g++ -std=c++17 -Ofast mandelbrot-candidate-sdl.cpp tp.cpp -o mandelbrot-candidate-sdl -lSDL -ljpeg -ltiff -lpng -pthread 

tp345a: tp345a.cpp tp.h
	g++ -DTEST -Wall -std=c++17 -Ofast tp345a.cpp -o tp345a -pthread 
tp345b: tp345b.cpp tp.h
	g++ -DTEST -Wall -std=c++17 -Ofast tp345b.cpp -o tp345b -pthread 
tp: tp.cpp tp.h
	g++ -DTEST -Wall -std=c++17 -Ofast tp.cpp -o tp -pthread 
	# clang++ -DTEST -Wall -std=c++17 -Ofast tp.cpp -o tp -pthread 

curses-progress: curses-progress.cpp
	g++ -std=c++17 $^ -o $@ -lcurses

curses-box: curses-box.cpp
	g++ -std=c++17 $^ -o $@ -lcurses

# cpp14-auto-lambda.cpp is a C++14 program
cpp14-auto-lambda: cpp14-auto-lambda.cpp
	g++ -Wall -std=c++14 $^ -o $@

cpp14: cpp14.cpp
	g++ -Wall -std=c++14 $^ -o $@

# pimpl.cpp is a C++14 program
pimpl: pimpl.cpp
	g++ -Wall -std=c++14 $^ -o $@

# c++14 threadpool, needs std::index_sequence
tp14: tp14.cpp
	g++ -Wall -std=c++14 $^ -o $@ -pthread

map-reduce: map-reduce.cpp
	g++ -Wall -std=c++17 $^ -o $@ -pthread

# getaddrinfo_a, gai_suspend, gai_error, gai_cancel - asynchronous network address and service translation
getaddrinfo_a: getaddrinfo_a.cpp
	g++ -Wall -std=c++17 $^ -o $@ -lanl

externconstMain : externconstMain.cpp externconst.cpp externconst.h 
	g++ externconstMain.cpp externconst.cpp -o externconstMain

longdouble: longdouble.cpp
	g++ longdouble.cpp -o $@ 
	./$@
	g++ -m32 longdouble.cpp -o $@ 
	./$@
	gcc longdouble.c -o $@ 
	./$@
	gcc -m32 longdouble.c -o $@ 
	./$@
	rm $@

references-are-pointers : references-are-pointers.cpp
	# are references pointers?
	g++ -Wa,-adhln -g references-are-pointers.cpp -o references-are-pointers > references-are-pointers.s
	# ABSOLUTELY!
	# the assembler generated for a function expecting a pointer and a function expecting a reference is a exactly the same.

