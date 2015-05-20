
		newz.x = z4.x * z4.x - z4.y * z4.y - z4.z * z4.z - z4.w * z4.w;
		newz.y = 2.0f * z4.x * z4.y - 2.0f * z4.w * z4.z;
		newz.z = 2.0f * z4.x * z4.z - 2.0f * z4.y * z4.w;
		newz.w = 2.0f * z4.x * z4.w - 2.0f * z4.y * z4.z;
		z4 = newz + c4;
		r = length(z4);
		if (r < colourMin) colourMin = r;
		z = (float3){z4.x, z4.y, z4.z};
		if(r>40.0f || any(isinf(z4))) 
		{
			dist = r;
			out.colourIndex = colourMin * 5000.0f;
			break;
		}


