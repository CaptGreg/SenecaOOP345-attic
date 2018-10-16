#define __CL_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 200
#include <CL/cl2.hpp>

#include <iostream>

using namespace cl;
using namespace std;

int main(int argc, char* argv[]) {
  try { 
    // Context(CL_DEVICE_TYPE_DEFAULT);  // works
    // Context(CL_DEVICE_TYPE_CPU);      // works
    Context(CL_DEVICE_TYPE_GPU);         // works
    static const unsigned elements = 20;
    std::vector<float> data(elements, 5);
    Buffer a(begin(data), end(data), true, false);
    Buffer b(begin(data), end(data), true, false);
    Buffer c(CL_MEM_READ_WRITE, elements * sizeof(float));

    Program addProg(R"d(
        kernel
        void add(   global const float * restrict const a,
                    global const float * restrict const b,
                    global       float * restrict const c) {
            unsigned idx = get_global_id(0);
            c[idx] = 2*a[idx] + b[idx];
        }
    )d", true);

#if CL_HPP_TARGET_OPENCL_VERSION >= 200
    auto add = compatibility::make_kernel<Buffer, Buffer, Buffer>(addProg, "add"); // ocl 2.0 comatibility with 1.x
    // cl2.hpp says "Please use KernelFunctor directly."
#else
    auto add = make_kernel<Buffer, Buffer, Buffer>(addProg, "add"); // deprecated in ocl 2.0
#endif
    add(EnqueueArgs(elements), a, b, c);

    std::vector<float> result(elements);
    cl::copy(c, begin(result), end(result));

    std::copy(begin(result), end(result), ostream_iterator<float>(cout, ", "));
  } catch(cl::Error e) {
    cerr << "(" << e.err() << ") - " << e.what() << "\n";
  }
}
