// This file has all commands which are executed inside iteration loop
//    for(i = 0; i < N; i++)
// example is based on Mandelbulb formula
// z is a main 3-dimentional iterated vector
// c is constant variable like in Mandelbrot set: z(n+1) = z(n)^2 + c
// r is a length of z vector

float th0 = asin(native_divide(z.z ,r)) + offset;
float ph0 = atan2(z.y, z.x) + offset;
float rp = native_powr(r, power - 1.0f);
float th = th0 * power;
float ph = ph0 * power ;
float cth = native_cos(th);
r_dz = rp * r_dz * power + 1.0f;
rp *= r;
z = (float3) {cth * native_cos(ph), cth * native_sin(ph), native_sin(th)};
z*=rp;
z+=c*mul;

//calculation of length of z vector
r = distance(z, orbitTrap);

//calculation of surface colour
if (r < colourMin) colourMin = r;

//needed for calculation of fake lights based on orbit traps
#if _orbitTrapsEnabled
if (i >= consts->fractal.fakeLightsMinIter && i <= consts->fractal.fakeLightsMaxIter) distFromOrbitTrap += (1.0f/(r*r));
#endif

//iteration terminate condition
if(r>4.0f || any(isinf(z))) 
{
	//calculation of distance estimation
	dist = 0.5f * r * native_log(r) / (r_dz);
	
	//calculation of surface colour
	out.colourIndex = colourMin * 5000.0f;
	break;
}
