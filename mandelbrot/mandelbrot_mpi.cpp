#include <string>
#include <sstream>
#include <iostream>
#include <chrono>

#include <time.h>         // clock_gettime
#include <sys/time.h>     // gettimeofday, getitimer

#include <SDL/SDL.h>

#include <mpi.h>

#define MAX_X 640 // 1280
#define MAX_Y 480 // 1080



static void draw(SDL_Surface* sdlSurface, uint32_t* pixels)
{
  for(uint32_t x = 0; x < MAX_X; x++) {
    for(uint32_t y = 0; y < MAX_Y; y++) {
      ((uint32_t*) sdlSurface->pixels) [ y * MAX_X + x ] = pixels[x * MAX_Y + y]; // transpose!
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

int main(int argc, char* argv[])
{
  std::chrono::time_point<std::chrono::high_resolution_clock> tStart;
  std::chrono::time_point<std::chrono::high_resolution_clock> tStop;
  typedef std::chrono::duration<int,std::milli> millisecs_t ;

  int numprocs, rank, edge, pixel_count, start, end;
  double max_values_sq;
  Uint32 max_iter;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(numprocs <= 1)
  {
    std::cerr << argv[0] << ": error: requires at least two MPI processes\n";
    return 1;
  }
  
  max_values_sq = 4.0;
  max_iter = 5000;

  edge = (MAX_X * MAX_Y) / (numprocs - 1);

  if(rank > 0)
  {
    int tile = rank - 1;

    Uint32* pixels;

    start = tile * edge;
    end = (tile == numprocs - 2) ? MAX_X * MAX_Y : (tile + 1) * edge;
    pixel_count = end - start;

    pixels = new Uint32[pixel_count];
    calc_pixels(start, end, pixels, max_values_sq, max_iter);

    MPI_Send((void*)pixels, pixel_count, MPI_INT, 0, 0, MPI_COMM_WORLD);
    delete [] pixels;
  }
  else /* rank == 0 */
  {

    int tile, recv_count = (edge + 1);
    char title[100];

    Uint32* field = new Uint32[MAX_X * MAX_Y];
    Uint32* fieldpos;

    SDL_Surface* sdlSurface;
    SDL_Event event;
        
    MPI_Status status;

    tStart = std::chrono::high_resolution_clock::now();
    for(tile = 1; tile < numprocs; tile++)
    {
      start = (tile - 1) * edge;
      end = (tile == numprocs - 1) ? MAX_X * MAX_Y : tile * edge;

      pixel_count = end - start;
      recv_count = pixel_count;

      fieldpos = field+start;

      MPI_Recv(fieldpos, recv_count, MPI_INT, tile, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
    tStop = std::chrono::high_resolution_clock::now();
    millisecs_t duration( std::chrono::duration_cast<millisecs_t>(tStop-tStart) ) ;
    long elapsed = duration.count();
    
    SDL_Init(SDL_INIT_EVERYTHING);

    sdlSurface = SDL_SetVideoMode(MAX_X, MAX_Y, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    
    std::stringstream ss;
    ss << argv[0] << " " 
    << numprocs << " processes "
    << elapsed*1.e-3 << " sec."
    << "\n";

    SDL_WM_SetCaption(ss.str().c_str(), title);
    std::cout << ss.str().c_str() << "\n";

    draw(sdlSurface, field);

    SDL_Flip(sdlSurface);
   
    do {
      SDL_Delay(50);
      SDL_PollEvent(&event);
    } while( event.type != SDL_QUIT && event.type != SDL_KEYDOWN );
        
    SDL_FreeSurface(sdlSurface);
    SDL_Quit();

    delete [] field;
  }

  MPI_Finalize();

  return 0;
}
