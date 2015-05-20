/*********************************************************
 /                   MANDELBULBER
 / primitive objects
 / using Fractal() function
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

float PrimitivePlane(float3 point, float3 centre, float3 normal)
{
	float3 plane = normal;
	plane = plane * (1.0/ length(plane));
	float planeDistance = dot(plane, point - centre);
	return planeDistance;
}

#if _primitiveBox
float PrimitiveBox(float3 point, float3 center, float3 size)
{
	float distance, planeDistance;
	float3 corner1 = (float3){center.x - 0.5f*size.x, center.y - 0.5f*size.y, center.z - 0.5f*size.z};
	float3 corner2 = (float3){center.x + 0.5f*size.x, center.y + 0.5f*size.y, center.z + 0.5f*size.z};

	planeDistance = PrimitivePlane(point, corner1, (float3){-1.0f,0.0f,0.0f});
	distance = planeDistance;
	planeDistance = PrimitivePlane(point, corner2, (float3){1.0f,0.0f,0.0f});
	distance = (planeDistance > distance) ? planeDistance : distance;

	planeDistance = PrimitivePlane(point, corner1, (float3){0.0f,-1.0f,0.0f});
	distance = (planeDistance > distance) ? planeDistance : distance;
	planeDistance = PrimitivePlane(point, corner2, (float3){0.0f,1.0f,0.0f});
	distance = (planeDistance > distance) ? planeDistance : distance;

	planeDistance = PrimitivePlane(point, corner1, (float3){0.0f,0.0f,-1.0f});
	distance = (planeDistance > distance) ? planeDistance : distance;
	planeDistance = PrimitivePlane(point, corner2, (float3){0.0f,0.0f,1.0f});
	distance = (planeDistance > distance) ? planeDistance : distance;

	return distance;
}
#endif

#if _primitiveInvertedBox
float PrimitiveInvertedBox(float3 point, float3 center, float3 size)
{
	float distance, planeDistance;
	float3 corner1 = (float3){center.x - 0.5f*size.x, center.y - 0.5f*size.y, center.z - 0.5f*size.z};
	float3 corner2 = (float3){center.x + 0.5f*size.x, center.y + 0.5f*size.y, center.z + 0.5f*size.z};

	planeDistance = PrimitivePlane(point, corner1, (float3){1.0f,0.0f,0.0f});
	distance = planeDistance;
	planeDistance = PrimitivePlane(point, corner2, (float3){-1.0f,0.0f,0.0f});
	distance = (planeDistance < distance) ? planeDistance : distance;

	planeDistance = PrimitivePlane(point, corner1, (float3){0.0f,1.0f,0.0f});
	distance = (planeDistance < distance) ? planeDistance : distance;
	planeDistance = PrimitivePlane(point, corner2, (float3){0.0f,-1.0f,0.0f});
	distance = (planeDistance < distance) ? planeDistance : distance;

	planeDistance = PrimitivePlane(point, corner1, (float3){0.0f,0.0f,1.0f});
	distance = (planeDistance < distance) ? planeDistance : distance;
	planeDistance = PrimitivePlane(point, corner2, (float3){0.0f,0.0f,-1.0f});
	distance = (planeDistance < distance) ? planeDistance : distance;

	return distance;
}
#endif

#if _primitiveSphere
float PrimitiveSphere(float3 point, float3 center, float radius)
{
	float dist = distance(point, center) - radius;
	return dist;
}
#endif

#if _primitiveInvertedSphere
float PrimitiveInvertedSphere(float3 point, float3 center, float radius)
{
	float dist = radius - distance(point, center);
	return dist;
}
#endif

#if _primitiveWater
float PrimitiveWater(float3 point, float height, float amplitude, float length, float rotation, int iterations, float animSpeed, int frame)
{
	float planeDistance = height - point.z;
	if(planeDistance < amplitude * 20.0f)
	{
		float angle = rotation/180.0*M_PI_F;
		float2 rotm1 = (float2){cos(angle), -sin(angle)};
		float2 rotm2 = (float2){sin(angle), cos(angle)};
		
		float3 point2 = (float3){point.x * cos(angle) - point.y * sin(angle), point.x * sin(angle) + point.y * cos(angle), point.z};
		
		float phase = animSpeed * frame;
		float k=0.23f;
		float waveXtemp = point.x;
		float waveYtemp = point.y;
		float waveX = 0.0f;
		float waveY = 0.0f;
		float p = 1.0f;
		float p2 = 0.05;
		for(int i=1; i <= iterations; i++)
		{
			float p3 = p * p2;
			float shift = phase / (i/3.0f + 1.0f);
			waveXtemp = sin(i + 0.4f*(waveX)*p3 + sin(k* point2.y / length*p3) + point2.x/length*p3 + shift)/p;
			waveYtemp = cos(i + 0.4f*(waveY)*p3 + sin(point2.x / length*p3) + k*point2.y/length*p3 + shift*0.23f)/p;
			waveX+=waveXtemp;
			waveY+=waveYtemp;
			p2 = p2 + (1.0f - p2)*0.7f;
			p *= 1.872f;
		}
		planeDistance += (waveX + waveY) * amplitude;
	}

	return planeDistance;
}
#endif