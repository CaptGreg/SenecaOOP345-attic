// g++ -std=c++11 cl-hpp.cpp -o cl-hpp -lOpenCL ; ./cl-hpp -c ; ./cl-hpp -g ; rm cl-offset

#include <stdlib.h> // prototype: int main(int argc, char **argv);
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <assert.h>
using namespace std;

#define __CL_ENABLE_EXCEPTIONS

#if defined(__APPLE__) || defined(__MACOSX)
#include <OpenCL/cl.hpp> // system (factory) copy
#else
// #include "CL/cl.hpp"    // private cl.hpp (Khronos 2011-06-15 download) 
#include <CL/cl.hpp> // system (factory) copy installed by graphics card vendor (ATI_STREAM, AMD_APP, NVIDIA)
#endif

// Helper function to get error string
// *********************************************************************
const char* oclErrorString(cl_int error)
{
    static const char* errorString[] = {
        "CL_SUCCESS",
        "CL_DEVICE_NOT_FOUND",
        "CL_DEVICE_NOT_AVAILABLE",
        "CL_COMPILER_NOT_AVAILABLE",
        "CL_MEM_OBJECT_ALLOCATION_FAILURE",
        "CL_OUT_OF_RESOURCES",
        "CL_OUT_OF_HOST_MEMORY",
        "CL_PROFILING_INFO_NOT_AVAILABLE",
        "CL_MEM_COPY_OVERLAP",
        "CL_IMAGE_FORMAT_MISMATCH",
        "CL_IMAGE_FORMAT_NOT_SUPPORTED",           // #10
        "CL_BUILD_PROGRAM_FAILURE",
        "CL_MAP_FAILURE",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",                                        // #20
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "",
        "CL_INVALID_VALUE",                        // #30
        "CL_INVALID_DEVICE_TYPE",
        "CL_INVALID_PLATFORM",
        "CL_INVALID_DEVICE",
        "CL_INVALID_CONTEXT",
        "CL_INVALID_QUEUE_PROPERTIES",
        "CL_INVALID_COMMAND_QUEUE",
        "CL_INVALID_HOST_PTR",
        "CL_INVALID_MEM_OBJECT",
        "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
        "CL_INVALID_IMAGE_SIZE",                  // #40
        "CL_INVALID_SAMPLER",
        "CL_INVALID_BINARY",
        "CL_INVALID_BUILD_OPTIONS",
        "CL_INVALID_PROGRAM",
        "CL_INVALID_PROGRAM_EXECUTABLE",
        "CL_INVALID_KERNEL_NAME",
        "CL_INVALID_KERNEL_DEFINITION",
        "CL_INVALID_KERNEL",
        "CL_INVALID_ARG_INDEX",
        "CL_INVALID_ARG_VALUE",                  // #50
        "CL_INVALID_ARG_SIZE",
        "CL_INVALID_KERNEL_ARGS",
        "CL_INVALID_WORK_DIMENSION",
        "CL_INVALID_WORK_GROUP_SIZE",
        "CL_INVALID_WORK_ITEM_SIZE",
        "CL_INVALID_GLOBAL_OFFSET",
        "CL_INVALID_EVENT_WAIT_LIST",
        "CL_INVALID_EVENT",
        "CL_INVALID_OPERATION",
        "CL_INVALID_GL_OBJECT",                 // #60
        "CL_INVALID_BUFFER_SIZE",
        "CL_INVALID_MIP_LEVEL",
        "CL_INVALID_GLOBAL_WORK_SIZE",
    };

    const int errorCount = sizeof(errorString) / sizeof(errorString[0]);

    const int index = -error;

    return (index >= 0 && index < errorCount) ? errorString[index] : "";

}


// const char * helloStr  = "__kernel void hello(void) { }";                                  // works
// const char * helloStr  = "__kernel void hello(void) { float4 f4[40];  }";                  // works
// const char * helloStr  = "__kernel void hello(void) { int x = 40; float4 f4[x];  }";       // fails
const char * helloStr  = "__kernel void hello(void) { const int x = 40; float4 f4[x];  }"; // fails
// const char * helloStr  = "__kernel void hello(void) { } // compiled in source code string";                                     // works

char *readFile(char *fileName)
{
    char *bp = NULL;
    FILE *fp = fopen(fileName, "rb");
    if(fp)
    {
        fseek(fp, 0l, SEEK_END);
        int len = ftell(fp);
        if(len)
        {
            bp = new char[len];
            if(bp)
            {
                rewind(fp);
                fread(bp, len, sizeof(*bp), fp);
            }
        }
        fclose(fp);
    }
    return bp;
}

void Usage(const char *pgm)
{
    cout << "usage: " << pgm << " [-c] [-g] [-k kernel-file-name] [-n kernel-name]\n";
    cout << R"usage(
optional arguments:
-c                  ==> use CPU
-g                  ==> use GPU
-k kernel-file-name ==> load and compile "kernel-file-name"
-n kernel-name      ==> execute "kernel--name"
default action is execute dummy "hello" kernel on the CPU
)usage";
}


int printKernel( VECTOR_CLASS<cl::Device> devices, cl::Kernel &kernel )
{
    int        nArgs    = kernel.getInfo<CL_KERNEL_NUM_ARGS>();

    size_t sWGS; cl_long lLMS, lWGSM, lPMS;

    // CAREFUL! cl_long is 64-bit, need long long on host for 64-bit, NOTE long which is only 32-bit!
    assert(sizeof(cl_long) == sizeof(long long));

    cl_int iWGS  = kernel.getWorkGroupInfo( devices[0], CL_KERNEL_WORK_GROUP_SIZE,                    &sWGS  );
    cl_int iLMS  = kernel.getWorkGroupInfo( devices[0], CL_KERNEL_LOCAL_MEM_SIZE,                     &lLMS  );
    cl_int iWGSM = kernel.getWorkGroupInfo( devices[0], CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, &lWGSM );
    cl_int iPMS  = kernel.getWorkGroupInfo( devices[0], CL_KERNEL_PRIVATE_MEM_SIZE,                   &lPMS  );

    char *kernelName = (char*) kernel.getInfo<CL_KERNEL_FUNCTION_NAME>().c_str();

    cout << "compiled: kernel name \"" << kernelName << "\", args=" << nArgs << ", wg size=" << iWGS << ", local mem size=" << iLMS << ", preferred wg size multiple=" << lWGSM << ", private mem=" << iPMS << "\n";

    cout << "compiled CL_KERNEL_FUNCTION_NAME                      = " << kernel.getInfo<CL_KERNEL_FUNCTION_NAME>()  << endl;
    cout << "compiled CL_KERNEL_NUM_ARGS                           = " << kernel.getInfo<CL_KERNEL_NUM_ARGS>()       << endl;
    cout << "device   CL_KERNEL_WORK_GROUP_SIZE                    = " << sWGS  << endl;
    cout << "device   CL_KERNEL_LOCAL_MEM_SIZE                     = " << lLMS << " bytes per work item\n";
    cout << "device   CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE = " << lWGSM  << endl;
    cout << "device   CL_KERNEL_PRIVATE_MEM_SIZE                   = " << lPMS << " bytes per work item\n";

    cl::size_t<3> sizes;

    // c++ code
    // template <typename T>
    // cl_int getWorkGroupInfo(
    //     const Device& device, cl_kernel_work_group_info name, T* param) const
    // kernel.getWorkGroupInfo( devices[0], CL_KERNEL_COMPILE_WORK_GROUP_SIZE, sizes ); // run time error: bad parameter

    // c-code call:
    // cl_int clGetKernelWorkGroupInfo ( 	
    //     cl_kernel kernel,
    //     cl_device_id device,
    //     cl_kernel_work_group_info param_name,
    //     size_t param_value_size,
    //     void *param_value,
    //     size_t *param_value_size_ret)

    // size_t size_ret;
    // kernel.getWorkGroupInfo( 
    //     devices[0], 
    //     CL_KERNEL_COMPILE_WORK_GROUP_SIZE, 
    //     sizeof(sizes),
    //     sizes ,
    //     &size_ret
    // );

    // cout << "CL_KERNEL_COMPILE_WORK_GROUP_SIZE      = " << sizes[0] << " " << sizes[1] << " " << sizes[2]      << "\n";

    return nArgs;
}

void printKernels( VECTOR_CLASS<cl::Device> devices, cl::Program &program )
{
    VECTOR_CLASS<cl::Kernel> kernels;

    program.createKernels( &kernels );
    for ( vector<cl::Kernel>::iterator it = kernels.begin(); it != kernels.end(); ++it )
    {
        printKernel( devices, *it );
    }
}


int main(int argc, char **argv)
{
    const char *pgm = argv[0];
#ifdef _MSC_VER
    cout << "_MSC_VER          = " << _MSC_VER << endl;
#endif
    // cout << "sizeof(void*)       = " << sizeof(void*);
    // if(sizeof(void*)==8) cout << " --> 64-bit code\n"; else " --> 32-bit code\n";
    // cout << "sizeof(short)       = " << sizeof(short) << endl;
    // cout << "sizeof(int)         = " << sizeof(int) << endl;
    // cout << "sizeof(long)        = " << sizeof(long) << endl;
    // cout << "sizeof(long long)   = " << sizeof(long long) << endl;
    // cout << "sizeof(float)       = " << sizeof(float) << endl;
    // cout << "sizeof(double)      = " << sizeof(double) << endl;
    // cout << "sizeof(long double) = " << sizeof(long double) << endl;
    // cout << "sizeof(cl_short)    = " << sizeof(cl_short) << endl;
    // cout << "sizeof(cl_int)      = " << sizeof(cl_int) << endl;
    // cout << "sizeof(cl_long)     = " << sizeof(cl_long) << endl;
    // cout << "sizeof(cl_bool)     = " << sizeof(cl_bool)   << endl;
    // cout << "sizeof(cl_float)    = " << sizeof(cl_float)  << endl;
    // cout << "sizeof(cl_double)   = " << sizeof(cl_double) << endl;
    // cout << "sizeof(cl_float2)   = " << sizeof(cl_float2) << endl;
    // cout << "sizeof(cl_float3)   = " << sizeof(cl_float3) << endl;
    // cout << "sizeof(cl_float4)   = " << sizeof(cl_float4) << endl;
    int  ret          = -1;
    int  errorCount   = 0;
    bool bUseGPU      = false;
    char *kernel      = NULL;
    const char *kernelName  = "hello";
    for(int i = 1; i < argc; i++)
    {
        if(*argv[i] == '-')
        {
            switch(*(argv[i] + 1) )
            {
            case 'c':   bUseGPU = false;    break;
            case 'g':   bUseGPU = true;     break;
            case 'k':   
                if(*(argv[i]+2)){
                    kernel = argv[i]+2;
                } else if(i+1 < argc) {
                   i++;
                   kernel = argv[i];
                }
                if(kernel)
                {
                    kernel = readFile(kernel);
                }
                break;
             case 'n':   
                if(*(argv[i]+2)){
                    kernelName = argv[i]+2;
                } else if(i+1 < argc) {
                   i++;
                   kernelName = argv[i];
                }
                break;
            case '?':
            default:
                Usage(pgm);
                exit(0);
                break;
            }
        }
    }

    cout << pgm << " Hello\n";

    cl_int err = CL_SUCCESS;
    try { // scope for platform
        cout << "platform\n";
        vector<cl::Platform>   platforms;
        cl::Platform::get(&platforms);
        if (platforms.size() == 0)
        {
            cout << "OpenCL not installed (count of OpenCL platforms is zero)\n";
            return ret;
        }

        // cout << "Number of platforms: " << platforms.size() << endl;
        const char *want    =               "AMD Accelerated Parallel Processing";
        const char *wantErr = "cannot find \"AMD Accelerated Parallel Processing\" platform";
        int count = 0;
        int found = -1;
        for (vector<cl::Platform>::iterator it = platforms.begin(); it != platforms.end(); ++it)
        {
            cout << "CL Platform " << count << "\n";
            cout << "CL Name  : " << (*it).getInfo<CL_PLATFORM_NAME>().c_str() << endl;
            cout << "CL Vendor: " << (*it).getInfo<CL_PLATFORM_VENDOR>().c_str() << endl;
            if(strcmp(want, (*it).getInfo<CL_PLATFORM_NAME>().c_str() ) == 0)
                found = count;
            count++;
        }
        if(found == -1)
        {
            throw(wantErr);
        }
        cout << "using platform " << want << "\n";

        { // scope for context
            cout << "cl context\n";
            // use the last platform = size-1
            cl_context_properties properties[] =
                { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[ platforms.size()-1 ])(), 0};
            cl_device_type deviceType = bUseGPU?  CL_DEVICE_TYPE_GPU: CL_DEVICE_TYPE_CPU;
            cl::Context context(deviceType, properties);

            { // scope for devices
                cout << "devices\n";
                vector<cl::Device>     devices = context.getInfo<CL_CONTEXT_DEVICES>();
                cout << "Number of type " << deviceType << (bUseGPU? " (GPU)": " (CPU)") << " devices  : " << devices.size() << endl;
                for (vector<cl::Device>::iterator it = devices.begin(); it != devices.end(); ++it)
                {
                    cout << "CL Device Name       : " << (*it).getInfo <CL_DEVICE_NAME>      ().c_str() << endl;
                    cout << "CL Device Vendor     : " << (*it).getInfo <CL_DEVICE_VENDOR>    ().c_str() << endl;
                    cout << "CL Device Profile    : " << (*it).getInfo <CL_DEVICE_PROFILE>   ().c_str() << endl;
                    cout << "CL Device Version    : " << (*it).getInfo <CL_DEVICE_VERSION>   ().c_str() << endl;
                    cout << "CL Driver Version    : " << (*it).getInfo <CL_DRIVER_VERSION>   ().c_str() << endl;
                    cout << "CL Device Extensions : " << (*it).getInfo <CL_DEVICE_EXTENSIONS>().c_str() << endl;
                }
                cout << endl;

                if(!kernel) 
                {
                    kernel = (char*) helloStr;
                }
                if(strlen(kernel) < 1000)
                {
                    cout << "kernel:\n" << helloStr  << "\n";
                }
                { // scope for source
                    cout << "compiling kernel source\n";
                    cl::Program::Sources source(1, make_pair(kernel,strlen(kernel)));

                    { // scope for program
                        cl::Program program = cl::Program(context, source);
                        try {
                            const char *options = "-I . -DAMD -DCL_PRINTF";
                            cout << "OpenCL compiler options: " << options << "\n";
                            program.build(devices, options);
                            printKernels( devices, program );

                            { // scope for kernel
                                cout << "selecting kernel \"" << kernelName << "\"\n"; 
                                cl::Kernel kernel(program, kernelName, &err);

                                { // scope for event and cmd q
                                    cout << "creating command queue\n";
                                    cl::CommandQueue queue(context, devices[0], 0, &err);

                                    cout << "queueing kernel\n";
                                    cl::Event event;
                                    queue.enqueueNDRangeKernel( kernel, cl::NullRange, cl::NDRange(4,4), cl::NullRange, NULL, &event); 

                                    cout << "waiting for kernel to finish\n";
                                    event.wait();
                                } // end scope for event and cmd q
                            } // end scope for kernel
                        }
                        catch (cl::Error err)        // for error processing #define __CL_ENABLE_EXCEPTIONS prior to including CL/cl.hpp
                        {
                            errorCount++;
                            cerr << "ERROR: " << err.what() << "(" << err.err() << ") " <<  oclErrorString( err.err() ) << "\n";
                            if( CL_BUILD_PROGRAM_FAILURE == err.err() )
                            {
                                cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
                                cout << "Build log output:\n";
                                cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0]) << endl;
                                cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
                                throw "compile, kernel selection, or execution failed";
                            }
                        }
                    } // end scope for program
                } // end scope for source
            } // end scope for devices
        } // end scope for context

        cout << "all done, error count is " << errorCount << "\n";
    } // end scope for platform     
    catch (string errMsg)
    {
        errorCount++;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "error string= " << errMsg << "\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    }
    catch (const char *errMsg)
    {
        errorCount++;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "char* error = " << errMsg << "\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    }
    catch (...)
    {
        errorCount++;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
        cout << "unspecified error\n";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    }
    cout << pgm << " Goodbye\n\n";
    return EXIT_SUCCESS;
}
