//==============================================================================
//
//  System Simulation Group
//
//  University Erlangen-Nuremberg
//  Department of Computational Science
//  System Simulation Group
//  Cauerstrasse 6
//  91058 Erlangen
//  Germany
//
//------------------------------------------------------------------------------
//
//  Copyright (C) 2010 University Erlangen-Nuremberg
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//------------------------------------------------------------------------------
//
// Copyright (C) Harald Koestler
//
//==============================================================================

#ifndef opencl_h
#define opencl_h

//#define __NO_STD_VECTOR
//#define __NO_STD_STRING
#define __CL_ENABLE_EXCEPTIONS

//this is vendor specific!
/*
#include <oclUtils.h>

#include <SDKUtil/SDKFile.hpp>
#include <SDKUtil/SDKCommon.hpp>
*/

#include <CL/cl.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <fstream>
#include <stdexcept>

#include "boost/shared_ptr.hpp"

namespace OpenCL {


inline std::ostream& operator<<(std::ostream& out, const cl::Platform& platform) {
    out << "CL_PLATFORM_PROFILE    = " << platform.getInfo<CL_PLATFORM_PROFILE>() << "\n";
    out << "CL_PLATFORM_VERSION    = " << platform.getInfo<CL_PLATFORM_VERSION>() << "\n";
    out << "CL_PLATFORM_NAME       = " << platform.getInfo<CL_PLATFORM_NAME>() << "\n";
    out << "CL_PLATFORM_VENDOR     = " << platform.getInfo<CL_PLATFORM_VENDOR>() << "\n";
    out << "CL_PLATFORM_EXTENSIONS = " << platform.getInfo<CL_PLATFORM_EXTENSIONS>() << "\n";
    return out;
}

inline std::ostream& operator<<(std::ostream& out, const cl::Device& device) {
    out << "CL_DEVICE_ADDRESS_BITS                  = " << device.getInfo<CL_DEVICE_ADDRESS_BITS>() << "\n";
    out << "CL_DEVICE_AVAILABLE                     = " << device.getInfo<CL_DEVICE_AVAILABLE>() << "\n";
    out << "CL_DEVICE_COMPILER_AVAILABLE            = " << device.getInfo<CL_DEVICE_COMPILER_AVAILABLE>() << "\n";
    out << "CL_DEVICE_ENDIAN_LITTLE                 = " << device.getInfo<CL_DEVICE_ENDIAN_LITTLE>() << "\n";
    out << "CL_DEVICE_ERROR_CORRECTION_SUPPORT      = " << device.getInfo<CL_DEVICE_ERROR_CORRECTION_SUPPORT>() << "\n";
    out << "CL_DEVICE_EXECUTION_CAPABILITIES        = " << device.getInfo<CL_DEVICE_EXECUTION_CAPABILITIES>() << "\n";
    out << "CL_DEVICE_EXTENSIONS                    = " << device.getInfo<CL_DEVICE_EXTENSIONS>() << "\n";
    out << "CL_DEVICE_GLOBAL_MEM_CACHE_SIZE         = " << device.getInfo<CL_DEVICE_GLOBAL_MEM_CACHE_SIZE>() << "\n";
    out << "CL_DEVICE_GLOBAL_MEM_CACHE_TYPE         = " << device.getInfo<CL_DEVICE_GLOBAL_MEM_CACHE_TYPE>() << "\n";
    out << "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE     = " << device.getInfo<CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE>() << "\n";
    out << "CL_DEVICE_GLOBAL_MEM_SIZE               = " << device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << "\n";
    out << "CL_DEVICE_IMAGE_SUPPORT                 = " << device.getInfo<CL_DEVICE_IMAGE_SUPPORT>() << "\n";
    out << "CL_DEVICE_IMAGE2D_MAX_HEIGHT            = " << device.getInfo<CL_DEVICE_IMAGE2D_MAX_HEIGHT>() << "\n";
    out << "CL_DEVICE_IMAGE2D_MAX_WIDTH             = " << device.getInfo<CL_DEVICE_IMAGE2D_MAX_WIDTH>() << "\n";
    out << "CL_DEVICE_IMAGE3D_MAX_DEPTH             = " << device.getInfo<CL_DEVICE_IMAGE3D_MAX_DEPTH>() << "\n";
    out << "CL_DEVICE_IMAGE3D_MAX_HEIGHT            = " << device.getInfo<CL_DEVICE_IMAGE3D_MAX_HEIGHT>() << "\n";
    out << "CL_DEVICE_IMAGE3D_MAX_WIDTH             = " << device.getInfo<CL_DEVICE_IMAGE3D_MAX_WIDTH>() << "\n";
    out << "CL_DEVICE_LOCAL_MEM_SIZE                = " << device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << "\n";
    out << "CL_DEVICE_LOCAL_MEM_TYPE                = " << device.getInfo<CL_DEVICE_LOCAL_MEM_TYPE>() << "\n";
    out << "CL_DEVICE_MAX_CLOCK_FREQUENCY           = " << device.getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>() << "\n";
    out << "CL_DEVICE_MAX_COMPUTE_UNITS             = " << device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << "\n";
    out << "CL_DEVICE_MAX_CONSTANT_ARGS             = " << device.getInfo<CL_DEVICE_MAX_CONSTANT_ARGS>() << "\n";
    out << "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE      = " << device.getInfo<CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE>() << "\n";
    out << "CL_DEVICE_MAX_MEM_ALLOC_SIZE            = " << device.getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>() << "\n";
    out << "CL_DEVICE_MAX_PARAMETER_SIZE            = " << device.getInfo<CL_DEVICE_MAX_PARAMETER_SIZE>() << "\n";
    out << "CL_DEVICE_MAX_READ_IMAGE_ARGS           = " << device.getInfo<CL_DEVICE_MAX_READ_IMAGE_ARGS>() << "\n";
    out << "CL_DEVICE_MAX_SAMPLERS                  = " << device.getInfo<CL_DEVICE_MAX_SAMPLERS>() << "\n";
    out << "CL_DEVICE_MAX_WORK_GROUP_SIZE           = " << device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << "\n";
    out << "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS      = " << device.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>() << "\n";
    out << "CL_DEVICE_MAX_WRITE_IMAGE_ARGS          = " << device.getInfo<CL_DEVICE_MAX_WRITE_IMAGE_ARGS>() << "\n";
    out << "CL_DEVICE_MEM_BASE_ADDR_ALIGN           = " << device.getInfo<CL_DEVICE_MEM_BASE_ADDR_ALIGN>() << "\n";
    out << "CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE      = " << device.getInfo<CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE>() << "\n";
    out << "CL_DEVICE_NAME                          = " << device.getInfo<CL_DEVICE_NAME>() << "\n";
    out << "CL_DEVICE_PLATFORM                      = " << device.getInfo<CL_DEVICE_PLATFORM>() << "\n";
    out << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR   = " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR>() << "\n";
    out << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE = " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE>() << "\n";
    out << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT  = " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT>() << "\n";
    out << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT    = " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT>() << "\n";
    out << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG   = " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG>() << "\n";
    out << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT  = " << device.getInfo<CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT>() << "\n";
    out << "CL_DEVICE_PROFILE                       = " << device.getInfo<CL_DEVICE_PROFILE>() << "\n";
    out << "CL_DEVICE_PROFILING_TIMER_RESOLUTION    = " << device.getInfo<CL_DEVICE_PROFILING_TIMER_RESOLUTION>() << "\n";
    out << "CL_DEVICE_QUEUE_PROPERTIES              = " << device.getInfo<CL_DEVICE_QUEUE_PROPERTIES>() << "\n";
    out << "CL_DEVICE_SINGLE_FP_CONFIG              = " << device.getInfo<CL_DEVICE_SINGLE_FP_CONFIG>() << "\n";
    out << "CL_DEVICE_TYPE                          = " << device.getInfo<CL_DEVICE_TYPE>() << "\n";
    out << "CL_DEVICE_VENDOR_ID                     = " << device.getInfo<CL_DEVICE_VENDOR_ID>() << "\n";
    out << "CL_DEVICE_VENDOR                        = " << device.getInfo<CL_DEVICE_VENDOR>() << "\n";
    out << "CL_DEVICE_VERSION                       = " << device.getInfo<CL_DEVICE_VERSION>() << "\n";
    out << "CL_DRIVER_VERSION                       = " << device.getInfo<CL_DRIVER_VERSION>() << "\n";
    return out;
}


/*! class for handling OpenCL applications
 idioms: RAII with boost::shared_ptr
*/

class OpenCL
{

private:

    //! OpenCL platforms, first found platform is used
    std::vector<cl::Platform> platforms_;
    //! OpenCL devices, several devices can be used on one platform
    std::vector<cl::Device> devices_;
    //! OpenCL command queues, one for each platform
    //! no auto_ptr because of trouble with containers, RAII by hand
    std::vector<boost::shared_ptr<cl::CommandQueue> > queue_;
    //! OpenCL context, all devices are used (means GPU for Nvidia and GPU+CPU for AMD)
    boost::shared_ptr<cl::Context> context_;
    //! OpenCL sources, currently assumed to be in one file
    boost::shared_ptr<cl::Program::Sources> sources_;
    //! OpenCL program, built from context and sources
    boost::shared_ptr<cl::Program> program_;
    //! OpenCL kernels, contains all kernels from current program
    std::vector<cl::Kernel> kernels_;
    //! stores names of all loaded OpenCL kernels
    std::map<std::string,int> kernel_names_;
    //! stores number of available devices
    cl_int no_devices_;
    cl_int device_;

    OpenCL() : no_devices_ ( 0 ) {}
    ~OpenCL() { }

    OpenCL ( OpenCL const& );
    OpenCL& operator= ( OpenCL const& ); 

public:

    static OpenCL& instance()
    {
        static OpenCL ocl;
        return ocl;
    }

    const cl::Program& program() const {
        return *program_;
    }
    const cl::Context& context() const {
        return *context_;
    }
    const cl::CommandQueue& queue() const  {
        return *queue_.at ( device_ );
    }

    void init (const std::string platformIdent = "",const bool profiling = true, const size_t selDev = 0)
    {
        cl_int err;
        int platno=-1;
        int counter=0;
        std::cout<<"Getting Platform Information\n";
        cl::Platform::get ( &platforms_ );
        if (platforms_.size()==0) std::cerr << "There are no OpenCL platforms" << std::endl;

        std::vector<cl::Platform>::iterator i;
        if (platformIdent == "")
            platno = 0;
        else {
            if ( platforms_.size() > 0 )
            {
                for ( i = platforms_.begin(); i != platforms_.end(); ++i )
                {
                    std::cout << "Possible platform: " << ( *i ).getInfo<CL_PLATFORM_VENDOR>().c_str() << std::endl;
                    std::cout << *i;

                    if (( *i ).getInfo<CL_PLATFORM_VENDOR>()==platformIdent)platno=counter;
                    ++counter;

        
		    //! uncomment below if specific platform shall be chosen!
	            /*
                    if ( !strcmp ( ( *i ).getInfo<CL_PLATFORM_VENDOR>().c_str(), "Advanced Micro Devices, Inc." ) )
                            {
                                break;
                            }
                            */
                    /*
                    if ( !strcmp ( ( *i ).getInfo<CL_PLATFORM_VENDOR>().c_str(), "NVIDIA Corporation" ) )
                    {
                    	break;
                    }
                    */
                }
            }

            if (platno==-1) std::cerr << "Trying to find OpenCL platform "<<platformIdent<<", but could n't find it" << std::endl;
        }

        cl_context_properties cps[3] = { CL_CONTEXT_PLATFORM, ( cl_context_properties ) ( platforms_[platno] ) (), 0 };

        std::cout<<"Creating a context found platform " << platno << "\n";
        std::cout<<"Getting device info\n";
        
	platforms_[platno].getDevices(CL_DEVICE_TYPE_GPU, &devices_);

        no_devices_ = devices_.size();
        queue_.resize ( no_devices_ );

	context_.reset (new cl::Context (devices_, cps, NULL, NULL, &err));

        std::cout << "Available devices: " << devices_.size() << std::endl;
        size_t dev_no = 0;

        for ( std::vector<cl::Device>::iterator it = devices_.begin(); it != devices_.end(); ++it )
        {
            std::cout << *it;

            cl_device_type device_type = it->getInfo<CL_DEVICE_TYPE>();
            cl_uint device_max_compute_units = it->getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
            cl_uint device_max_work_item_dimensions = it->getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>();
            size_t  device_max_work_group_size = it->getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
            cl_uint device_max_clock_freq = it->getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>();
            cl_ulong device_max_mem_alloc_size = it->getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

            std::cout << "device type: " << device_type << std::endl;
            std::cout << "max compute units: " << device_max_compute_units << std::endl;
            std::cout << "max work item dimensions: " << device_max_work_item_dimensions << std::endl;
            std::cout << "max work group size: " << device_max_work_group_size << std::endl;
            std::cout << "max clock freq: " << device_max_clock_freq << std::endl;
            std::cout << "max mem alloc size: " << device_max_mem_alloc_size << std::endl;

            std::cout << "create queue for device " << dev_no << " and set profiling to " << profiling << std::endl;
            if ( profiling )
                queue_[dev_no].reset ( new cl::CommandQueue ( *context_, devices_[dev_no], CL_QUEUE_PROFILING_ENABLE, &err ) );
            else
                queue_[dev_no].reset ( new cl::CommandQueue ( *context_, devices_[dev_no], 0, &err ) );

            ++dev_no;
        }

        // select here the device with a certain devicetype
        if (selDev < dev_no) {
		device_ = selDev;
	        std::cout << "OpenCL: Selecting Device Number: "<< device_ <<" on platform "<<platformIdent << std::endl;
	} else {
		std::stringstream errMsg;
                errMsg << "OpenCL: Cannot use Device Number: " << selDev << ", only " << dev_no << " devices available on platform " << platformIdent << std::endl; 
		throw std::runtime_error(errMsg.str());	
	}

    }

  void initById (const size_t selPlat = 0,const bool profiling = true, const size_t selDev = 0)
    {
        cl_int err;
        int platno=-1;
        std::cout<<"Getting Platform Information\n";
        cl::Platform::get ( &platforms_ );
        if (platforms_.size()< selPlat) std::cerr << "Can't find selected platform" << std::endl;

	platno = selPlat;	

        cl_context_properties cps[3] = { CL_CONTEXT_PLATFORM, ( cl_context_properties ) ( platforms_[platno] ) (), 0 };

        std::cout<<"Creating a context found platform " << platno << "\n";

        std::cout<<"Getting device info\n";
        platforms_[selPlat].getDevices(CL_DEVICE_TYPE_DEFAULT, &devices_); 

        no_devices_ = devices_.size();
        queue_.resize ( no_devices_ );

	context_.reset (new cl::Context (devices_, cps, NULL, NULL, &err));

        std::cout << "Available devices: " << devices_.size() << std::endl;
        size_t dev_no = 0;

        for ( std::vector<cl::Device>::iterator it = devices_.begin(); it != devices_.end(); ++it )
        {
            std::cout << *it;

            cl_device_type device_type = it->getInfo<CL_DEVICE_TYPE>();
            cl_uint device_max_compute_units = it->getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
            cl_uint device_max_work_item_dimensions = it->getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>();
            size_t  device_max_work_group_size = it->getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
            cl_uint device_max_clock_freq = it->getInfo<CL_DEVICE_MAX_CLOCK_FREQUENCY>();
            cl_ulong device_max_mem_alloc_size = it->getInfo<CL_DEVICE_MAX_MEM_ALLOC_SIZE>();

            std::cout << "device type: " << device_type << std::endl;
            std::cout << "max compute units: " << device_max_compute_units << std::endl;
            std::cout << "max work item dimensions: " << device_max_work_item_dimensions << std::endl;
            std::cout << "max work group size: " << device_max_work_group_size << std::endl;
            std::cout << "max clock freq: " << device_max_clock_freq << std::endl;
            std::cout << "max mem alloc size: " << device_max_mem_alloc_size << std::endl;

            std::cout << "create queue for device " << dev_no << " and set profiling to " << profiling << std::endl;
            if ( profiling )
                queue_[dev_no].reset ( new cl::CommandQueue ( *context_, devices_[dev_no], CL_QUEUE_PROFILING_ENABLE, &err ) );
            else
                queue_[dev_no].reset ( new cl::CommandQueue ( *context_, devices_[dev_no], 0, &err ) );

            ++dev_no;
        }

        std::string platformIdent(platforms_[selPlat].getInfo<CL_PLATFORM_NAME>()); 

        // select here the device with a certain devicetype
        if (selDev < dev_no) {
		device_ = selDev;
	        std::cout << "OpenCL: Selecting Device Number: "<< device_ <<" on platform "<<platformIdent << std::endl;
	} else {
		std::stringstream errMsg;
                errMsg << "OpenCL: Cannot use Device Number: " << selDev << ", only " << dev_no << " devices available on platform " << platformIdent << std::endl; 
		throw std::runtime_error(errMsg.str());	
	}

    }


    void LoadSourceFile ( const std::string filename, const bool load_from_binary = false )
    { 

        std::ifstream fin ( filename.c_str() );
        if ( !fin ){
            std::stringstream errMsg;
	    errMsg  << "OpenCL::LoadSourceFile We couldn't load CL source code" << filename << std::endl;
	    throw std::runtime_error (errMsg.str());
	
	}
        std::istreambuf_iterator<char> begin ( fin );
        std::istreambuf_iterator<char> end;
        std::string code ( begin, end );

        sources_.reset ( new cl::Program::Sources ( 1, std::make_pair ( code.c_str(), code.length() ) ) );

        if ( load_from_binary )
        {
            std::vector<std::pair<const void*, size_t> > binaries;
            binaries.push_back ( std::make_pair ( code.c_str(), code.length() ) );
            program_.reset ( new cl::Program ( *context_, devices_, binaries ) );
        }
        else
        {
            program_.reset ( new cl::Program ( *context_, *sources_ ) );
        }
        //! notify function for callback can be used (user does not have to wait), second arg , "--help"
        try {
            program_->build ( devices_ , "-I.");
        } catch (...) {
            std::string buildError;
            program_->getBuildInfo(devices_[device_], CL_PROGRAM_BUILD_LOG, &buildError);
            std::cout<<buildError<<std::endl;
            std::cout<< program_->getBuildInfo<CL_PROGRAM_BUILD_OPTIONS>(devices_[device_]) << std::endl;
            exit(0);
        }
        //}

        program_->createKernels ( &kernels_ );
        std::string str;
        size_t args;
        size_t cnt = 0;

        std::cout << "loaded kernels from file " << filename << std::endl;
        for ( std::vector<cl::Kernel>::iterator it = kernels_.begin(); it != kernels_.end(); ++it )
        {
            ( *it ).getInfo ( CL_KERNEL_FUNCTION_NAME,&str );
            ( *it ).getInfo ( CL_KERNEL_NUM_ARGS,&args );
            kernel_names_.insert ( std::make_pair ( str , cnt ) );
            std::cout << str << " " << args << std::endl;
            ++cnt;
        }
    }

    cl::KernelFunctor initKernel (const std::string name, const cl::NDRange& global, const cl::NDRange& local, const cl::NDRange& offset = cl::NullRange)
    {
        cl::KernelFunctor kf = kernels_[kernel_names_[name]].bind ( *queue_[device_], offset, global, local );
        return kf;
    }

};

}

/*
//im main Programm kann man meine Klasse dann wie folgt benutzen:

try
{

	OpenCL::OpenCL* ocl = &OpenCL::OpenCL::instance();

	ocl->init();
	ocl->LoadSourceFile ( "./srcCL/HelloCL_Kernels.cl" );

	cl::Buffer uBuffer ( ocl->context(), CL_MEM_READ_WRITE, size *sizeof ( float ), NULL );

	cl::KernelFunctor initSF = ocl->initKernel ( "initScalarField",cl::NDRange ( RHS_GPU[0].nc(), RHS_GPU[0].nr()-1 ), cl::NDRange ( 16,16 ) );

	unsigned long startns, endns;
	cl::Event event;

	event = initSF ( uBuffer,0.0f );
	event.wait();

	// time measurement
	event.getProfilingInfo ( CL_PROFILING_COMMAND_START,&startns );
	event.getProfilingInfo ( CL_PROFILING_COMMAND_END,&endns );
	std::cout << " time " << ( endns-startns ) /1e6 << " ms " <<  std::endl;


}
catch ( cl::Error err )
{
	...
}

*/

#endif


