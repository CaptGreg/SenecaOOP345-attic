################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Render3D.cpp \
../src/algebra.cpp \
../src/callbacks.cpp \
../src/cimage.cpp \
../src/cl_support.cpp \
../src/common_math.cpp \
../src/database.cpp \
../src/files.cpp \
../src/fractal.cpp \
../src/fractparams.cpp \
../src/image.cpp \
../src/interface.cpp \
../src/morph.cpp \
../src/netrender.cpp \
../src/parameters.cpp \
../src/primitives.cpp \
../src/settings.cpp \
../src/shaders.cpp \
../src/texture.cpp \
../src/timeline.cpp \
../src/undo.cpp 

OBJS += \
./src/Render3D.o \
./src/algebra.o \
./src/callbacks.o \
./src/cimage.o \
./src/cl_support.o \
./src/common_math.o \
./src/database.o \
./src/files.o \
./src/fractal.o \
./src/fractparams.o \
./src/image.o \
./src/interface.o \
./src/morph.o \
./src/netrender.o \
./src/parameters.o \
./src/primitives.o \
./src/settings.o \
./src/shaders.o \
./src/texture.o \
./src/timeline.o \
./src/undo.o 

CPP_DEPS += \
./src/Render3D.d \
./src/algebra.d \
./src/callbacks.d \
./src/cimage.d \
./src/cl_support.d \
./src/common_math.d \
./src/database.d \
./src/files.d \
./src/fractal.d \
./src/fractparams.d \
./src/image.d \
./src/interface.d \
./src/morph.d \
./src/netrender.d \
./src/parameters.d \
./src/primitives.d \
./src/settings.d \
./src/shaders.d \
./src/texture.d \
./src/timeline.d \
./src/undo.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/atk-1.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/cairo -I/usr/include/gtk-2.0 -I/usr/include/glib-2.0 -I/usr/include/libpng12 -O3 -msse2 -ffast-math -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CXXFLAGS) $(CPPFLAGS) -o  "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


