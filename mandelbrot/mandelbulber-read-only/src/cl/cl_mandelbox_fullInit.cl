	float tgladDE = 1.0f;
	float3 ones = 1.0f;
	float3 foldingLimit = consts->fractal.mandelbox.foldingLimit;
	float3 foldingValue = consts->fractal.mandelbox.foldingValue;
	float mr2 = consts->fractal.mandelbox.minRadius * consts->fractal.mandelbox.minRadius;
	float fr2 = consts->fractal.mandelbox.fixedRadius * consts->fractal.mandelbox.fixedRadius;
	float scale = consts->fractal.mandelbox.scale;
	colourMin = 0.0f;