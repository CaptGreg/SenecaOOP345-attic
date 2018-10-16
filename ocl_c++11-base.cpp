#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <iostream>

using namespace cl;
using namespace std;

int main(int argc, char* argv[]) {
  try { 
    Context(CL_DEVICE_TYPE_DEFAULT);
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

    auto add = make_kernel<Buffer, Buffer, Buffer>(addProg, "add"); // deprecated in ocl 2.0
    add(EnqueueArgs(elements), a, b, c);

    std::vector<float> result(elements);
    cl::copy(c, begin(result), end(result));

    std::copy(begin(result), end(result), ostream_iterator<float>(cout, ", "));
  } catch(cl::Error e) {
    cerr << "(" << e.err() << ") - " << e.what() << "\n";
  }
}
