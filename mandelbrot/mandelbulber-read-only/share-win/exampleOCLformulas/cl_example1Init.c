// This file has all commands which are executed once before iteration loop.
// It can be used to initialize all variables and do some preliminary calculations
// all available variables are defined in mandelbulber_cl_data.h

float tgladDE = 1.0f;

//example use of custom parameter
float3 ones = 1.0f + consts->fractal.custom[0];
	
float mr2 = consts->fractal.mandelbox.minRadius * consts->fractal.mandelbox.minRadius;
float fr2 = consts->fractal.mandelbox.fixedRadius * consts->fractal.mandelbox.fixedRadius;
float scale = consts->fractal.mandelbox.scale;

//this variable is used to calculate surface colour
colourMin = 0.0f;
