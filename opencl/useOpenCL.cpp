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
//
#include "useOpenCL.h"

int main(int argc, char**argv)
{
    
    const int nr = 256;
    const int nc = 256;
    const int size = 256 * 256;

    try
    {

        OpenCL::OpenCL* ocl = &OpenCL::OpenCL::instance();

        ocl->init();
        ocl->LoadSourceFile ( "./hello.cl" );

        cl::Buffer uBuffer ( ocl->context(), CL_MEM_READ_WRITE, size * sizeof ( float ), NULL );

        // cl::KernelFunctor initSF = ocl->initKernel ( "initScalarField",cl::NDRange ( RHS_GPU[0].nc(), RHS_GPU[0].nr()-1 ), cl::NDRange ( 16,16 ) );
        cl::KernelFunctor initSF = ocl->initKernel ( "initScalarField",cl::NDRange ( nc, nr ), cl::NDRange ( 16,16 ) );

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
    }
}
