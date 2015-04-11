// g++ -o hpp -c -g -Wall -I/opt/AMDAPP/include -L/opt/AMDAPP/lib/x86_64 -lOpenCL hpp.cpp
// cl /Fohpp.obj /c hpp.cpp /TP /nologo /EHsc /Zi /RTCs /Fd -DAMD "/IC:\Program Files (x86)\AMD APP\include" "/I C:\Program Files\Windows SDKs\Windows\v6.0AInclude" hpp.cpp
// link /nologo /OUT:hpp.exe "/LIBPATH:C:\Program Files (x86)\AMD APP\lib\x86_64" OpenCL.lib hpp.obj


#include <stdlib.h> // prototype: int main(int argc, char **argv);
#include <cstdio> // req'd for printf
// #include <cstdlib>
#include <iostream>
using namespace std;

#define __CL_ENABLE_EXCEPTIONS
// #define __NO_STD_STRING
// #define __NO_STD_VECTOR
#include <CL/cl.hpp>

const char * helloStr  = "__kernel void hello(void) {   } ";


void
checkErr( const char *func, cl_int err )
{
    if( err != CL_SUCCESS )
    {
        cerr << "C-code checkErr " << func << "(): ";
        switch( err )
        {
        case CL_BUILD_PROGRAM_FAILURE:  cerr << "CL_BUILD_PROGRAM_FAILURE";         break;
        case CL_COMPILER_NOT_AVAILABLE: cerr << "CL_COMPILER_NOT_AVAILABLE";        break;
        case CL_DEVICE_NOT_AVAILABLE:   cerr << "CL_DEVICE_NOT_AVAILABLE";          break;
        case CL_DEVICE_NOT_FOUND:       cerr << "CL_DEVICE_NOT_FOUND";              break;
        case CL_INVALID_BINARY:         cerr << "CL_INVALID_BINARY";                break;
        case CL_INVALID_BUILD_OPTIONS:  cerr << "CL_INVALID_BUILD_OPTIONS";         break;
        case CL_INVALID_CONTEXT:        cerr << "CL_INVALID_CONTEXT";               break;
        case CL_INVALID_DEVICE:         cerr << "CL_INVALID_DEVICE";                break;
        case CL_INVALID_DEVICE_TYPE:    cerr << "CL_INVALID_DEVICE_TYPE";           break;
        case CL_INVALID_OPERATION:      cerr << "CL_INVALID_OPERATION";             break;
        case CL_INVALID_PLATFORM:       cerr << "CL_INVALID_PLATFORM";              break;
        case CL_INVALID_PROGRAM:        cerr << "CL_INVALID_PROGRAM";               break;
        case CL_INVALID_VALUE:          cerr << "CL_INVALID_VALUE";                 break;
        case CL_OUT_OF_HOST_MEMORY:     cerr << "CL_OUT_OF_HOST_MEMORY";            break;
        default:                        cerr << "Unknown error code: " << (int)err; break;
        }
        cerr << "\n";

        exit( err );
    }
}

 
int main(int argc, char **argv)
{
     cl_int err = CL_SUCCESS;
     try {
        vector<cl::Platform>   platforms;
        cl::Platform::get(&platforms);
        cout << "CL_PLATFORM_VENDOR  = " << platforms[0].getInfo<CL_PLATFORM_VENDOR>() << "\n";

        // choose 1st platform
        cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[ 0 ])(), 0 };
        cl::Context context(CL_DEVICE_TYPE_GPU, properties); // or CL_DEVICE_TYPE_CPU;

        vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
        cout << "CL_DEVICE_NAME  = " << devices[0].getInfo<CL_DEVICE_NAME>() << "\n";

        cl::Program::Sources source(1, make_pair(helloStr,strlen(helloStr)));

        cl::Program program = cl::Program(context, source);

        program.build(devices);

        if(0)
        {
            VECTOR_CLASS<size_t> programBinarySizes = program.getInfo<CL_PROGRAM_BINARY_SIZES>();
            VECTOR_CLASS<char*>  programBinaries    = program.getInfo<CL_PROGRAM_BINARIES>();

            cl::Program::Binaries binaries(1, make_pair( (const void*) programBinaries[0], (size_t) programBinarySizes[0] ) );

            program = cl::Program( context, devices, binaries);

            try
            {
                program.build(devices);
                cout << "program.build with binary successful\n";
            }
            catch (cl::Error err) // print build log
            {
                cerr << "program.build with binary failed!\n";
                cerr <<  program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]);
                throw err;
            }
        }

        cl::Kernel kernel(program, "hello", &err);

        cl::CommandQueue cmdQ(context, devices[0], 0, &err);

        cl::Event event;
        cmdQ.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(4,4), cl::NDRange(2,2), NULL, &event);
        event.wait();
        cout << "KERNEL \"" << helloStr << "\" EXECUTION SUCCESSFUL\n";
        if(1)
        {
            // Use the C++ () operator interface to C-code.
            // See Munshi, pg 371.
            // C-code taken from attic/clbinary/clbinaryuse.c
            int                             c_i                      = 0;
            cl_int                          c_err                    = CL_SUCCESS;
            cl_int                          c_nPlatforms             = 0;
            cl_platform_id                  *c_platforms             = NULL;
            cl_platform_id                  c_platform               = (cl_platform_id)NULL;
            cl_context_properties           c_cprops[3];
            cl_context                      c_context;
            size_t                          c_nDevices               = 0;
            cl_device_id                    *c_devices               = NULL;
            cl_device_id                    c_device_id              = 0;
            size_t                          c_binary_size            = 0;
            const unsigned char             *c_binary                = NULL;
            cl_program                      c_program                = NULL;


            {
            VECTOR_CLASS<size_t> programBinarySizes = program.getInfo<CL_PROGRAM_BINARY_SIZES>();
            VECTOR_CLASS<char*>  programBinaries    = program.getInfo<CL_PROGRAM_BINARIES>();

            // use the () operator to access the underlying C-code types
            c_platforms   = &platforms[0]();   // () operator
            c_device_id   = 0;
            c_devices     = &devices[0]();     // () operator
            c_context     = context();         // () operator
            // use binaries set up by compiler invoked by cl.hpp wrapper
            c_binary_size = programBinarySizes[0];
            c_binary      = (const unsigned char *) ((void*) programBinaries[0] );

            /* get a device count for this context. */
            c_err = clGetContextInfo( c_context, CL_CONTEXT_DEVICES, 0, NULL, &c_nDevices );
            checkErr( "clGetContextInfo", c_err );
            c_nDevices = c_nDevices / sizeof(cl_device_id); /* need to generate actual device count from size of required buffer. */
            printf( "C-code Number of devices found: %d\n", (int) c_nDevices );
            c_devices = (cl_device_id *) new cl_device_id[ c_nDevices ];

            /* grab the handles to all of the devices in the context. */
            c_err = clGetContextInfo( c_context, CL_CONTEXT_DEVICES, sizeof(cl_device_id)*c_nDevices, c_devices, NULL );
            checkErr( "clGetContextInfo", c_err );

            // clCreateProgramWithBinary FAILS!!!!!!!!!!!!!!!
            c_program     = clCreateProgramWithBinary( c_context, 1, &c_device_id, &c_binary_size, &c_binary, NULL, &c_err );
            checkErr( "clCreateProgramWithBinary", c_err );

            c_err         = clBuildProgram( c_program, 1, &c_device_id, NULL, NULL, NULL );
            checkErr( "clBuildProgram", c_err );
            }
        }
    }
    catch (cl::Error err) { cerr << "ERROR: " << err.what() << " " << err.err() << "\n"; }

    return EXIT_SUCCESS;
}
