// This file has all commands which are executed inside iteration loop
//    for(i = 0; i < N; i++)
// example is based on Mandelbox formula
// z is a main 3-dimentional iterated vector
// c is constant variable like in Mandelbrot set: z(n+1) = z(n)^2 + c
// r is a length of z vector

z = fabs(z + ones) - fabs(z - ones) - z;

float rr = dot(z,z);
float m = scale;
if (rr < mr2) m = scale / mr2;
else if (rr < fr2) m = scale / rr;

//example of use of rotation matrix (Mandelbox main rotation)
z = Matrix33MulFloat3(consts->fractal.mandelbox.mainRot, z);

z = z * m + c;

//part of distance estimation calculation
tgladDE = tgladDE * fabs(m) + 1.0f;

//length of z vector
r = distance(z, orbitTrap);

//surface colour
colourMin += fabs(m);

//needed for calculation of fake lights based on orbit traps
#if _orbitTrapsEnabled
if (i >= consts->fractal.fakeLightsMinIter && i <= consts->fractal.fakeLightsMaxIter) distFromOrbitTrap += (1.0f/(r*r));
#endif

//iteration terminate condition
if(r>1024.0f) 
{
	//calculation of distance estimation
	dist = r / fabs(tgladDE);
	//calculation of surface colour
	out.colourIndex = colourMin / i * 300.0f;
	break;
}