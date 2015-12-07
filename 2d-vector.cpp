#include <vector>
#include <iostream>
using namespace std;

// const int WIDTH  = 4000;
// const int HEIGHT = 2000;
const int WIDTH  = 2*1920;    // XHD (4K) Video resolution, true 4K is 4096x2160
const int HEIGHT = 2*1080;

vector    <vector<float> >                 vector2D;

int main(int argc, char*argv[])
{
  // Set up sizes. (HEIGHT x WIDTH)
  vector2D.resize(HEIGHT);
  for (int y = 0; y < HEIGHT; ++y)
    vector2D[y].resize(WIDTH);

  for(int y = 0; y < HEIGHT; y++) {
    for(int x = 0; x < WIDTH; x++) {
      vector2D [y][x] = 1e6*y + x;
    }
  }
}
