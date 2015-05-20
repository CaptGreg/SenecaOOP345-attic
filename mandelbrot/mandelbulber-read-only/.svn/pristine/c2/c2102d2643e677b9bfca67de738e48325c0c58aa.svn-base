
		float th0 = asin(native_divide(z.z ,r));
		float ph0 = atan2(z.y, z.x);
		float rp = native_powr(r, power - 1.0f);
		float th = th0 * power;
		float ph = ph0 * power;
		float cth = native_cos(th);
		r_dz = rp * r_dz * power + 1.0f;
		rp *= r;
		z = (float3) {cth * native_cos(ph), cth * native_sin(ph), native_sin(th)};
		z*=rp;
		z+=c;
		r = distance(z, orbitTrap);
		if (r < colourMin) colourMin = r;
		
#if _orbitTrapsEnabled
		if (i >= consts->fractal.fakeLightsMinIter && i <= consts->fractal.fakeLightsMaxIter) distFromOrbitTrap += (1.0f/(r*r));
#endif
		
		if(r>4.0f || any(isinf(z))) 
		{
			dist = 0.5f * r * native_log(r) / (r_dz);
			out.colourIndex = colourMin * 5000.0f;
			break;
		}
