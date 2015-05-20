		int3 cond1, cond2;
		
		cond1 = isgreater(z, foldingLimit);
		cond2 = isless(z, -foldingLimit);
		z = select(z,  foldingValue - z, cond1);
		z = select(z,  -foldingValue - z, cond2);
		
		//z = fabs(z + ones) - fabs(z - ones) - z;

		float rr = dot(z,z);
		float m = scale;
		if (rr < mr2)	m *= native_divide(fr2, mr2);
		else if (rr < fr2)	m *= native_divide(fr2, rr);
		
		z = Matrix33MulFloat3(consts->fractal.mandelbox.mainRot, z);
		
		z = z * m + c;
		tgladDE = tgladDE * fabs(m) + 1.0f;
		r = distance(z, orbitTrap);
		
		colourMin += fabs(m);
		
#if _orbitTrapsEnabled
		if (i >= consts->fractal.fakeLightsMinIter && i <= consts->fractal.fakeLightsMaxIter) {distFromOrbitTrap += (1.0f/(r*r)); minOrbitTrapDist = min(minOrbitTrapDist, r);}
#endif
		
		if(r>1024.0f) 
		{
			dist = r / fabs(tgladDE);
			out.colourIndex = colourMin / i * 300.0f;
			break;
		}