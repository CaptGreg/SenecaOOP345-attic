		if(consts->fractal.ifs.absX) z.x = fabs(z.x);
		if(consts->fractal.ifs.absY) z.y = fabs(z.y);
		if(consts->fractal.ifs.absZ) z.z = fabs(z.z);

		for(int i=0; i<9; i++)
		{
			if(consts->fractal.ifs.enabled[i])
			{
				z = Matrix33MulFloat3(consts->fractal.ifs.rot[i], z);
				float length = dot(z, consts->fractal.ifs.direction[i]);
				if(length < consts->fractal.ifs.distance[i])
				{
					z -= consts->fractal.ifs.direction[i] * (2.0f * (length - consts->fractal.ifs.distance[i]) * consts->fractal.ifs.intensity[i]);
				}
			}
		}
		
		z = Matrix33MulFloat3(consts->fractal.ifs.mainRot, z - consts->fractal.ifs.offset) + consts->fractal.ifs.offset;
		
		if(consts->fractal.ifs.edge.x > 0.0f) z.x = consts->fractal.ifs.edge.x - fabs(consts->fractal.ifs.edge.x - z.x);
		if(consts->fractal.ifs.edge.y > 0.0f) z.y = consts->fractal.ifs.edge.y - fabs(consts->fractal.ifs.edge.y - z.y);
		if(consts->fractal.ifs.edge.z > 0.0f) z.z = consts->fractal.ifs.edge.z - fabs(consts->fractal.ifs.edge.z - z.z);
		
		r = length(z);
		if (r < colourMin) colourMin = r;
		
		z *= consts->fractal.ifs.scale;
		
		if(consts->fractal.ifs.mengerSpongeMode)
		{
			z.x -= consts->fractal.ifs.offset.x * (consts->fractal.ifs.scale - 1.0f);
			z.y -= consts->fractal.ifs.offset.y * (consts->fractal.ifs.scale - 1.0f);
			float ztemp = consts->fractal.ifs.offset.z * (consts->fractal.ifs.scale - 1.0f);
			if(z.z > 0.5f * ztemp) z.z -= ztemp;
		}
		else
		{
			z -= consts->fractal.ifs.offset * (consts->fractal.ifs.scale - 1.0f);
		}
		
		DE *= consts->fractal.ifs.scale;
		r = distance(z, orbitTrap);
		
#if _orbitTrapsEnabled
		if (i >= consts->fractal.fakeLightsMinIter && i <= consts->fractal.fakeLightsMaxIter) distFromOrbitTrap += (1.0f/(r*r));
#endif
		
		if(r>1024.0f) 
		{
			dist = (r - 2.0f) / DE;
			out.colourIndex = colourMin * 1000.0f;
			break;
		}