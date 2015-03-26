#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <vector>       // want std::vector, not cl::vector
#include <iostream>
#include <iterator>     // ostream_iterator
// #include <algorithm> // compiles without

using namespace cl;
using namespace std;

int main(int argc, char* argv[]) {
    Context(CL_DEVICE_TYPE_DEFAULT);
    static const unsigned elements = 1000;
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
            c[idx] = a[idx] + b[idx];
        }
    )d", true);

    auto add = make_kernel<Buffer, Buffer, Buffer>(addProg, "add");
    add(EnqueueArgs(elements), a, b, c);

    std::vector<float> result(elements);
    cl::copy(c, begin(result), end(result));

    std::copy(begin(result), end(result), ostream_iterator<float>(cout, ", "));
}
