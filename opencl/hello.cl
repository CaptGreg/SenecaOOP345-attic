#pragma OPENCL EXTENSION cl_amd_printf : enable

__kernel void helloCL()
{
    size_t i = get_global_id(0);
	/*print work item ID*/
	
	printf("Hello World, my ID is %d\n", i);

    if(i == 2) { 
         printf("   get_work_dim()     = %d work_dim\n", get_work_dim()); 
         printf("   get_global_id(0)   = %d global_work_size\n", get_global_id(0));   // index <= get_work_dim()-1
         printf("   get_global_size(0) = %d N\n", get_global_size(0));                // index <= get_work_dim()-1

         printf("   get_num_groups(0)  = %d (N + localWorkSize-1) / localWorkSize;\n", get_num_groups(0));  
         printf("   get_local_size(0)  = %d local_work_size\n", get_local_size(0));   // index <= get_work_dim()-1
         printf("   get_group_id(0)    = %d\n", get_group_id(0));                     // index <= get_work_dim()-1
         printf("   get_local_id(0)    = %d\n", get_local_id(0));                     // index <= get_work_dim()-1
         printf("testing while loop ...");
        while(i < 10)
            i++;
         printf("while loop finished\n");
                                                                                      
   } 
	
	return;
}

__kernel void initScalarField()
{
}
