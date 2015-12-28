# usage: make filename

# -fmax-errors=N give up after N errors. Present in GCC 4.6 and later.
# -Wfatal-errors give up after one error. Present in GCC 4.0 and later.

CFLAGS = -Wall -mmmx -msse -fmax-errors=1 -Wfatal-errors
CXXFLAGS = -std=c++11 -Wall -mmmx -msse -fmax-errors=1 -Wfatal-errors
CXXDFLAGS = -ggdb

C11FLAG  = -std=c++11 
CXX      = g++-5
CXX5     = g++-5
# May 15, 2015, clang++ has missing/corrupt header files: won't compile fastforward, includes fail
# use g++ for fastforward
CXX      = clang++
CXX      = g++

ifeq ($(HOSTNAME),raspberrypi)
  CC   = gcc
  CXX  = g++
endif

ifeq ($(HOSTNAME),raspberrypi)
  CC   = gcc
  CXX  = g++
endif

ifeq ($(HOSTNAME),amd1100t)
  CC   = gcc-5
  CXX  = g++
  CXX  = clang++
  CXX  = g++-5
  CC5  = gcc-5
  CXX5 = g++-5
endif

ifeq ($(HOSTNAME),hp)
  CC   = gcc-5
  CXX  = g++-5
endif

ifeq ($(HOSTNAME),matrix)
  # Fall 2014 semester (2014-09-08), matrix g++ is 4.6.2, released October 26, 2011
  CXX = g++
  C11FLAG = -std=c++0x 
  # g++ 4.9
  CXX = /usr/local/gcc/gcc-cilk/bin/g++
  C11FLAG = -std=c++11 
endif

OPTCFLAGS = -fopenmp -fmax-errors=1 -Wfatal-errors 
CXXFLAGS = -Wall $(C11FLAG) $(CXXDFLAGS) $(OPTCFLAGS)

# NOTE AMDAPP Beta link paths are different from standard AMDAPP
AMDAPP      = AMDAPPSDK-3.0-0-Beta
AMDAPP      = AMDAPP
CXXFLAGSOCL = -I/opt/$(AMDAPP)/include
LFLAGSOCL   = -L/opt/$(AMDAPP)/lib/x86_64 -lamdocl64

# NOTE April 19, 2015 These OpenCL flags compile
CXXFLAGSOCL = 
CXXFLAGSOCL = -I/opt/$(AMDAPP)/include
LFLAGSOCL   = -lOpenCL

# AMDAPP v 2.x at least linked and ran
# AMDAPP      = AMDAPP
# CXXFLAGSOCL = -I/opt/$(AMDAPP)/include
# LFLAGSOCL   = -L/opt/$(AMDAPP)/lib/x86_64 -lamdocl64

OPTLFLAGS   = -lrt -pthread
LFLAGS      = $(OPTLFLAGS)

% : %.cpp Makefile
	$(CXX) $(CXXFLAGS) $< -o $@ $(LFLAGS)

# compile fastforward with g++.  Clang++ on matrix has header problems.
# compiles fine on matrix with /usr/local/gcc/gcc-cilk/bin/g++
# fastforward : fastforward.cpp Makefile
	# $(CXX) -std=c++0x  $< -o $@ -pthread


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

ocl_header: ocl_header.cpp Makefile
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $< $(LFLAGSOCL) -o $@

ocl_ker_q: ocl_ker_q.cpp Makefile
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $< $(LFLAGSOCL) -o $@

ocldemo2: ocldemo2.cpp Makefile
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $< $(LFLAGSOCL) -o $@

oclvecadd: oclvecadd.cpp Makefile
	$(CXX) $(CXXFLAGS)  $(CXXFLAGSOCL) $< $(LFLAGSOCL) -o $@


gl3ctx : gl3ctx.cpp
	$(CXX) gl3ctx.cpp -o gl3ctx -lGL  -lX11 
	./gl3ctx

glVAO_VBO : glVAO_VBO.cpp
	$(CXX) $(CXXFLAGS)  $< -o $@ -lGL -lglut -lGLEW 
	./$@

# http://learnopengl.com/code_viewer.php?code=getting-started/shaders-interpolated
glfwShaderInterp : glfwShaderInterp.cpp
	$(CXX) $(CXXFLAGS)  $< -o $@ -lGL -lGLEW -lglfw
	./$@

#  http://learnopengl.com/#!Getting-started/Shaders
glfwShaderUniform : glfwShaderUniform.cpp
	$(CXX) $(CXXFLAGS)  $< -o $@ -lGL -lGLEW -lglfw
	./$@

#### http://learnopengl.com/code_viewer.php?code=advanced/geometry_shader_explode_shaders
### more complicated: needs "Shader.h" "Camera.h" "Model.h" files, plus the model
### TODO: make code standalone
## glfwShaderGeometry : glfwShaderGeometry.cpp
##	$(CXX) $(CXXFLAGS)  $< -o $@ -lGL -lGLEW -lglfw
##	./$@

pthread : pthread.c
	clang -Wall -pthread pthread.c -o pthread

boost-open : boost-open
	$(CXX) $< -o $@ -lboost_iostreams

jsoncpp : jsoncpp.cpp Makefile
	$(CXX) $(CXXFLAGS) `pkg-config --cflags jsoncpp`   $< `pkg-config --libs jsoncpp` -o $@
	#$(CXX) $(CXXFLAGS) $< -o $@ -ljsoncpp

vectorization : vectorization.cpp Makefile
	@echo compiler $(CXX)
	# compiles fine on AMD1100t with Ubuntu 14.04 and g++ 4.9/5.0
	# fails to resolve instrinsics with clang++
	# needs the -mmmx -msse flags to compile instrinsics
	$(CXX) -std=c++11  -ggdb -Ofast -mmmx -msse -msse2  vectorization.cpp -o vectorization && ./vectorization 
	# compile again with auto vectorization turned on
	$(CXX) -std=c++11  -ggdb -Ofast -mmmx -msse -msse2  -ftree-vectorize -ftree-vectorizer-verbose=2 vectorization.cpp -o vectorization && ./vectorization 

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
	g++ -std=c++11 -Ofast mandelbrot-345a.cpp -o mandelbrot-345a -ljpeg -ltiff -lpng -pthread

mandelbrot-345b : mandelbrot-345b.cpp
	g++ -std=c++11 -Ofast mandelbrot-345b.cpp tp.cpp -o mandelbrot-345b -ljpeg -ltiff -lpng -pthread

mandelbrot-345aphase2 : mandelbrot-345aphase2.cpp
	g++ -std=c++11 -Ofast mandelbrot-345aphase2.cpp -o mandelbrot-345aphase2 -ljpeg -ltiff -lpng -pthread

mandelbrot-345bphase2 : mandelbrot-345bphase2.cpp
	g++ -std=c++11 -Ofast mandelbrot-345bphase2.cpp -o mandelbrot-345bphase2 -ljpeg -ltiff -lpng -pthread

mandelbrot-345aphase3 : mandelbrot-345aphase3.cpp tp345a.cpp tp345a.h
	g++ -std=c++11 -Ofast mandelbrot-345aphase3.cpp tp345a.cpp -o mandelbrot-345aphase3 -lSDL -ljpeg -ltiff -lpng -pthread

mandelbrot-345bphase3 : mandelbrot-345bphase3.cpp tp345b.cpp tp345b.h
	g++ -std=c++11 -Ofast mandelbrot-345bphase3.cpp tp345b.cpp -o mandelbrot-345bphase3 -lSDL -ljpeg -ltiff -lpng -pthread

mandelbrot-candidate: mandelbrot-candidate.cpp tp.cpp tp.h
	g++ -std=c++11 -Ofast mandelbrot-candidate.cpp tp.cpp -o mandelbrot-candidate -ljpeg -ltiff -lpng -pthread

mandelbrot-candidate-sdl: mandelbrot-candidate-sdl.cpp tp.cpp tp.h
	g++ -std=c++11 -Ofast mandelbrot-candidate-sdl.cpp tp.cpp -o mandelbrot-candidate-sdl -lSDL -ljpeg -ltiff -lpng -pthread 

tp345a: tp345a.cpp tp.h
	g++ -DTEST -Wall -std=c++11 -Ofast tp345a.cpp -o tp345a -pthread 
tp345b: tp345b.cpp tp.h
	g++ -DTEST -Wall -std=c++11 -Ofast tp345b.cpp -o tp345b -pthread 
tp: tp.cpp tp.h
	g++ -DTEST -Wall -std=c++11 -Ofast tp.cpp -o tp -pthread 
	# clang++ -DTEST -Wall -std=c++11 -Ofast tp.cpp -o tp -pthread 
