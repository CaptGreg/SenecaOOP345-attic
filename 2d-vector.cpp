#include <vector>
#include <iostream>
#include <memory>   // unique_ptr
using namespace std;

const int WIDTH  = 2*1920;    // XHD (4K) 3840x2160 Video resolution, true 4K is 4096x2160
const int HEIGHT = 2*1080;

int main(int argc, char*argv[])
{
  {  // 2d array using std::vector
     vector <vector<float> >  table;

    // Set up sizes. (HEIGHT x WIDTH)
    table.resize(HEIGHT);
    for (int y = 0; y < HEIGHT; ++y)
      table[y].resize(WIDTH);

    for(int y = 0; y < HEIGHT; y++) {
      for(int x = 0; x < WIDTH; x++) {
        table [y][x] = 1e6*y + x;
      }
    }
    // auto access = [&] (int x, int y) { return ref(table[y][x]); };
    // float& access = [&] (int x, int y) { return table[y][x]; };
    #define ACCESS(X, Y) table[Y][X]
    for(int y = 0; y < HEIGHT; y++) {
      for(int x = 0; x < WIDTH; x++) {
        ACCESS(x,y) = 1e6*y + x;
      }
    }
  }

  { // map linear array to 2d array
    float *table = new float[WIDTH * HEIGHT];

    for(int y = 0; y < HEIGHT ; y++ )
      for(int x = 0; x < WIDTH; x++)
        table[y * WIDTH + x] = 1e6 * y + x;

    delete [] table;
  }

  { // map linear smart point array to 2d array 
  std::unique_ptr< float > table( new float[WIDTH*HEIGHT] );
  for(int y = 0; y < HEIGHT ; y++ )
    for(int x = 0; x < WIDTH; x++)
      *(table.get() + y * WIDTH + x) = 1e6 * y + x;
  } // smart pointer out of scope (automatically deleted)

  { // map linear [] smart point array to 2d array 
  std::unique_ptr< float[] > table( new float[WIDTH*HEIGHT] ); // setup for unique_ptr::operator[]
  for(int y = 0; y < HEIGHT ; y++ )
    for(int x = 0; x < WIDTH; x++)
      table[y * WIDTH + x] = 1e6 * y + x;  // operator[] same as get()[i]
  } // smart pointer out of scope (automatically deleted)

  { // 2d-array row table: (HEIGHT+1) alloc/dealloc calls
    float **rowTable = new float*[ HEIGHT ];
    for(int y =0; y < HEIGHT; y++)
      rowTable[y] = new float[WIDTH];

    for(int y = 0; y < HEIGHT ; y++ )
      for(int x = 0; x < WIDTH; x++)
        rowTable [y] [x] = 1e6 * y + x;

    for(int y =0; y < HEIGHT; y++)
      delete [] rowTable[y];
    delete [] rowTable; 
  }

  { // rowtable+data allocated in one alloc/dealloc pair of calls
    unsigned char *table = new unsigned char[  HEIGHT * sizeof(float*) +   WIDTH * HEIGHT * sizeof(float) ];

    float** rowTable = (float**)table;
    float*  data     = (float*) (table + HEIGHT * sizeof(float*));

    for(int y =0; y < HEIGHT; y++)
      rowTable[y] = data + y * WIDTH;

    for(int y = 0; y < HEIGHT ; y++ )
      for(int x = 0; x < WIDTH; x++)
        rowTable [y] [x] = 1e6 * y + x;

    delete [] table; 
  }

  { // rowtable+data allocated in one smart pointer
    unique_ptr<unsigned char> table( new unsigned char[ HEIGHT * sizeof(float*) +  WIDTH * HEIGHT * sizeof(float) ] );

    float** rowTable = (float**)  table.get();
    float*  data     = (float*)  (table.get() + HEIGHT * sizeof(float*));

    for(int y =0; y < HEIGHT; y++)
      rowTable[y] = data + y * WIDTH;

    for(int y = 0; y < HEIGHT ; y++ )
      for(int x = 0; x < WIDTH; x++)
        rowTable [y] [x] = 1e6 * y + x;
  } // unique_ptr 'table' deleted when out of scope

  { // 3d pointer
    int WIDTH  = 3;
    int HEIGHT = 3;
    int DEPTH  = 3;

    float (*data)[DEPTH][HEIGHT][WIDTH] = (float(*)[DEPTH][HEIGHT][WIDTH]) new float[DEPTH*HEIGHT*WIDTH]; 

    for(int z = 0; z < DEPTH ; z++ ) {
      for(int y = 0; y < HEIGHT ; y++ ) {
        for(int x = 0; x < WIDTH; x++) {
          (* data ) [z] [y] [x] = 1e2 * z + y +  1e-2 * x;
        }
      }
    }

    for(int z = 0; z < DEPTH ; z++ ) {
      for(int y = 0; y < HEIGHT ; y++ ) {
        for(int x = 0; x < WIDTH; x++) {
          cout << "[" << z << "][" << y << "][" << x << "]="
             << (* data ) [z] [y] [x] 
             << "\n";
        }
      }
    }

    delete [] data;
  }

} // end main
