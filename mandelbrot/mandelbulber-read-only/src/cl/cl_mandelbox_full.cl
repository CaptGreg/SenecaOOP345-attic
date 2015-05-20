		bool lockout = false;
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rot[0], z);
		if (z.x > consts->fractal.mandelbox.foldingLimit)
		{
			z.x = consts->fractal.mandelbox.foldingValue - z.x;
			colourMin += consts->fractal.mandelbox.colorFactorX;
			lockout = true;
		}
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rotinv[0], z);
		
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rot[1], z);
		if (!lockout && z.x < -consts->fractal.mandelbox.foldingLimit)
		{
			z.x = -consts->fractal.mandelbox.foldingValue - z.x;
			colourMin += consts->fractal.mandelbox.colorFactorX;
		}
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rotinv[1], z);
		
		lockout = false;
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rot[2], z);
		if (z.y > consts->fractal.mandelbox.foldingLimit)
		{
			z.y = consts->fractal.mandelbox.foldingValue - z.y;
			colourMin += consts->fractal.mandelbox.colorFactorY;
			lockout = true;
		}
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rotinv[2], z);
		
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rot[3], z);
		if (!lockout && z.y < -consts->fractal.mandelbox.foldingLimit)
		{
			z.y = -consts->fractal.mandelbox.foldingValue - z.y;
			colourMin += consts->fractal.mandelbox.colorFactorY;
		}
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rotinv[3], z);
		
		lockout = false;
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rot[4], z);
		if (z.z > consts->fractal.mandelbox.foldingLimit)
		{
			z.z = consts->fractal.mandelbox.foldingValue - z.z;
			colourMin += consts->fractal.mandelbox.colorFactorZ;
			lockout = true;
		}
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rotinv[4], z);
		
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rot[5], z);
		if (!lockout && z.z < -consts->fractal.mandelbox.foldingLimit)
		{
			z.z = -consts->fractal.mandelbox.foldingValue - z.z;
			colourMin += consts->fractal.mandelbox.colorFactorZ;
		}
		z = Matrix33MulFloat3(consts->fractal.mandelbox.rotinv[5], z);
		
		float rr = dot(z,z);
		float m = scale;
		if (rr < mr2)	m = native_divide(scale, mr2);
		else if (rr < fr2)	m = native_divide(scale,rr);
		
		z = Matrix33MulFloat3(consts->fractal.mandelbox.mainRot, z);
		
		z = z * m + c;
		tgladDE = tgladDE * fabs(m) + 1.0f;
		r = distance(z, orbitTrap);
		
		colourMin += fabs(m);
		
#if _orbitTrapsEnabled
		if (i >= consts->fractal.fakeLightsMinIter && i <= consts->fractal.fakeLightsMaxIter) distFromOrbitTrap += (1.0f/(r*r));
#endif
		
		if(r>1024.0f) 
		{
			dist = r / fabs(tgladDE);
			out.colourIndex = colourMin / i * 300.0f;
			break;
		}