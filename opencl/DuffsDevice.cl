
// test break in for loop
// void syntaxCrash(float3 *f3) { int i; for(i=0;i<9;i++) {if(i==5) break;} }

void DuffsDevice(__global float *from, __global float *to, int count)
{
        int n=(count+7)/8;
        switch(count%8){
        case 0:      do{     *to = *from++;
        case 7:              *to = *from++;
        case 6:              *to = *from++;
        case 5:              *to = *from++;
        case 4:              *to = *from++;
        case 3:              *to = *from++;
        case 2:              *to = *from++;
        case 1:              *to = *from++;
                }while(--n>0);
        }
}

__kernel void hello(__global float* a, __global float* b, __global float* c)
{
    size_t i = get_global_id(0);
    DuffsDevice(a, c, i); // call it!
    c[i] = c[i] + b[i];
}
