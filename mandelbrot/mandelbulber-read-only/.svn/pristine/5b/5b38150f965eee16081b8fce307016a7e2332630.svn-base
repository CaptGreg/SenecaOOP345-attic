		z = fabs(z);
		
		if (z.x - z.y < 0.0f)
		{
			temp = z.y;
			z.y = z.x;
			z.x = temp;
		}
		if (z.x - z.z < 0.0f)
		{
			temp = z.z;
			z.z = z.x;
			z.x = temp;
		}
		if (z.y - z.z < 0.0f)
		{
			temp = z.z;
			z.z = z.y;
			z.y = temp;
		}
		
		r = length(z);
		if (r < colourMin) colourMin = r;
		
		z *= 3.0f;

		z.x -= 2.0f;
		z.y -= 2.0f;
		if (z.z > 1.0f) z.z -= 2.0f;
		r = distance(z, orbitTrap);
		DE *= 3.0f;

#if _orbitTrapsEnabled
		if (i >= consts->fractal.fakeLightsMinIter && i <= consts->fractal.fakeLightsMaxIter) distFromOrbitTrap += (1.0f/(r*r));
#endif
		
		if (r > 1024.0f)
		{
			dist = r / fabs(DE);
			out.colourIndex = colourMin * 1000.0f;
			break;
		}
		