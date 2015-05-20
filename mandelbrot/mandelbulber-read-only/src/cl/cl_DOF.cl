//*********************************************************
//                   MANDELBULBER
// kernel for DOF effect
// 
//
// author: Krzysztof Marczak
// contact: buddhi1980@gmail.com
// licence: GNU GPL v3.0
//
//*********************************************************

typedef struct
{
	int width;
	int height;
	float focus;
	float radius;
} sParamsDOF;

typedef struct
{
	float z;
	int i;
} sSortZ;

//------------------ MAIN RENDER FUNCTION --------------------
kernel void DOF(__global ushort4 *in_image, __global ushort4 *out_image, __global sSortZ *zBuffer, sParamsDOF p)
{
	const unsigned int i = get_global_id(0);

	uint index = p.height * p.width - i - 1;
	int ii = zBuffer[index].i;

	int2 scr = (int2){ii % p.width, ii / p.width};
	float z = zBuffer[index].z;
	float blur = fabs(z - p.focus) / z * p.radius;
	blur = min(blur, 500.0f);
	float4 center = convert_float4(in_image[scr.x + scr.y * p.width]);
	//float4 center = read_imagef(in_image, scr);
	float factor = blur * blur * sqrt(blur)* M_PI_F/3.0f;
	int blurInt = (int)blur;
	
	int2 scr2;
	int2 start = (int2){scr.x - blurInt, scr.y - blurInt};
	start = max(start, 0);
	int2 end = (int2){scr.x + blurInt, scr.y + blurInt};
	end = min(end, (int2){p.width - 1, p.height - 1});

	for (scr2.y = start.y; scr2.y <= end.y; scr2.y++)
	{
		for(scr2.x = start.x; scr2.x <= end.x; scr2.x++)
		{
		//	if(scr2.x >= 0 && scr2.x < p.width && scr2.y >=0 && scr2.y < p.height)
			//{
			//clamp(scr2, 0, (int2){p.width, p.height});
				float2 d = convert_float2(scr - scr2);
				float r = length(d);
				float op = (blur - r) / factor;
				op = clamp(op, 0.0f, 1.0f);
				float opN = 1.0f - op;
				uint address = scr2.x + scr2.y * p.width;
				float4 old = convert_float4(out_image[address]);
				out_image[address] = convert_ushort4(opN * old + op * center);
				//float4 old = read_imagef(out_image, scr2);
				//float4 newPixel = opN * old + op * center;
				//write_imagef(out_image, scr2, newPixel);
			//}
		}
	}
}

