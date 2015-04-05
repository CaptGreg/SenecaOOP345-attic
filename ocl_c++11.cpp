// NOTE compiler warnings: clCreateSampler + clCreateCommandQueue deprecated
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <vector>       // want std::vector, not cl::vector
#include <iostream>
#include <exception>
#include <iterator>     // ostream_iterator
// #include <algorithm> // compiles without

using namespace cl;
using namespace std;

int main(int argc, char* argv[]) 
{
  try {
    Context(CL_DEVICE_TYPE_DEFAULT);
    unsigned elements = 10 * 1000 * 1000;
    if(argc  > 1) elements = atoi(argv[1]);
    std::vector<float> data5(elements, 5.5);
    std::vector<float> data6(elements, 3.25);
    Buffer a(begin(data5), end(data5), true, false);
    Buffer b(begin(data6), end(data6), true, false);
    Buffer c(CL_MEM_READ_WRITE, elements * sizeof(float));

    Program addProg(R"d(
        kernel
        void add(   global const float * restrict const a,
                    global const float * restrict const b,
                    global       float * restrict const c) {
            unsigned idx = get_global_id(0);
            c[idx] = a[idx] + b[idx];
        }
    )d", true);

    auto add = make_kernel<Buffer, Buffer, Buffer>(addProg, "add");
    add(EnqueueArgs(elements), a, b, c);

    std::vector<float> result(elements);
    cl::copy(c, begin(result), end(result));

    // creating vectors with millions of entries.  DO NOT PRINT
    // std::copy(begin(result), end(result), ostream_iterator<float>(cout, ", "));
    std::cout<<result[0]; for(int i = 0 ; i < 10 ; i++ ) std::cout<<", "<<result[i]; std::cout<<"\n";
  } catch (const cl::Error& e) {
      std::cerr << "threw cl::Error: " << e.what() << "\n";
  } catch (const std::exception& e) {
      std::cerr << "threw std::exception: " << e.what() << "\n";
  }
}
