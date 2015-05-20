//*********************************************************
//                   MANDELBULBER
// full kernel for image rendering
// 
//
// author: Krzysztof Marczak
// contact: buddhi1980@gmail.com
// licence: GNU GPL v3.0
//
//*********************************************************

typedef float3 cl_float3;
typedef float cl_float;
typedef int cl_int;
typedef unsigned int cl_uint;
typedef unsigned short cl_ushort;
typedef unsigned char cl_uchar;

#include INCLUDE_PATH_CL_DATA

#define MAX_RAYMARCHING 100000

__constant sampler_t sampler = CLK_NORMALIZED_COORDS_TRUE |
                               CLK_ADDRESS_REPEAT |
                               CLK_FILTER_LINEAR;

typedef struct
{
	float3 z;
	float iters;
	float distance;
	float colourIndex;
	float distFromOrbitTrap;
	float minOrbitTrapDist;
	enumClObjectType objectOut;
} formulaOut;

static formulaOut Fractal(__constant sClInConstants *consts, float3 point, sClCalcParams *calcParam);
static formulaOut CalculateDistance(__constant sClInConstants *consts, float3 point, sClCalcParams *calcParam);

int rand(int* seed)
{
    int s = *seed;
    int i = 0;
   	int const a = 15484817;
   	int const m = 6571759; 
   	s = ((long)(s * a)) % m;
  
    *seed = s;
    return(s % 65536);
}

float3 Matrix33MulFloat3(matrix33 matrix, float3 vect)
{
	float3 out;
	out.x = dot(vect, matrix.m1);
	out.y = dot(vect, matrix.m2);
	out.z = dot(vect, matrix.m3);
	return out;
}

matrix33 Matrix33MulMatrix33(matrix33 m1, matrix33 m2)
{
	matrix33 out;
	out.m1.x = m1.m1.x * m2.m1.x + m1.m1.y * m2.m2.x + m1.m1.z * m2.m3.x;
	out.m1.y = m1.m1.x * m2.m1.y + m1.m1.y * m2.m2.y + m1.m1.z * m2.m3.y;
	out.m1.z = m1.m1.x * m2.m1.z + m1.m1.y * m2.m2.z + m1.m1.z * m2.m3.z;
	out.m2.x = m1.m2.x * m2.m1.x + m1.m2.y * m2.m2.x + m1.m2.z * m2.m3.x;
	out.m2.y = m1.m2.x * m2.m1.y + m1.m2.y * m2.m2.y + m1.m2.z * m2.m3.y;
	out.m2.z = m1.m2.x * m2.m1.z + m1.m2.y * m2.m2.z + m1.m2.z * m2.m3.z;
	out.m3.x = m1.m3.x * m2.m1.x + m1.m3.y * m2.m2.x + m1.m3.z * m2.m3.x;
	out.m3.y = m1.m3.x * m2.m1.y + m1.m3.y * m2.m2.y + m1.m3.z * m2.m3.y;
	out.m3.z = m1.m3.x * m2.m1.z + m1.m3.y * m2.m2.z + m1.m3.z * m2.m3.z;
	return out;
}

matrix33 RotateX(matrix33 m, float angle)
{
		matrix33 out, rot;
		float s = sin(angle);
		float c = cos(angle);		
		rot.m1 = (float3) {1.0f, 0.0f, 0.0f};
		rot.m2 = (float3) {0.0f, c   , -s  };
		rot.m3 = (float3) {0.0f, s   ,  c  };
		out = Matrix33MulMatrix33(m, rot);
		return out;
}

matrix33 RotateY(matrix33 m, float angle)
{
		matrix33 out, rot;
		float s = sin(angle);
		float c = cos(angle);		
		rot.m1 = (float3) {c   , 0.0f, s   };
		rot.m2 = (float3) {0.0f, 1.0f, 0.0f};
		rot.m3 = (float3) {-s  , 0.0f, c   };
		out = Matrix33MulMatrix33(m, rot);
		return out;
}

matrix33 RotateZ(matrix33 m, float angle)
{
		matrix33 out, rot;
		float s = sin(angle);
		float c = cos(angle);		
		rot.m1 = (float3) { c  , -s  , 0.0f};
		rot.m2 = (float3) { s  ,  c  , 0.0f};
		rot.m3 = (float3) {0.0f, 0.0f, 1.0f};
		out = Matrix33MulMatrix33(m, rot);
		return out;
}

float4 quaternionMul(float4 q1, float4 q2)
{
//source: http://www.cs.columbia.edu/~keenan/Projects/QuaternionJulia/QJuliaFragment.html
	float4 result;
	result.x = q1.x * q2.x - dot(q1.yzw, q2.yzw);
	result.yzw = q1.x * q2.yzw + q2.x * q1.yzw + cross(q1.yzw, q2.yzw);
	return result;
}

float4 quaternionSqr(float4 q)
{
	//source: http://www.cs.columbia.edu/~keenan/Projects/QuaternionJulia/QJuliaFragment.html
	float4 result;
	result.x = q.x * q.x - dot(q.yzw, q.yzw);
	result.yzw = 2.0f * q.x * q.yzw;
	return result;
}

float3 IndexToColour(int index, global float3 *palette)
{
	float3 colOut, col1, col2, colDiff;

	if (index < 0)
	{
		colOut = palette[255];
	}
	else
	{
		int index2 = index % 65280;
		int no = index2 / 256;
		col1 = palette[no];
		col2 = palette[no+1];
		colDiff = col2 - col1;
		float delta = (index2 % 256)/256.0f;
		colOut = col1 + colDiff * delta;
	}
	return colOut;
}

float IterOpacity(float step, float iters, float maxN, float trim, float opacitySp)
{
	float opacity = ((float)iters - trim) / maxN;
	if(opacity < 0.0f) opacity = 0.0f;
	opacity*=opacity;
	opacity*=step  * opacitySp;
	if(opacity > 1.0f) opacity = 1.0f;
	return opacity;
}

float3 CalculateNormals(__constant sClInConstants *consts, sClShaderInputData *input)
{
	float3 normal = 0.0f;
	//calculating normal vector based on distance estimation (gradient of distance function)
	if (!consts->params.slowShading)
	{
		float delta = input->delta;
		if(consts->fractal.interiorMode) delta = input->delta * consts->params.quality * 0.2f;

		float s1, s2, s3, s4;
		input->calcParam->N *= 5.0f;
		//input.calcParam->minN = 0;
		input->calcParam->distThresh = input->dist_thresh;
		input->calcParam->normalCalculationMode = true;
		s1 = CalculateDistance(consts, input->point, input->calcParam).distance;

		float3 deltax = (float3) {delta, 0.0f, 0.0f};
		s2 = CalculateDistance(consts, input->point + deltax, input->calcParam).distance;

		float3 deltay = (float3) {0.0f, delta, 0.0f};
		s3 = CalculateDistance(consts, input->point + deltay, input->calcParam).distance;

		float3 deltaz = (float3) {0.0f, 0.0f, delta};
		s4 = CalculateDistance(consts, input->point + deltaz, input->calcParam).distance;

		normal.x = s2 - s1;
		normal.y = s3 - s1;
		normal.z = s4 - s1;
		input->calcParam->N = consts->fractal.N;
		input->calcParam->normalCalculationMode = false;
		//input.calcParam->minN = input.param->fractal.minN;
	}

	//calculating normal vector based on average value of binary central difference
	else
	{
		float result2;
		bool max_iter;

		float3 point2;
		float3 point3;
		input->calcParam->distThresh = input->delta;
		input->calcParam->normalCalculationMode = true;
		for (point2.x = -1.0f; point2.x <= 1.0f; point2.x += 0.2f) //+0.2
		{
			for (point2.y = -1.0f; point2.y <= 1.0f; point2.y += 0.2f)
			{
				for (point2.z = -1.0f; point2.z <= 1.0f; point2.z += 0.2f)
				{
					point3 = input->point + point2 * input->delta;

					float dist = CalculateDistance(consts, point3, input->calcParam).distance;

					if (dist < input->dist_thresh || max_iter) result2 = 0.0f;
					else result2 = 1.0f;

					normal += (point2 * result2);
				}
			}
		}
		input->calcParam->normalCalculationMode = false;
	}

	if(normal.x == 0.0f && normal.y == 0.0f && normal.z == 0.0f)
	{
		normal.x = 1.0f;
	}
	else
	{
		normal = normalize(normal);
	}

	return normal;
}



float3 RayMarching(__constant sClInConstants *consts, sClCalcParams *calcParam, float3 start, float3 direction, float maxScan, bool binaryEnable, float *distThreshOut, float *lastDistOut, bool *foundOut, float *depthOut, int *stepCount)
{
	float3 point;
	bool found = false;
	float scan = length(start)*5.0e-6f;
	float distThresh = *distThreshOut;
	float dist = 0.0f;
	float search_accuracy = 0.01f * consts->params.quality;
	//printf("consts->params.quality %f, start %f %f %f\n", consts->params.quality, start.x, start.y, start.z);
	float search_limit = 1.0f - search_accuracy;
	float step = 1e-10f;
	float resolution = 1.0f/consts->params.width;
	*stepCount = 0;
	int count = 0;
	float distThreshInit = *distThreshOut;
	//printf("DE_factor %f\n", consts->params.DEfactor);
	
	//printf("Start, start.x = %g, start.y = %g, start.z = %g\n", start.x, start.y, start.z);
	formulaOut outF;
	for (int i = 0; i < MAX_RAYMARCHING; i++)
	{
		point = start + direction * scan;
		//printf("viewVector %f %f %f\n", direction.x, direction.y, direction.z);
		//printf("scan %f\n", scan);
		//printf("DE_factor %f\n", consts->params.DEfactor);

		if (consts->fractal.constantDEThreshold)
		{
			distThresh = consts->params.quality;
			//printf("DistThresh = %f\n", distThresh);
		}
		else
		{
			distThresh = scan * resolution * consts->params.persp / consts->params.quality + distThreshInit;
		}

		//conts->fractal.detailSize = distThresh;

		calcParam->distThresh = distThresh;
		calcParam->normalCalculationMode = false;
		outF = CalculateDistance(consts, point, calcParam);
		dist = outF.distance;
		
		//printf("Distance = %f\n", dist/distThresh);

		if (dist > 3.0f) dist = 3.0f;
		if (dist < distThresh)
		{
			found = true;
			break;
		}
		//printf("DE_factor %f\n", consts->params.DEfactor);
		step = (dist - 0.5f * distThresh) * consts->params.DEfactor;// * (0.9f + 0.2f * rand(&calcParam->randomSeed)/65536.0);
		scan += step;
		if (scan > maxScan)
		{
			break;
		}
		count = i;
	}
	*stepCount = count;
	
	if (found && binaryEnable)
	{
		step *= 0.5f;
		for (int i = 0; i < 10; i++)
		{
			if (dist < distThresh && dist > distThresh * search_limit)
			{
				break;
			}
			else
			{
				if (dist > distThresh)
				{
					scan += step;
					point = start + direction * scan;
				}
				else if (dist < distThresh * search_limit)
				{
					scan -= step;
					point = start + direction * scan;
				}
			}
			outF = CalculateDistance(consts, point, calcParam);
			dist = outF.distance;
			//printf("Distance binary = %g\n", dist/distThresh);

			step *= 0.5f;
		}
	}

	calcParam->objectType = outF.objectOut;
	*foundOut = found;
	*distThreshOut = distThresh;
	*lastDistOut = dist;
	*depthOut = scan;
	return point;
}

float3 MainShadow(__constant sClInConstants *consts, sClShaderInputData *input)
{
	float3 shadow = 1.0f;

	//starting point
	float3 point2;

	float factor = input->delta / input->resolution;
	if (!consts->params.penetratingLights) factor = consts->params.viewDistanceMax;
	float dist = input->dist_thresh;

	float DE_factor = consts->params.DEfactor;
	if(consts->params.iterFogEnabled || consts->params.volumetricLightEnabledAny) DE_factor = 1.0;

	float start = input->delta;
	//if (input->calcParam->interiorMode) start = input.dist_thresh * DE_factor * 0.5;

	float opacity = 0.0f;
	float shadowTemp = 1.0f;

	float softRange = tan(consts->params.shadowConeAngle / 180.0f * 3.14f);
	float maxSoft = 0.0f;

	bool bSoft = (!consts->params.iterFogEnabled && !consts->fractal.limitsEnabled /*&& !input.calcParam->iterThresh*/) && softRange > 0.0;
	
	float distThresh =  input->dist_thresh;
	
	for (float i = start; i < factor; i += dist * DE_factor)
	{
		point2 = input->point + input->lightVect * i;
		input->calcParam->distThresh = distThresh;
		formulaOut out = CalculateDistance(consts, point2, input->calcParam);
		dist = out.distance;
		
		if(consts->params.iterFogEnabled || consts->params.volumetricLightEnabledAny)
		{
			if (!consts->fractal.constantDEThreshold)
				distThresh = distance(input->eyePoint, point2) * input->resolution * consts->params.persp / consts->params.quality * 0.707106f;
			else
				distThresh = consts->params.quality;
		}
		else
			distThresh = input->dist_thresh;
		
		if (bSoft)
		{
			float angle = (dist - distThresh) / i;
			if (angle < 0.0f) angle = 0.0f;
			if (dist < distThresh) angle = 0.0f;
			float softShadow = (1.0f - angle / softRange);
			if (consts->params.penetratingLights) softShadow *= (factor - i) / factor;
			if (softShadow < 0.0f) softShadow = 0.0f;
			if (softShadow > maxSoft) maxSoft = softShadow;
		}

		if (consts->params.iterFogEnabled)
		{
			opacity = IterOpacity(dist * DE_factor, out.iters, consts->fractal.N, consts->params.iterFogOpacityTrim,
					consts->params.iterFogOpacity);
		}
		else
		{
			opacity = 0.0f;
		}
		shadowTemp -= opacity * (factor - i) / factor;
		
		if (dist < distThresh || shadowTemp < 0.0f)
		{
			shadowTemp -= (factor - i) / factor;
			if (!consts->params.penetratingLights) shadowTemp = 0.0f;
			if (shadowTemp < 0.0f) shadowTemp = 0.0f;
			break;
		}
	}
	if (!bSoft)
	{
		shadow = shadowTemp;
	}
	else
	{
		shadow = (1.0f - maxSoft);
	}
	return shadow;
}

float3 MainSpecular(sClShaderInputData *input)
{
	float3 specular;
	float3 halfVector = input->lightVect - input->viewVector;
	halfVector = normalize(halfVector);
	float shade2 = dot(input->normal,halfVector);
	if (shade2 < 0.0f) shade2 = 0.0f;
	shade2 = pow(shade2, 30.0f) * 1.0f;
	if (shade2 > 15.0f) shade2 = 15.0f;
	specular = shade2;
	return specular;
}

float3 SurfaceColour(__constant sClInConstants *consts, sClShaderInputData *input, global cl_float3 *palette)
{
	float3 surfaceColour = 0.0f;
	switch(input->objectType)
	{
		case clObjFractal:
		{
			input->calcParam->N *= 10;
			formulaOut outF = Fractal(consts, input->point, input->calcParam);
			input->calcParam->N /= 10;
			int colourNumber = outF.colourIndex * consts->params.colouringSpeed + 256.0 * consts->params.colouringOffset;
			surfaceColour = 1.0;
			if (consts->params.colouringEnabled) surfaceColour = IndexToColour(colourNumber, palette);
			break;
		}
		case clObjPlane:
		{
			surfaceColour = consts->fractal.primitives.primitivePlaneColour;
			break;
		}
		case clObjBox:
		{
			surfaceColour = consts->fractal.primitives.primitiveBoxColour;
			break;
		}
		case clObjBoxInv:
		{
			surfaceColour = consts->fractal.primitives.primitiveInvertedBoxColour;
			break;
		}
		case clObjSphere:
		{
			surfaceColour = consts->fractal.primitives.primitiveSphereColour;
			break;
		}
		case clObjSphereInv:
		{
			surfaceColour = consts->fractal.primitives.primitiveInvertedSphereColour;
			break;
		}
		case clObjWater:
		{
			surfaceColour = consts->fractal.primitives.primitiveWaterColour;
			break;
		}
	}
	return surfaceColour;
}

float3 FastAmbientOcclusion2(__constant sClInConstants *consts, sClShaderInputData *input)
{
	//reference: http://www.iquilezles.org/www/material/nvscene2008/rwwtt.pdf (Iñigo Quilez – iq/rgba)

	float delta = input->dist_thresh;
	float aoTemp = 0.0f;
	int quality = consts->params.globalIlumQuality;
	input->calcParam->distThresh = delta;
	for (int i = 1; i < quality * quality; i++)
	{
		float scan = i * i * delta;
		float3 pointTemp = input->point + input->normal * scan;
		float 		dist = CalculateDistance(consts, pointTemp, input->calcParam).distance;
		aoTemp += 1.0f / (pow(2.0f, i)) * (scan - consts->params.fastAoTune * dist) / input->dist_thresh;
	}
	float ao = 1.0f - 0.2f * aoTemp;
	if (ao < 0.0f) ao = 0.0f;
	float3 output = ao;
	return output;
}

#if _SlowAOEnabled
float3 AmbientOcclusion(__constant sClInConstants *consts, sClShaderInputData *input, global float3 *vectorsAround, global float3 *vectorsAroundColours)
{
	float3 AO = 0.0f;

	float start_dist = input->delta;
	float end_dist = input->delta / input->resolution;
	float intense = 0.0f;
	float distThresh = input->dist_thresh;
	
	for (int i = 0; i < input->vectorsCount; i++)
	{
		float3 v = vectorsAround[i];
		float3 colour = vectorsAroundColours[i];

		//printf("ao vector %d, %f %f %f\n", i, v.x, v.y, v.z);
		
		float dist = input->lastDist;

		float opacity = 0.0;
		float shadowTemp = 1.0;

		int count = 0;
		for (float r = start_dist; r < end_dist; r += dist * 2.0f)
		{
			float3 point2 = input->point + v * r;

			input->calcParam->distThresh = distThresh;
			formulaOut out = CalculateDistance(consts, point2, input->calcParam);
			dist = out.distance;

			if (consts->params.iterFogEnabled)
			{
				opacity = IterOpacity(dist * 2.0f, out.iters, consts->fractal.N, consts->params.iterFogOpacityTrim,
						consts->params.iterFogOpacity);
			}
			else
			{
				opacity = 0.0;
			}
			shadowTemp -= opacity * (end_dist - r) / end_dist;

			if(consts->params.iterFogEnabled || consts->params.volumetricLightEnabledAny)
			{
				if (!consts->fractal.constantDEThreshold)
					distThresh = distance(input->eyePoint, point2) * input->resolution * consts->params.persp / consts->params.quality * 0.707106f;
				else
					distThresh = consts->params.quality;
			}
			else
				distThresh = input->dist_thresh;
			
			if (dist < distThresh || shadowTemp < 0.0)
			{
				shadowTemp -= (end_dist - r) / end_dist;
				if (shadowTemp < 0.0) shadowTemp = 0.0;
				break;
			}
			if (count > 100) break;
		}

		intense = shadowTemp;

		AO += intense * colour;

	}
	AO = AO / input->vectorsCount;

	return AO;
}
#endif

#if _AuxLightsEnabled
float AuxShadow(__constant sClInConstants *consts, sClShaderInputData *input, float distanceToLight, float3 lightVector)
{
	
	float step = input->delta;
	float dist = step;
	float light = 1.0f;

	float opacity = 0.0f;
	float shadowTemp = 1.0f;

	//float DE_factor = consts->params.DEfactor;
	float DE_factor = 1.0;
	
	float distThresh = input->dist_thresh;

	if(consts->params.iterFogEnabled || consts->params.volumetricLightEnabled[0]) DE_factor = 1.0f;
	int count = 0;
	//printf("start, thresh = %f, distance = %f\n", input->dist_thresh, distanceToLight);
	for (float i = input->delta; i < distanceToLight; i += dist * DE_factor)
	{
		//printf("i = %f, dist = %f\n", i, dist);
		float3 point2 = input->point + lightVector * i;

		input->calcParam->distThresh = distThresh;
		formulaOut out = CalculateDistance(consts, point2, input->calcParam);
		dist = out.distance;
		
		if (consts->params.iterFogEnabled)
		{
			opacity = IterOpacity(dist * DE_factor, out.iters, consts->fractal.N, consts->params.iterFogOpacityTrim,
					consts->params.iterFogOpacity);
		}
		else
		{
			opacity = 0.0f;
		}
		shadowTemp -= opacity * (distanceToLight - i) / distanceToLight;

		if(consts->params.iterFogEnabled || consts->params.volumetricLightEnabledAny)
		{
			if (!consts->fractal.constantDEThreshold)
				distThresh = distance(input->eyePoint, point2) * input->resolution * consts->params.persp / consts->params.quality * 0.707106f;
			else
				distThresh = consts->params.quality;
		}
		else
			distThresh = input->dist_thresh;
		
		if (dist < distThresh || shadowTemp < 0.0f || count > 1000)
		{
			if (consts->params.penetratingLights)
			{
				shadowTemp -= (distanceToLight - i) / distanceToLight;
				if (shadowTemp < 0.0f) shadowTemp = 0.0f;
			}
			else
			{
				shadowTemp = 0.0f;
			}
			break;
		}
		count ++;
	}
	light = shadowTemp;
	return light;
}

float3 LightShading(__constant sClInConstants *consts, sClShaderInputData *input, sClLight light, int number, float3 *specularOut)
{
	float3 shading;

	float3 d = light.position - input->point;

	float distance = length(d);

	//angle of incidence
	float3 lightVector = d;
	lightVector = normalize(lightVector);

	float intensity = consts->params.auxLightIntensity * 100.0f * light.intensity / (distance * distance) / number;
	float shade = dot(input->normal, lightVector);
	if (shade < 0.0f) shade = 0.0f;
	shade = shade * intensity;
	if (shade > 500.0f) shade = 500.0f;

	//specular
	float3 halfVector = lightVector - input->viewVector;
	halfVector = normalize(halfVector);
	float shade2 = dot(input->normal, halfVector);
	if (shade2 < 0.0f) shade2 = 0.0f;
	shade2 = pow(shade2, 30.0f) * 1.0f;
	shade2 *= intensity * consts->params.specularIntensity;
	if (shade2 > 15.0) shade2 = 15.0;

	//calculate shadow
	if ((shade > 0.01f || shade2 > 0.01f) && consts->params.shadow)
	{
		float light = AuxShadow(consts, input, distance, lightVector);
		shade *= light;
		shade2 *= light;
	}
	else
	{
		if (consts->params.shadow)
		{
			shade = 0.0f;
			shade2 = 0.0f;
		}
	}

	shading = shade * light.colour;
	(*specularOut) = shade2 * light.colour;

	return shading;
}

float3 AuxLightsShader(__constant sClInConstants *consts, sClShaderInputData *input, float3 *specularOut, global sClLight *lights)
{
	int numberOfLights = consts->params.auxLightNumber;

	float3 shadeAuxSum = 0.0f;
	float3 specularAuxSum = 0.0f;
	for (int i = 0; i < numberOfLights; i++)
	{
		if (lights[i].enabled)
		{
			float3 specularAuxOutTemp;
			float3 shadeAux = LightShading(consts, input, lights[i], numberOfLights, &specularAuxOutTemp);
			shadeAuxSum += shadeAux;
			specularAuxSum += specularAuxOutTemp;
		}
	}
	*specularOut = specularAuxSum;
	return shadeAuxSum;
}
#endif

#if _orbitTrapsEnabled
float3 FakeLights(__constant sClInConstants *consts, sClShaderInputData *input, float3 *fakeSpecOut)
{
	float3 fakeLights;

	input->calcParam->orbitTrap = consts->params.fakeLightsOrbitTrap;

	formulaOut out = Fractal(consts, input->point, input->calcParam);
	float r = 1.0/(out.distFromOrbitTrap+1e-10);
	//float fakeLight = consts->params.fakeLightsIntensity/(r*r + 1e-20f);
	float fakeLight = consts->params.fakeLightsIntensity * (out.distFromOrbitTrap);

	float3 deltax = (float3)
	{ input->delta, 0.0f, 0.0f};
	out = Fractal(consts, input->point + deltax, input->calcParam);
	float rx = 1.0/(out.distFromOrbitTrap+1e-10);

	float3 deltay = (float3)
	{ 0.0f, input->delta, 0.0f};
	out = Fractal(consts, input->point + deltay, input->calcParam);
	float ry = 1.0/(out.distFromOrbitTrap+1e-10);

	float3 deltaz = (float3)
	{ 0.0f, 0.0f, input->delta};
	out = Fractal(consts, input->point + deltaz, input->calcParam);
	float rz = 1.0/(out.distFromOrbitTrap+1e-10);

	float3 fakeLightNormal;
	fakeLightNormal.x = r - rx;
	fakeLightNormal.y = r - ry;
	fakeLightNormal.z = r - rz;

	if(length(fakeLightNormal) == 0.0f)
	{
		fakeLightNormal.x = 0.0f;
	}
	else
	{
		fakeLightNormal = normalize(fakeLightNormal);
	}

	fakeLight = sqrt(fakeLight);
	
	float fakeLight2 = fakeLight * dot(input->normal, fakeLightNormal);
	if(fakeLight2 < 0.0f) fakeLight2 = 0.0f;
	fakeLights = fakeLight2;

	float3 halfVector = fakeLightNormal - input->viewVector;
	halfVector = normalize(halfVector);
	float fakeSpecular = dot(input->normal, halfVector);
	if (fakeSpecular < 0.0f) fakeSpecular = 0.0f;
	fakeSpecular = pow(fakeSpecular, 30.0f) * fakeLight;
	if (fakeSpecular > 15.0f) fakeSpecular = 15.0f;
	(*fakeSpecOut) = fakeSpecular;

	input->calcParam->orbitTrap = 0.0f;

	return fakeLights;
}
#endif

float3 ObjectShader(__constant sClInConstants *consts, sClShaderInputData *input, float3 *specularOut, float3 *colourOut, global sClInBuff *inBuff)
{
	float3 output;

	//normal vector
	float3 vn = CalculateNormals(consts, input);
	input->normal = vn;
	
	float3 mainLight = consts->params.mainLightIntensity * consts->params.mainLightColour;
	
	//calculate shading based on angle of incidence
	float shadeTemp = dot(input->normal, input->lightVect);
	if (shadeTemp < 0.0f) shadeTemp = 0.0f;
	shadeTemp = consts->params.mainLightIntensity * ((1.0f - consts->params.shading) + consts->params.shading * shadeTemp);
	float3 shade = shadeTemp;
	
	//calculate shadow
	float3 shadow = 1.0f;
	if(consts->params.shadow) shadow = MainShadow(consts, input);
	
	//calculate specular highlight
	float3 specular = MainSpecular(input) * consts->params.specularIntensity;
	
	//calculate surface colour
	float3 colour = SurfaceColour(consts, input, inBuff->palette);
	
	
	//ambient occlusion
	float3 ambient = 0.0f;
	if(consts->params.fastAmbientOcclusionEnabled)
	{
		ambient = FastAmbientOcclusion2(consts, input);
	}
	else if(consts->params.slowAmbientOcclusionEnabled)
	{
#if _SlowAOEnabled
		ambient = AmbientOcclusion(consts, input, inBuff->vectorsAround, inBuff->vectorsAroundColours);
#endif
	}
	float3 ambient2 = ambient * consts->params.ambientOcclusionIntensity;
	
	//additional lights
	float3 auxLights = 0.0f;
	float3 auxLightsSpecular = 0.0f;
#if _AuxLightsEnabled
	auxLights = AuxLightsShader(consts, input, &auxLightsSpecular, inBuff->lights);
	auxLightsSpecular *= consts->params.specularIntensity;
#endif
	
	float3 fakeLights = 0.0f;
	float3 fakeLightsSpecular = 0.0f;
#if _orbitTrapsEnabled
	fakeLights = FakeLights(consts, input, &fakeLightsSpecular);
#endif
	
	output = mainLight * shadow * (shade * colour) + ambient2 * colour + (auxLights + fakeLights) * colour;
	(*specularOut) = mainLight * shadow * specular + auxLightsSpecular + fakeLightsSpecular;
	(*colourOut) = colour;
	
	return output;
}

float3 BackgroundShader(__constant sClInConstants *consts, sClShaderInputData *input, image2d_t imageBackground)
{
	float3 colour;
#if	_texturedBackground
	float alfaTexture = fmod(atan2(input->viewVector.y, input->viewVector.x) + 2.5f * M_PI, 2.0f * M_PI_F);
	float betaTexture = atan2(input->viewVector.z, length(input->viewVector.xy));
	if (betaTexture > 0.5f * M_PI_F) betaTexture = 0.5f * M_PI_F - betaTexture;
	if (betaTexture < -0.5f * M_PI_F) betaTexture = -0.5f * M_PI_F + betaTexture;
	float2 texCord;
	texCord.x = alfaTexture / (2.0f * M_PI_F);
	texCord.y = (betaTexture / (M_PI_F) + 0.5f);
	float4 pixel = read_imagef(imageBackground, sampler, texCord);
	colour = pixel.xyz;
#else
	float3 vector = { 0.0f, 0.0f, -1.0f };
	vector = normalize(vector);
	float grad = dot(input->viewVector, vector) + 1.0f;

	if (grad < 1.0f)
	{
		float ngrad = 1.0f - grad;
		colour = consts->params.backgroundColour3 * ngrad + consts->params.backgroundColour2 * grad;
	}
	else
	{
		grad = grad - 1.0f;
		float ngrad = 1.0f - grad;
		colour = consts->params.backgroundColour2 * ngrad + consts->params.backgroundColour1 * grad;
	}
#endif
	return colour;
}

float3 VolumetricShader(__constant sClInConstants *consts, sClShaderInputData *input, global sClInBuff *inBuff, float3 oldPixel, float *opacityOut, float *alphaOut)
{
	float3 output = oldPixel;
	float scan = input->depth;
	if(scan > consts->params.viewDistanceMax) scan = consts->params.viewDistanceMax;
	float dist = input->lastDist;
	float distThresh = input->dist_thresh;
	float delta = input->delta;
	bool end = false;
	float alpha = 0.0f;
	float totalOpacity = 0.0f;
	
	//glow init
  float glow = input->stepCount * consts->params.glowIntensity / 512.0f * consts->params.DEfactor;
  float glowN = 1.0f - glow;
  if (glowN < 0.0f) glowN = 0.0f;
  float3 glowColour = (consts->params.glowColour1 * glowN + consts->params.glowColour2 * glow);
	
  //volumetric fog init
	float colourThresh = consts->params.fogColour1Distance;
	float colourThresh2 = consts->params.fogColour2Distance;
	float fogReduce = consts->params.fogDistanceFactor;
	float fogIntensity = consts->params.fogDensity;
	
	float lastOrbitTrapDist = 1.0e3f;
  
	for(int i=0; i<MAX_RAYMARCHING; i++)
	{
		//calculate back step
		float step = min(dist, lastOrbitTrapDist) * consts->params.DEfactor * (0.9f + 0.2f * rand(&input->calcParam->randomSeed)/65536.0);
		
		if(step < 1.0e-6f) step = 1.0e-6f;
		if(step > scan) 
		{
			step = scan;
			end = true;
		}
		//else if(step > 100.0f * distThresh) step = 100.0f * distThresh;
		
		if(step < distThresh) step = distThresh;
		scan -= step;
		//printf("scan = %g, step = %g, distThresh = %g, dist = %g\n", scan, step, distThresh, dist);
		
		float3 point = input->startPoint + input->viewVector * scan;
		input->point = point;
		
		input->calcParam->distThresh = distThresh;
		formulaOut out = CalculateDistance(consts, point, input->calcParam);
		dist = out.distance;
		
		if (consts->fractal.constantDEThreshold)
		{
			distThresh = consts->params.quality;
		}
		else
		{
			distThresh = distance(point, input->eyePoint) * input->resolution * consts->params.persp / consts->params.quality;
		}
		input->dist_thresh = distThresh;
		
		if(consts->fractal.constantDEThreshold) input->delta = scan * input->resolution * consts->params.persp;
		else input->delta = distThresh * consts->params.quality;
		delta = input->delta;
		
		//------------------- glow
		float glowOpacity = glow * step / input->depth;
		output = glowOpacity * glowColour + (1.0f - glowOpacity) * output;
		alpha += glowOpacity;
		
		//------------------ visible light
		if (consts->params.auxLightVisibility > 0.0f)
		{
			float lowestLightSize = 1.0e10f;
			float lowestLightDist = 1.0e10f;
			for (int i = 0; i < consts->params.auxLightNumber; ++i)
			{
				if (inBuff->lights[i].enabled)
				{
					float3 lightDistVect = point - inBuff->lights[i].position;
					float lightDist = length(lightDistVect);
					float lightSize = inBuff->lights[i].intensity * consts->params.auxLightIntensity * consts->params.auxLightVisibility;
					float r2 = lightDist / lightSize;
					if (r2 < lowestLightSize)
					{
							lowestLightSize = r2;
							lowestLightDist = lightDist;
					}
				}
			}

			//small steps close to light source to improve accuracy
			int smallSteps = 0;
			int smallStep_start = 0;
			int smallStep_end = 1;
			float step2 = step;

			smallSteps = 10.0f * step / (lowestLightDist + 1.0e-15f);
			if(smallSteps > 50) smallSteps = 50;
			if (smallSteps > 0)
			{
				smallStep_start = 1;
				smallStep_end = smallSteps + 1;
				step2 = step / (smallSteps + 1.0f);
			}

			for (int smallStep = smallStep_start; smallStep < smallStep_end; smallStep++)
			{
				float3 point2 = point + input->viewVector * step2 * smallStep;

				for (int i = 0; i < consts->params.auxLightNumber; ++i)
				{
					if (inBuff->lights[i].enabled)
					{
						float3 lightDistVect = point2 - inBuff->lights[i].position;
						float lightDist = length(lightDistVect);
						float lightSize = inBuff->lights[i].intensity * consts->params.auxLightIntensity * consts->params.auxLightVisibility;
						float r2 = lightDist / lightSize;
						if (r2 > 1.0f) r2 = 1.0f;
						float bellFunction = (cos(r2 * M_PI_F) + 1.0f) / (r2 * r2 + 0.02f) * 1.0f;
						float lightDensity = step2 * bellFunction / lightSize;

						output += lightDensity * inBuff->lights[i].colour;
						alpha += lightDensity;
					}
				}
			}
		}
		
#if _orbitTrapsEnabled
		if(consts->params.fakeLightsEnabled)
		{
			input->calcParam->orbitTrap = consts->params.fakeLightsOrbitTrap;
			formulaOut out = Fractal(consts, input->point, input->calcParam);
			float r = sqrt(1.0f/(out.distFromOrbitTrap + 1.0e-10f));
			float fakeLight = 1.0f / (pow(r, 10.0f / consts->params.fakeLightsVisibilitySize) * pow(10.0f, 10.0f / consts->params.fakeLightsVisibilitySize) + 1e-20f);
			output += fakeLight * step * consts->params.fakeLightsVisibility;
			alpha += fakeLight * step * consts->params.fakeLightsVisibility;
			input->calcParam->orbitTrap = 0.0f;
			lastOrbitTrapDist = out.minOrbitTrapDist / pow(10.0f, 10.0f / consts->params.fakeLightsVisibilitySize);
		}
#endif		
		//---------------------- volumetric lights with shadows in fog


		for (int i = 0; i < 5; i++)
		{
			if (i == 0 && consts->params.volumetricLightEnabled[0])
			{
				float3 shadowOutputTemp = MainShadow(consts, input);
				output += shadowOutputTemp * step * consts->params.volumetricLightIntensity[0] * consts->params.mainLightColour;
				alpha += (shadowOutputTemp.x + shadowOutputTemp.y + shadowOutputTemp.z)/3.0 * step * consts->params.volumetricLightIntensity[0];
			}
#if _AuxLightsEnabled
			if (i > 0)
			{
				if (inBuff->lights[i - 1].enabled && consts->params.volumetricLightEnabled[i])
				{
					float3 d = inBuff->lights[i - 1].position - point;
					float distance = length(d);
					if(distance > 0.0f)
					{
						float distance2 = distance * distance;
						float3 lightVectorTemp = d;
						lightVectorTemp = normalize(lightVectorTemp);
						float light = AuxShadow(consts, input, distance, lightVectorTemp);
						//float light = 1.0;
						output += light * inBuff->lights[i - 1].colour * consts->params.volumetricLightIntensity[i] * step / distance2;
						alpha += light * consts->params.volumetricLightIntensity[i] * step / distance2;
					}
				}
			}
#endif
		}

		
		//----------------------- basic fog
		if(consts->params.fogEnabled)
		{
			float fogDensity = step / consts->params.fogVisibility;
			if(fogDensity > 1.0f) fogDensity = 1.0f;
			output = fogDensity * consts->params.fogColour + (1.0f - fogDensity) * output;
			totalOpacity = fogDensity + (1.0f - fogDensity) * totalOpacity;
			alpha = fogDensity + (1.0f - fogDensity) * alpha;
		}
		
		
		//-------------------- volumetric fog
		if(fogIntensity > 0.0f)
		{
			float densityTemp = (step * fogReduce) / (dist * dist + fogReduce * fogReduce);

			float k = dist / colourThresh;
			if (k > 1.0f) k = 1.0f;
			float kn = 1.0f - k;
			float3 fogTemp = (consts->params.fogColour1 * kn + consts->params.fogColour2 * k);

			float k2 = dist / colourThresh2 * k;
			if (k2 > 1.0f) k2 = 1.0f;
			kn = 1.0f - k2;
			fogTemp = fogTemp * kn + consts->params.fogColour3 * k2;

			float fogDensity = 0.3f * fogIntensity * densityTemp / (1.0f + fogIntensity * densityTemp);
			if(fogDensity > 1.0f) fogDensity = 1.0f;

			output = fogDensity * fogTemp + (1.0 - fogDensity) * output;
			
			totalOpacity = fogDensity + (1.0 - fogDensity) * totalOpacity;
			alpha = fogDensity + (1.0 - fogDensity) * alpha;
		}
		
		//iter fog
		
		if (consts->params.iterFogEnabled)
		{
			int L = out.iters;
			float opacity = IterOpacity(step, L, consts->fractal.N, consts->params.iterFogOpacityTrim, consts->params.iterFogOpacity);

			float3 newColour = 0.0f;
			if (opacity > 0)
			{
				//fog colour
				float iterFactor = (float) 2.0f * (L - consts->params.iterFogOpacityTrim) / (consts->fractal.N - consts->params.iterFogOpacityTrim);
				float k = iterFactor;
				if (k > 1.0) k = 1.0;
				if (k < 0.0) k = 0.0;
				float kn = 1.0 - k;
				float3 fogCol = (consts->params.fogColour1 * kn + consts->params.fogColour2 * k);

				float k2 = iterFactor - 1.0f;
				if (k2 < 0.0f) k2 = 0.0f;
				if (k2 > 1.0f) k2 = 1.0f;
				kn = 1.0f - k2;
				fogCol = (fogCol * kn + consts->params.fogColour3 * k2);

				//----

				
				for (int i = 0; i < 5; i++)
				{
					if (i == 0)
					{
						if (consts->params.mainLightIntensity > 0.0)
						{
							float3 shadowOutputTemp = MainShadow(consts, input);
							newColour += shadowOutputTemp * consts->params.mainLightColour * consts->params.mainLightIntensity;
						}
					}

#if _AuxLightsEnabled
					if (i > 0)
					{
						if (inBuff->lights[i - 1].enabled)
						{
							float3 d = inBuff->lights[i - 1].position - point;
							float distance = length(d);
							float distance2 = distance * distance;
							float3 lightVectorTemp = normalize(d);
							float3 light = AuxShadow(consts, input, distance, lightVectorTemp);
							float3 intensity = inBuff->lights[i - 1].intensity * 100.0f;
							newColour += light * inBuff->lights[i - 1].colour / distance2 * intensity;
						}
					}
#endif
				}

				
				//if (input.param->global_ilumination && !input.param->fastGlobalIllumination)
				//{
				//	sShaderOutput AO = AmbientOcclusion(input);
				//	newColour.R += AO.R * input.param->doubles.imageAdjustments.globalIlum;
				//	newColour.G += AO.G * input.param->doubles.imageAdjustments.globalIlum;
				//	newColour.B += AO.B * input.param->doubles.imageAdjustments.globalIlum;
				//}
				
				if (opacity > 1.0f) opacity = 1.0f;

				output = output * (1.0f - opacity) + newColour * opacity * fogCol;
				totalOpacity = opacity + (1.0f - opacity) * totalOpacity;
				alpha = opacity + (1.0f - opacity) * alpha;
			}
		}
		
		
		if(end) break;
	}

	alpha = min(1.0f, alpha);
	(*alphaOut) = alpha;
	totalOpacity = min(1.0f, totalOpacity);
	(*opacityOut) = totalOpacity;
	return output;
}



//------------------ MAIN RENDER FUNCTION --------------------
kernel void fractal3D(__global sClPixel *out, __global sClInBuff *inBuff, __constant sClInConstants *consts, __global sClReflect *reflectBuff, image2d_t imageBackground, int Gcl_offset)
{
	
	int cl_offset = Gcl_offset;
	const unsigned int i = get_global_id(0) + cl_offset;
	const unsigned int imageX = i % consts->params.width;
	const unsigned int imageY = i / consts->params.width;
	const unsigned int buffIndex = (i - cl_offset);
	
	const size_t local_id = get_local_id(0);
	const size_t local_size = get_local_size(0);
	const size_t group_id = get_group_id(0);
	const int local_offset = get_global_id(0) * 10;
	
	
	int seed = i + consts->params.randomSeed;
	for(int i=0; i<20; i++)
	{
		rand(&seed);
	}
	
	if(imageY < consts->params.height)
	{
		float2 screenPoint = (float2) {convert_float(imageX), convert_float(imageY)};
		float width = convert_float(consts->params.width);
		float height = convert_float(consts->params.height);
		float resolution = 1.0f/width;
		
		const float3 one = (float3) {1.0f, 0.0f, 0.0f};
		const float3 ones = 1.0f;
		
		matrix33 rot;
		rot.m1 = (float3){1.0f, 0.0f, 0.0f};
		rot.m2 = (float3){0.0f, 1.0f, 0.0f};
		rot.m3 = (float3){0.0f, 0.0f, 1.0f};
		rot = RotateZ(rot, consts->params.alpha);
		rot = RotateX(rot, consts->params.beta);
		rot = RotateY(rot, consts->params.gamma);
		
		float zBuff;
		//local sClStep stepBuff[MAX_RAYMARCHING];
		int buffCount;
		
		int maxRay = consts->params.reflectionsMax;		

		sClCalcParams calcParam;
		calcParam.N = consts->fractal.N;
		calcParam.orbitTrap = (float3){0.0f, 0.0f, 0.0f};
		calcParam.randomSeed = seed;
		calcParam.objectType = clObjFractal;
		
		float3 colour = 0.0f;
		float3 resultShader = 0.0f;
		
		
		//DOF effect
#if _DOFEnabled
		int blurIndex = 0;
		float3 totalColour = (float3) {0.0f, 0.0f, 0.0f};
		float focus = consts->params.DOFFocus;

		float randR = 0.001f * consts->params.DOFRadius*focus * sqrt(rand(&seed) / 65536.0 / 2.0f + 0.5f);
		float randAngle = rand(&seed);
		float randX = randR * sin(randAngle);
		float randZ = randR * cos(randAngle);
#else
		float randX = 0.0f;
		float randZ = 0.0f;
		float focus = 1.0f;
#endif //end DOFEnabled
		float3 start = 0.0f;
		if (consts->params.perspectiveType == 1 || consts->params.perspectiveType == 2)
		{
			float3 back = (float3)
			{ randX, 0.0f, randZ};
			start = consts->params.vp - Matrix33MulFloat3(rot, back);
		}
		else
		{
			float3 back = (float3) {randX/consts->params.zoom, 1.0f, randZ/consts->params.zoom } / consts->params.persp * consts->params.zoom;
			start = consts->params.vp - Matrix33MulFloat3(rot, back);
		}
		//printf("x %f, y %f, start %f %f %f\n", screenPoint.x, screenPoint.y, start.x, start.y, start.z);

		float aspectRatio = width / height;
		float x2,z2;
		x2 = (screenPoint.x / width - 0.5f) * aspectRatio;
		z2 = (screenPoint.y / height - 0.5f);

		float x3, z3;
		if (consts->params.perspectiveType == 1 || consts->params.perspectiveType == 2)
		{
			x3 = x2 + randX / focus / M_PI_F / consts->params.persp;
			z3 = z2 + randZ / focus / M_PI_F / consts->params.persp;
		}
		else
		{
			x3 = x2 + randX / focus / consts->params.persp / consts->params.persp;
			z3 = z2 + randZ / focus / consts->params.persp / consts->params.persp;
		}
		
		float3 viewVector;
		bool hemisphereCut = false;
		float fov = consts->params.persp;

		if (consts->params.perspectiveType == 1)//fish eye
		{
			if (consts->params.fishEyeCut && length((float2)
							{ x2, z2}) > 0.5f / fov)
			{
				hemisphereCut = true;
			}

			float x4 = x3 * M_PI_F;
			float z4 = z3 * M_PI_F;
			float r = length((float2)
					{ x4, z4});

			if(r == 0.0f)
			{
				viewVector.x = 0.0f;
				viewVector.z = 0.0f;
				viewVector.y = 1.0f;
			}
			else
			{
				viewVector.x = x4 / r * sin(r * fov);
				viewVector.z = z4 / r * sin(r * fov);
				viewVector.y = cos(r * fov);
			}
		}
		else if(consts->params.perspectiveType == 2) //equirectangular
		{
			float x4 = x3 * M_PI_F;
			float z4 = z3 * M_PI_F;

			viewVector.x = sin(fov * x4) * cos(fov * z4);
			viewVector.z = sin(fov * z4);
			viewVector.y = cos(fov * x4) * cos(fov * z4);
		}
		else //three-point perspective
		{
			viewVector = (float3)
			{ x3 * fov, 1.0f, z3 * fov};
		}

		//rotate vieviector by view angle
		viewVector = Matrix33MulFloat3(rot, viewVector);

		//ray-marching			
		float distThresh = 0.0f;
		float3 point = start;
		float3 startRay = start;
		float lastDist = 0.0f;
		bool found = false;
		float depth = 0.0f;
		int stepCount = 0;

		float3 objectColour = 0.0f;

		float3 lightVector = (float3)
		{
			cos(consts->params.mainLightAlfa - 0.5f * M_PI_F) * cos(-consts->params.mainLightBeta),
			sin(consts->params.mainLightAlfa - 0.5f * M_PI_F) * cos(-consts->params.mainLightBeta),
			sin(-consts->params.mainLightBeta)};
		lightVector = Matrix33MulFloat3(rot, lightVector);

		int rayEnd = 0;

		//printf("start ray-tracing\n");
		//printf("Max ray = %d\n", maxRay);
		for (int ray = 0; ray <= maxRay; ray++)
		{
			sClShaderInputData shaderInputData;
			reflectBuff[ray + local_offset].start = startRay;
			reflectBuff[ray + local_offset].viewVector = viewVector;
			reflectBuff[ray + local_offset].found = false;
			//printf("startRay %f %f %f\n", startRay.x, startRay.y, startRay.z);
			//printf("viewVector %f %f %f\n", viewVector.x, viewVector.y, viewVector.z);
			//reflectBuff[ray].buffCount = 0;

			point = RayMarching(consts, &calcParam, startRay, viewVector, consts->params.viewDistanceMax, true, &distThresh, &lastDist, &found, &depth, &stepCount);

			//printf("point %f %f %f\n", point.x, point.y, point.z);

			reflectBuff[ray + local_offset].depth = depth;
			reflectBuff[ray + local_offset].found = found;
			reflectBuff[ray + local_offset].lastDist = lastDist;
			reflectBuff[ray + local_offset].point = point;
			reflectBuff[ray + local_offset].distThresh = distThresh;
			reflectBuff[ray + local_offset].stepCount = stepCount;
			reflectBuff[ray + local_offset].objectType = calcParam.objectType;
			
			if(calcParam.objectType == clObjFractal) reflectBuff[ray + local_offset].reflect = consts->params.reflect;
#if _primitivePlane
			if(calcParam.objectType == clObjPlane) reflectBuff[ray + local_offset].reflect = consts->fractal.primitives.primitivePlaneReflect;
#endif
#if _primitiveBox
			if(calcParam.objectType == clObjBox) reflectBuff[ray + local_offset].reflect = consts->fractal.primitives.primitiveBoxReflect;
#endif
#if _primitiveInvertedBox
			if(calcParam.objectType == clObjBoxInv) reflectBuff[ray + local_offset].reflect = consts->fractal.primitives.primitiveInvertedBoxReflect;
#endif
#if _primitiveSphere
			if(calcParam.objectType == clObjSphere) reflectBuff[ray + local_offset].reflect = consts->fractal.primitives.primitiveSphereReflect;
#endif
#if _primitiveInvertedSphere
			if(calcParam.objectType == clObjSphereInv) reflectBuff[ray + local_offset].reflect = consts->fractal.primitives.primitiveInvertedSphereReflect;
#endif
#if _primitiveWater
			if(calcParam.objectType == clObjWater) reflectBuff[ray + local_offset].reflect = consts->fractal.primitives.primitiveWaterReflect;
#endif
			
			//printf("reflectBuff[ray].distThresh %f\n", reflectBuff[ray + local_offset].distThresh);

			rayEnd = ray;
			if(!found) break;
			if(reflectBuff[ray + local_offset].reflect == 0) break;

			//calculate new ray direction and start point
			startRay = point;

			shaderInputData.calcParam = &calcParam;
			shaderInputData.dist_thresh = distThresh;
			shaderInputData.lightVect = lightVector;
			shaderInputData.point = point;
			shaderInputData.viewVector = viewVector;
			if(consts->fractal.constantDEThreshold) shaderInputData.delta = depth * resolution * consts->params.persp;
			else shaderInputData.delta = distThresh * consts->params.quality;
			if(shaderInputData.delta < 1.0e-6) shaderInputData.delta = 1.0e-6;
			float3 vn = CalculateNormals(consts, &shaderInputData);
			viewVector = viewVector - vn * dot(viewVector,vn) * 2.0f;
			startRay = startRay + viewVector * distThresh;
		}

		float3 surfaceColour = 0.0f;
		float alphaChannel = 0.0f;
		float opacity = 0.0f;
		for(int ray = rayEnd; ray >= 0; ray--)
		{
			sClShaderInputData shaderInputData;
			float3 objectShader = 0.0f;
			float3 backgroudShader = 0.0f;
			float3 volumetricShader = 0.0f;
			float3 specular = 0.0f;

			calcParam.randomSeed = seed;
			shaderInputData.calcParam = &calcParam;

			shaderInputData.dist_thresh = reflectBuff[ray + local_offset].distThresh;

			if(consts->fractal.constantDEThreshold) shaderInputData.delta = reflectBuff[ray + local_offset].depth * resolution * consts->params.persp;
			else shaderInputData.delta = reflectBuff[ray + local_offset].distThresh * consts->params.quality;
			if(shaderInputData.delta < 1.0e-6) shaderInputData.delta = 1.0e-6;
			
			shaderInputData.lightVect = lightVector;
			shaderInputData.point = reflectBuff[ray + local_offset].point;
			shaderInputData.startPoint = reflectBuff[ray + local_offset].start;
			shaderInputData.eyePoint = start;
			shaderInputData.viewVector = reflectBuff[ray + local_offset].viewVector;
			shaderInputData.vectorsCount = consts->params.AmbientOcclusionNoOfVectors;
			shaderInputData.lastDist = reflectBuff[ray + local_offset].lastDist;
			shaderInputData.depth = reflectBuff[ray + local_offset].depth;
			shaderInputData.stepCount = reflectBuff[ray + local_offset].stepCount;
			shaderInputData.resolution = resolution;
			//shaderInputData.envMappingTexture = param.envmapTexture;
			shaderInputData.objectType = reflectBuff[ray + local_offset].objectType;

			if(reflectBuff[ray + local_offset].found)
			{
				//printf("Last dist %f = \n", lastDist / distThresh);	
				objectShader = ObjectShader(consts, &shaderInputData, &specular, &surfaceColour, inBuff);
			}
			else
			{
				backgroudShader = BackgroundShader(consts, &shaderInputData, imageBackground);
				reflectBuff[ray + local_offset].depth = 1e20f;
			}

			float reflect = reflectBuff[ray + local_offset].reflect;
			if (maxRay > 0 && rayEnd > 0 && ray != rayEnd)
			{
				colour = resultShader * reflect;
				colour += (1.0f - reflect) * (objectShader + backgroudShader) + specular;
			}
			else
			{
				colour = objectShader + backgroudShader + specular;
			}
			resultShader = VolumetricShader(consts, &shaderInputData, inBuff, colour, &opacity, &alphaChannel);
			//resultShader = colour;

			//seed = shaderInputData.calcParam->randomSeed;

		}
		zBuff = reflectBuff[0 + local_offset].depth;
		if(reflectBuff[0 + local_offset].found) alphaChannel = 1.0f;

		float3 finallColour = resultShader;
		
		sClPixel pixel;
		
		pixel.R = finallColour.x;
		pixel.G = finallColour.y;
		pixel.B = finallColour.z;
		pixel.zBuffer = zBuff;
		pixel.colR = convert_uchar(surfaceColour.x*255.0f);
		pixel.colG = convert_uchar(surfaceColour.y*255.0f);
		pixel.colB = convert_uchar(surfaceColour.z*255.0f);
		pixel.opacity = convert_ushort(opacity * 65535.0f);
		pixel.alpha = convert_ushort(alphaChannel * 65535.0f);
		
		out[buffIndex] = pixel;
	}
}

