		float r1 = native_powr(r, power - 1.0f);
		float r2 = r1 * r;
		float th = atan2(z.y, z.x);
		float ph = -atan2(z.z, sqrt(z.x * z.x + z.y * z.y));

		float p_r1_rdz = power * r1 * r_dz;
		float ph_phdz = (power - 1.0f) * ph + ph_dz;
		float th_thdz = (power - 1.0f) * th + th_dz;
		dz.x = native_cos(ph_phdz) * native_cos(th_thdz);
		dz.y = native_cos(ph_phdz) * native_sin(th_thdz);
		dz.z = native_sin(ph_phdz);
		dz = dz * p_r1_rdz + one;
		r_dz = length(dz);
		th_dz = atan2(dz.y, dz.x);
		ph_dz = -atan2(dz.z, sqrt(dz.x * dz.x + dz.y * dz.y));

		float pth = power * th;
		float pph = power * ph;
		z.x = native_cos(pph) * native_cos(pth);
		z.y = native_cos(pph) * native_sin(pth);
		z.z = native_sin(pph);

		z = z * r2 + c;
		r = distance(z, orbitTrap);
		if (r < colourMin) colourMin = r;
		
#if _orbitTrapsEnabled
		if (i >= consts->fractal.fakeLightsMinIter && i <= consts->fractal.fakeLightsMaxIter) distFromOrbitTrap += (1.0f/(r*r+1e-10));
#endif
		
		if(r>4.0f || any(isinf(z))) 
		{
			dist = 0.5f * r * native_log(r) / (r_dz);
			out.colourIndex = colourMin * 5000.0f;
			break;
		}
