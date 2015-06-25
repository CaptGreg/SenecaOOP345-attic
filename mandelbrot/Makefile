ALL = mandelbrot mandelbrot_cpp11 mandelbrot_mp mandelbrot_mpi  mandelbrot_cl mandelbrot_cpp11

all: $(ALL)

CXX = g++
CXX = clang++

AMDAPP=/opt/AMDAPP

#include $(AMDAPP)/make/openclsdkdefs.mk 

CXXFLAGS   = $(CFLAGSOPENCV)  -I/opt/AMDAPP/include -Ofast -std=c++11 -fopenmp -Ofast

LFLAGS    = $(LFLAGSOPENCV) -lOpenCL -lSDL

mandelbrot: mandelbrot.cpp Makefile
	$(CXX) $(CXXFLAGS) $@.cpp -o $@ $(LFLAGS)

mandelbrot_cpp11: mandelbrot_cpp11.cpp Makefile
	$(CXX) $(CXXFLAGS) $@.cpp -o $@ $(LFLAGS)

mandelbrot_mp: mandelbrot_mp.cpp Makefile
	$(CXX) $(CXXFLAGS) $@.cpp -o $@ $(LFLAGS)

mandelbrot_mpi: mandelbrot_mpi.cpp Makefile
	mpicxx.openmpi $(CXXFLAGS) mandelbrot_mpi.cpp -o mandelbrot_mpi $(LFLAGS)

mandelbrot_cl: mandelbrot_cl.cpp Makefile
	$(CXX) $(CXXFLAGS) $@.cpp -o $@ $(LFLAGS)


run:	mandelbrot mandelbrot_mp mandelbrot_mpi mandelbrot_cl mandelbrot_cpp11
	./mandelbrot
	./mandelbrot_cpp11
	./mandelbrot_mp
	mpirun.openmpi -n 12 ./mandelbrot_mpi
	mpirun.openmpi -n 10 ./mandelbrot_mpi
	mpirun.openmpi -n 8 ./mandelbrot_mpi
	mpirun.openmpi -n 6 ./mandelbrot_mpi
	mpirun.openmpi -n 4 ./mandelbrot_mpi
	./mandelbrot_cl

clean:
	-rm $(ALL)
