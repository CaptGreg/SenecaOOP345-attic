#include <string>
#include <sstream>
#include <iostream>
#include <chrono>

#include <time.h>         // clock_gettime
#include <sys/time.h>     // gettimeofday, getitimer

#include <SDL/SDL.h>

#include <iostream>
#include <assert.h>
using namespace std;

#define __CL_ENABLE_EXCEPTIONS

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp> // system (factory) copy
#else
// #include "CL/cl.hpp"    // private cl.hpp (Khronos 2011-06-15 download) 
#include <CL/cl.hpp> // system (factory) copy installed by graphics card vendor (ATI_STREAM, AMD_APP, NVIDIA)
#endif

#define MAX_X 640 // 1280
#define MAX_Y 480 // 1024

#define SIZE (MAX_X * MAX_Y)
struct globals {
  bool bUseGPU;
	Uint32 pixels[SIZE];
} g;

void Usage(char *pgm)
{
    std::cout << "usage: " << pgm << "s [-c] [-g]\n";
    std::cout << ("optional arguments:\n");
    std::cout << ("   -c                  ==> use CPU\n");
    std::cout << ("   -g                  ==> use GPU\n");
}


// Helper function to get error string
// *********************************************************************
const char* oclErrorString(cl_int error)
{
    static const char* errorString[] = {
        "CL_SUCCESS",
        "CL_DEVICE_NOT_FOUND",
        "CL_DEVICE_NOT_AVAILABLE",
        "CL_COMPILER_NOT_AVAILABLE",
        "CL_MEM_OBJECT_ALLOCATION_FAILURE",
        "CL_OUT_OF_RESOURCES",
        "CL_OUT_OF_HOST_MEMORY",
        "CL_PROFILING_INFO_NOT_AVAILABLE",
        "CL_MEM_COPY_OVERLAP",
        "CL_IMAGE_FORMAT_MISMATCH",
        "CL_IMAGE_FORMAT_NOT_SUPPORTED",           // #10
        "CL_BUILD_PROGRAM_FAILURE",
        "CL_MAP_FAILURE",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",                                        // #20
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "CL_INVALID_VALUE",                        // #30
        "CL_INVALID_DEVICE_TYPE",
        "CL_INVALID_PLATFORM",
        "CL_INVALID_DEVICE",
        "CL_INVALID_CONTEXT",
        "CL_INVALID_QUEUE_PROPERTIES",
        "CL_INVALID_COMMAND_QUEUE",
        "CL_INVALID_HOST_PTR",
        "CL_INVALID_MEM_OBJECT",
        "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
        "CL_INVALID_IMAGE_SIZE",                  // #40
        "CL_INVALID_SAMPLER",
        "CL_INVALID_BINARY",
        "CL_INVALID_BUILD_OPTIONS",
        "CL_INVALID_PROGRAM",
        "CL_INVALID_PROGRAM_EXECUTABLE",
        "CL_INVALID_KERNEL_NAME",
        "CL_INVALID_KERNEL_DEFINITION",
        "CL_INVALID_KERNEL",
        "CL_INVALID_ARG_INDEX",
        "CL_INVALID_ARG_VALUE",                  // #50
        "CL_INVALID_ARG_SIZE",
        "CL_INVALID_KERNEL_ARGS",
        "CL_INVALID_WORK_DIMENSION",
        "CL_INVALID_WORK_GROUP_SIZE",
        "CL_INVALID_WORK_ITEM_SIZE",
        "CL_INVALID_GLOBAL_OFFSET",
        "CL_INVALID_EVENT_WAIT_LIST",
        "CL_INVALID_EVENT",
        "CL_INVALID_OPERATION",
        "CL_INVALID_GL_OBJECT",                 // #60
        "CL_INVALID_BUFFER_SIZE",
        "CL_INVALID_MIP_LEVEL",
        "CL_INVALID_GLOBAL_WORK_SIZE",
    };

    const int errorCount = sizeof(errorString) / sizeof(errorString[0]);

    const int index = -error;

    return (index >= 0 && index < errorCount) ? errorString[index] : "";

}




static void draw_pixel(SDL_Surface* sdlSurface, Uint32 x, Uint32 y, Uint32 color)
{
  Uint32* pixel;

  pixel = (Uint32*)sdlSurface->pixels + y * MAX_X + x;
  *pixel = color;
}

static void draw(SDL_Surface* sdlSurface, Uint32* pixels)
{
  Uint32 i, j;

  for(i = 0; i < MAX_X; i++)
  {
    for(j = 0; j < MAX_Y; j++)
    {
      draw_pixel(sdlSurface, i, j, pixels[i * MAX_Y + j]);
    }
  }
}

static Uint32 mandelbrot_point(double cx, double cy, double max_value_sq,
                               Uint32 max_iter)
{
  double value_sq = 0;
  double x = 0, xt;
  double y = 0, yt;
  Uint32 iter = 0;
  
  while((value_sq <= max_value_sq) && (iter < max_iter))
  {
    xt = (x * x) - (y * y) + cx;
    yt = 2 * x * y + cy;
    x = xt;
    y = yt;
    iter++;
    value_sq = x * x + y * y;
  }

  return iter;
}

static void calc_pixels(Uint32 start, Uint32 end, Uint32* pixels,
                       double max_values_sq, Uint32 max_iter)
{
  Uint32 i, iter, icolor;
  double cx, cy;

  double pd_x = 3.0 / (double)MAX_X;
  double pd_y = 2.0 / (double)MAX_Y;

  for(i = start; i < end; i++)
  {
    cx = -2.0 + (i / MAX_Y) * pd_x;
    cy = -1.0 + (i % MAX_Y) * pd_y;

    iter= mandelbrot_point(cx, cy, max_values_sq, max_iter);

    icolor = (double)iter/ (double)max_iter * (1u << 24);
    pixels[i-start] = icolor;
  }
}

int calc_pixels_cl(Uint32 start, Uint32 end, Uint32* pixels,
                       double max_values_sq, Uint32 max_iter)
{
   cl_int err = CL_SUCCESS;
    int errorCount = 0;
    const char *kernelName = "mandelbrot";
    const char * clSrc = 
    "__kernel void mandelbrot(void)"
    "{ }";                                  // works

   std::vector<cl::Platform>   platforms;
   std::vector<cl::Device>     devices;
   cl::Program                 program;

   cl_device_type deviceType = CL_DEVICE_TYPE_CPU;

  deviceType = ::g.bUseGPU ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU;

   try {

     cl::Platform::get(&platforms);
     if (platforms.size() == 0)
         throw "OpenCL not on this machine";

     int count = 0;
     const char *want    =               "AMD Accelerated Parallel Processing";
     int found = -1;
     std::cout << "Number of platforms:\t " << platforms.size() << std::endl;
     for (std::vector<cl::Platform>::iterator i = platforms.begin(); i != platforms.end(); ++i) {
          // pick a platform and do something
           if(strcmp(want, (*i).getInfo<CL_PLATFORM_NAME>().c_str() ) == 0) 
               found = count;
           count++;
     }     
     if(-1 == found)
      throw "cannot find OpenCL platform";

     cl_context_properties properties[] = 
        { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[found])(), 0};
     cl::Context context(deviceType, properties); 
 
     devices = context.getInfo<CL_CONTEXT_DEVICES>();
 
     cl::Program::Sources source(1, std::make_pair(clSrc, strlen(clSrc)));
     program = cl::Program(context, source);
     const char *options = "-I .";
     program.build(devices, options);
 
     cl::Kernel kernel(program, kernelName, &err);
 
     cl::CommandQueue queue(context, devices[0], 0, &err);

     cl::Buffer pixelBuf(context, CL_MEM_WRITE_ONLY, sizeof(::g.pixels));

     kernel.setArg(0, pixelBuf);

     // C++ enqueueNDRangeKernel method blocks by default
     // cl::Event event;
     queue.enqueueNDRangeKernel(
         kernel, 
         cl::NullRange,                 // offset
         cl::NDRange(SIZE),             // global
         cl::NullRange                  // local
         // , NULL                         // list of events to complete before this kernel executes
         // , &event                       // this kernel event
     );
     // event.wait();

     queue.enqueueReadBuffer(pixelBuf, CL_TRUE, 0, SIZE * sizeof(::g.pixels[0]), (void *) ::g.pixels);

   } catch (cl::Error err) {
      std::cerr << "ERROR: " << err.what() << "(" << err.err() << ")" << std::endl;
      if( CL_BUILD_PROGRAM_FAILURE == err.err() )
      {
         std::cout << "Build log ouput:\n";
         std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]); 
      }
   } catch (const char *errMsg) {
      std::cout << "error = " << errMsg << "\n";
   }
 
  return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
  std::chrono::time_point<std::chrono::high_resolution_clock> tStart;
  std::chrono::time_point<std::chrono::high_resolution_clock> tStop;
  typedef std::chrono::duration<int,std::milli> millisecs_t ;

  int  ret          = -1;
  int  errorCount   = 0;
  ::g.bUseGPU = false;
  for(int i = 1; i < argc; i++) {
    if(*argv[i] == '-') {
      switch(*(argv[i] + 1) ) {
      case 'c':   ::g.bUseGPU = false;     break;
      case 'g':   ::g.bUseGPU = true;      break;
      default:    Usage(argv[0]); exit(0); break;
      }
    } else {
      Usage(argv[0]);
      exit(0);
    }
  }

  int pixel_count, start, end;
  double max_values_sq;
  Uint32 max_iter;
  char title[100];

  SDL_Surface* sdlSurface;
  SDL_Event event;

  max_values_sq = 4.0;
  max_iter = 5000;

  calc_pixels_cl(start = 0, end= MAX_X * MAX_Y, ::g.pixels, max_values_sq, max_iter);
  tStop = std::chrono::high_resolution_clock::now();
  millisecs_t duration( std::chrono::duration_cast<millisecs_t>(tStop-tStart) ) ;
  long elapsed = duration.count();

  SDL_Init(SDL_INIT_EVERYTHING);

  sdlSurface = SDL_SetVideoMode(MAX_X, MAX_Y, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

  std::stringstream ss;
  ss << argv[0] << " " << elapsed*1.e-3;
  SDL_WM_SetCaption(ss.str().c_str(), title);
  std::cout << ss.str().c_str() << "\n";

  draw(sdlSurface, ::g.pixels);

  SDL_Flip(sdlSurface);

  do {
    SDL_Delay(50);
    SDL_PollEvent(&event);
  } while( event.type != SDL_QUIT && event.type != SDL_KEYDOWN );

  SDL_FreeSurface(sdlSurface);
  SDL_Quit();

  return 0;
}
