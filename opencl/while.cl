__kernel void myKernel( ) 
{  

     unsigned g_id_x = get_global_id(0);  

     while( g_id_x < 10 ) {     
        g_id_x ++;
    }
}

