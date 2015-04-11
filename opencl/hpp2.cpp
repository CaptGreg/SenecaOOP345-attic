// g++ -o hpp -c -g -Wall -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86_64 -lOpenCL hpp.cpp
// cl /Fohpp.obj /c hpp.cpp /TP /nologo /EHsc /Zi /RTCs /Fd -DAMD "/IC:\Program Files (x86)\AMD APP\include" "/I C:\Program Files\Windows SDKs\Windows\v6.0AInclude" hpp.cpp
// link /nologo /OUT:hpp.exe "/LIBPATH:C:\Program Files (x86)\AMD APP\lib\x86_64" OpenCL.lib hpp.obj


// #include <stdlib.h> // prototype: int main(int argc, char **argv);
#include <cstdlib>
#include <iostream>
using namespace std;

#define __CL_ENABLE_EXCEPTIONS
// #define __NO_STD_STRING
#define __NO_STD_VECTOR
#include <CL/cl.hpp>

const char * helloStr  = "__kernel void hello(void) {   } ";
 
int main(int argc, char **argv)
{
    cout << "\n";
     cl_int err = CL_SUCCESS;
     try {
        cl::vector<cl::Platform>   platforms;
        cl::Platform::get(&platforms);
        cout << argv[0] << " CL_PLATFORM_VENDOR             = " << platforms[0].getInfo<CL_PLATFORM_VENDOR>() << "\n";
        // choose 1st platform
        cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[ 0 ])(), 0 };
        cl::Context context(CL_DEVICE_TYPE_GPU, properties); // or CL_DEVICE_TYPE_CPU;

        cl::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
        cout << argv[0] << " CL_DEVICE_VENDOR               = " << devices[0].getInfo <CL_DEVICE_VENDOR>            () << "\n";
        cout << argv[0] << " CL_DEVICE_NAME                 = " << devices[0].getInfo <CL_DEVICE_NAME>              () << "\n";
        cout << argv[0] << " CL_DEVICE_PROFILE              = " << devices[0].getInfo <CL_DEVICE_PROFILE>           () << "\n";
        cout << argv[0] << " CL_DEVICE_VERSION              = " << devices[0].getInfo <CL_DEVICE_VERSION>           () << "\n";
        cout << argv[0] << " CL_DEVICE_MAX_WORK_GROUP_SIZE  = " << devices[0].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << "\n";


        cl::Program::Sources source(1, make_pair(helloStr,strlen(helloStr)));
        cl::Program program = cl::Program(context, source);
        program.build(devices);
        cl::Kernel kernelFromSource(program, "hello", &err);
        cl::CommandQueue cmdQ(context, devices[0], 0, &err);
        cl::Event event;
        cmdQ.enqueueNDRangeKernel(kernelFromSource, cl::NullRange, cl::NDRange(4,4), cl::NDRange(2,2), NULL, &event);
        event.wait();
        cout << argv[0] << " KERNEL source compile \"" << helloStr << "\" EXECUTION SUCCESSFUL\n";

        cout << argv[0] << " source compile program works - now grab binary and compile binary\n";
        VECTOR_CLASS<size_t> programBinarySizes = program.getInfo<CL_PROGRAM_BINARY_SIZES>();
        cout << argv[0] << " binary size = " << programBinarySizes[0] << " bytes\n";
        VECTOR_CLASS<char*>  programBinaries    = program.getInfo<CL_PROGRAM_BINARIES>();

        cl::Program::Binaries binaries(1, make_pair( (const void*) programBinaries[0], (size_t) programBinarySizes[0] ) );
        program = cl::Program( context, devices, binaries);
        try
        {
            program.build(devices);
            cout << argv[0] << " program.build with binary successful\n";
        }
        catch (cl::Error err) // print build log
        {
            cerr << argv[0] << " program.build with binary failed!\n";
            cerr << argv[0] << " " <<  program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
            throw err;
        }

        cl::Kernel kernelFromBinary(program, "hello", &err);

        cmdQ.enqueueNDRangeKernel(kernelFromBinary, cl::NullRange, cl::NDRange(4,4), cl::NDRange(2,2), NULL, &event);
        event.wait();
        cout << argv[0] << " KERNEL binary compile \"" << helloStr << "\" EXECUTION SUCCESSFUL\n";
    }
    catch (cl::Error err) { cerr << argv[0] << " ERROR: " << err.what() << " " << err.err() << "\n"; }

    return EXIT_SUCCESS;
}
