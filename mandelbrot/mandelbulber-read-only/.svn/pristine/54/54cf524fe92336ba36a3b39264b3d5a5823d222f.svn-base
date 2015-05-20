/*********************************************************
 /                   MANDELBULBER
 / class for animation keyframe interpolation
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

#include <math.h>
#include <stdio.h>
#include "morph.hpp"

CMorph::CMorph(int size, int recordSize)
{
	count = size;
	dataSize = recordSize;
	dataSets.resize(count);
	for (int i = 0; i < count; i++)
		dataSets[i].resize(dataSize);
	output.resize(dataSize);
}

void CMorph::AddData(int index, double *data)
{
	if (index < count)
	{
		for (int i = 0; i < dataSize; i++)
		{
			dataSets[index][i] = data[i];
		}
	}
}

void CMorph::Linear(int frame, double *destData)
{
	int key = frame / framesPerKey;
	double factor = (frame % framesPerKey) / (double) framesPerKey;
	for (int i = 0; i < dataSize; i++)
	{
		double delta = dataSets[key + 1][i] - dataSets[key][i];
		output[i] = dataSets[key][i] + delta * factor;
		destData[i] = output[i];
	}
}

void CMorph::CatmullRom(int frame, double *destData)
{
	double v1, v2, v3, v4;
	int key = frame / framesPerKey;
	double factor = (frame % framesPerKey) / (double) framesPerKey;
	double factor2 = factor*factor;
	double factor3 = factor2*factor;
	for (int i = 0; i < dataSize; i++)
	{
		if (key >= 1) v1 = dataSets[key - 1][i];
		else v1 = dataSets[key][i];

		if (key < count) v2 = dataSets[key][i];
		else v2 = dataSets[count - 1][i];

		if (key < count - 1) v3 = dataSets[key + 1][i];
		else v3 = dataSets[count - 1][i];

		if (key < count - 2) v4 = dataSets[key + 2][i];
		else v4 = dataSets[count - 1][i];

		bool logaritmic = false;
		bool negative = false;
		if ((v1 > 0 && v2 > 0 && v3 > 0 && v4 > 0) || (v1 < 0 && v2 < 0 && v3 < 0 && v4 < 0))
		{
			if(v1<0) negative = true;
			//double totalVariation = (fabs(v2 - v1) + fabs(v3 - v2) + fabs(v4 - v3)) / 3.0;
			double average = (v1 + v2 + v3 + v4)/4.0;
			if (average > 0)
			{
				//double var1 = fabs(v2 - v1) / totalVariation;
				//double var2 = fabs(v3 - v2) / totalVariation;
				//double var3 = fabs(v4 - v3) / totalVariation;
				double deviation = (fabs(v2 - v1) + fabs(v3 - v2) + fabs(v4 - v3))/average;
				//if (var1 < 0.2 || var2 < 0.2 || var3 < 0.2)
				if(deviation>0.1)
				{
					v1 = log(fabs(v1));
					v2 = log(fabs(v2));
					v3 = log(fabs(v3));
					v4 = log(fabs(v4));
					logaritmic = true;
				}
			}
		}
		double value = 0.5 * ((2 * v2) + (-v1 + v3) * factor + (2 * v1 - 5 * v2 + 4 * v3 - v4) * factor2 + (-v1 + 3 * v2 - 3 * v3 + v4) * factor3);
		if(logaritmic)
		{
			if(negative) value = -exp(value);
			else value = exp(value);
		}
		if(value > 1e20) value = 1e20;
		if(value < -1e20) value = 1e20;
		if(fabs(value) < 1e-20) value = 0.0;

		//printf("Morph value = %g\n", value);

		output[i]=value;
		destData[i] = output[i];
	}
}
