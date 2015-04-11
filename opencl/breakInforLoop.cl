
// test break in for loop
// makes compiler emit 'syntax error' but indicates success?

void syntaxCrash(float3 *f3) { int i; for(i=0;i<9;i++) {if(i==5) break;} }

__kernel void hello(__global float* a, __global float* b, __global float* c)
{
    size_t i = get_global_id(0);
    c[i] = a[i] + b[i];
}
