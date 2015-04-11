// This is file "mul.cl".

#pragma OPENCL EXTENSION cl_amd_printf : enable

// get_work_dim()     = work_dim argument specified in clEnqueueNDRangeKernel. 
// get_global_id()    = number of global work-items specified to execute the kernel.
// get_global_size(0) = global_work_size argument to clEnqueueNDRangeKernel.
// get_local_size(0)  = local_work_size argument to clEnqueueNDRangeKernel
// get_local_id(0)    = unique local work-item ID
// get_num_groups(0)  = number of work-groups that will execute a kernel
// get_group_id(0)    = unique global work-item ID
 
__kernel void hello(__global float* a, __global float* b, __global float* c) 
{ 
    size_t i = get_global_id(0); // index <= get_work_dim()-1
    if(i == 1) { 
         printf("file %s\n", __FILE__);
         printf("get_work_dim()     = %d work_dim\n", get_work_dim()); 
         printf("get_global_id(0)   = %d global_work_size\n", get_global_id(0));   // index <= get_work_dim()-1
         printf("get_global_size(0) = %d N\n", get_global_size(0));                // index <= get_work_dim()-1
         printf("get_local_size(0)  = %d local_work_size\n", get_local_size(0));   // index <= get_work_dim()-1
         printf("get_local_id(0)    = %d\n", get_local_id(0));                     // index <= get_work_dim()-1
         printf("get_num_groups(0)  = %d (N + localWorkSize-1) / localWorkSize;\n", get_num_groups(0));  
                                                                                   // index <= get_work_dim()-1
         printf("get_group_id(0)    = %d\n", get_group_id(0));                     // index <= get_work_dim()-1
   } 
 
    c[i] = a[i] * b[i]; 
}
