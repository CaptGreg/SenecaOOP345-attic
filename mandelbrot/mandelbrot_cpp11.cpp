#include <string>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>

#include <SDL/SDL.h>

#define MAX_X 640 // 1280
#define MAX_Y 480 // 1024

static void draw(SDL_Surface* sdlSurface, uint32_t* pixels)
{
  for(uint32_t x = 0; x < MAX_X; x++) {
    for(uint32_t y = 0; y < MAX_Y; y++) {
      ((uint32_t*) sdlSurface->pixels) [ y * MAX_X + x ] = pixels[x * MAX_Y + y]; // transpose!
    }
  }
}

static uint32_t mandelbrot_point(double cx, double cy, double max_value_sq,
                               uint32_t max_iter)
{
  double value_sq = 0;
  double x = 0, xt;
  double y = 0, yt;
  uint32_t iter = 0;
  
  while((value_sq <= max_value_sq) && (iter < max_iter)) {
    xt = (x * x) - (y * y) + cx;
    yt = 2 * x * y + cy;
    x = xt;
    y = yt;
    iter++;
    value_sq = x * x + y * y;
  }

  return iter;
}

static void calc_pixels(uint32_t start, uint32_t end, uint32_t* pixels,
                       double max_values_sq, uint32_t max_iter)
{
  uint32_t i, iter, icolor;
  double cx, cy;

  double pd_x = 3.0 / (double)MAX_X;
  double pd_y = 2.0 / (double)MAX_Y;

  for(i = start; i < end; i++) {
    cx = -2.0 + (i / MAX_Y) * pd_x;
    cy = -1.0 + (i % MAX_Y) * pd_y;

    iter = mandelbrot_point(cx, cy, max_values_sq, max_iter);

    icolor = (double)iter/ (double)max_iter * (1u << 24);
    pixels[i-start] = icolor;
  }
}

static void calc_pixels_with_threads(uint32_t start, uint32_t end, uint32_t* pixels,
                       double max_values_sq, uint32_t max_iter)
{
  const uint32_t NUM_THREADS = 100;

  std::thread t[NUM_THREADS];

  uint32_t pixel_count = end - start;
  uint32_t chunk = pixel_count / NUM_THREADS;

  uint32_t s, e; // start,end working variables
  s = 0;
  for(int thr = 0; thr < NUM_THREADS; thr++) {
    e = s + chunk;
    if(thr == NUM_THREADS-1) e = end;
    t[thr] = std::thread(calc_pixels, s, e, pixels+s, max_values_sq, max_iter);
    s += chunk;
  }
  // for(int tid = 0; tid < NUM_THREADS; tid++) t[tid].join();
  for( auto& e : t ) e.join();
}

int main(int argc, char* argv[])
{
  std::chrono::time_point<std::chrono::high_resolution_clock> tStart;
  std::chrono::time_point<std::chrono::high_resolution_clock> tStop;
  typedef std::chrono::duration<int,std::milli> millisecs_t ;

  uint32_t start, end;
  double max_values_sq;
  uint32_t max_iter;
  char title[100];
  uint32_t* pixels = new uint32_t[MAX_X * MAX_Y];

  if(pixels) {
    SDL_Surface* sdlSurface;
    SDL_Event event;

    max_values_sq = 4.0;
    max_iter = 5000;

    tStart = std::chrono::high_resolution_clock::now();
    start = 0, end= MAX_X * MAX_Y;
    calc_pixels_with_threads(start, end, pixels+start, max_values_sq, max_iter);
    tStop = std::chrono::high_resolution_clock::now();
    millisecs_t duration( std::chrono::duration_cast<millisecs_t>(tStop-tStart) ) ;
    long elapsed = duration.count();

    SDL_Init(SDL_INIT_EVERYTHING);

    sdlSurface = SDL_SetVideoMode(MAX_X, MAX_Y, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    std::stringstream ss;
    ss << argv[0] << " " << elapsed*1.e-3 << " sec.";
    SDL_WM_SetCaption(ss.str().c_str(), title);
    std::cout << ss.str().c_str() << "\n";

    draw(sdlSurface, pixels);

    SDL_Flip(sdlSurface);

    do {
      SDL_Delay(50);
      SDL_PollEvent(&event);
    } while( event.type != SDL_QUIT && event.type != SDL_KEYDOWN );

    SDL_FreeSurface(sdlSurface);
    SDL_Quit();

    delete [] pixels;
  } else {
    std::cout << "no memory\n";
  }

  return 0;
}
