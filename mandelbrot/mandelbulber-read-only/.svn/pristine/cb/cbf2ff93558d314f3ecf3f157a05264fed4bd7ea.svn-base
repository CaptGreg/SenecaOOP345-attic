/*********************************************************
 /                   MANDELBULBER
 / shaders for rendering engine
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

#ifndef SHADERS_H_
#define SHADERS_H_

#include "Render3D.h"

struct sShaderOutput
{
	double R;
	double G;
	double B;
	double alpha;
};

struct sLight
{
	CVector3 position;
	sRGB colour;
	double intensity;
	bool enabled;
};

struct sShaderInputData
{
	sParamRender *param;
	sFractal *calcParam;
	CVector3 point;
	CVector3 viewVector;
	CVector3 eyePoint;
	CVector3 normal;
	CVector3 lightVect;
	double dist_thresh;
	double lastDist;
	double delta;
	sVectorsAround *vectorsAround;
	int vectorsCount;
	cTexture *envMappingTexture;
	double depth;
	sStep *stepBuff;
	int stepCount;
	enumObjectType objectType;
};

extern sLight *Lights;
extern int lightsPlaced;

sShaderOutput ObjectShader(sShaderInputData input, sShaderOutput *surfaceColour, sShaderOutput *specularOut);
sShaderOutput BackgroundShader(sShaderInputData input);
sShaderOutput VolumetricShader(sShaderInputData input, sShaderOutput oldPixel, sShaderOutput *opacityOut);

sShaderOutput MainShadow(sShaderInputData &input);
sShaderOutput AmbientOcclusion(sShaderInputData &input);
sShaderOutput FastAmbientOcclusion(sShaderInputData &input);
sShaderOutput FastAmbientOcclusion2(sShaderInputData &input);
CVector3 CalculateNormals(sShaderInputData input);
sShaderOutput MainShading(sShaderInputData &input);
sShaderOutput MainSpecular(sShaderInputData &input);
sShaderOutput EnvMapping(sShaderInputData &input);
sShaderOutput SurfaceColour(sShaderInputData &input);
sShaderOutput TexturedBackground(sParamRender *param, CVector3 viewVector);
sShaderOutput LightShading(sShaderInputData &input, sLight light, int number, sShaderOutput *specular);
sShaderOutput AuxLightsShader(sShaderInputData &input, sShaderOutput *specularOut);
double AuxShadow(sShaderInputData &input, double distance, CVector3 lightVector);
sShaderOutput FakeLights(sShaderInputData &input, sShaderOutput *fakeSpec);
void PlaceRandomLights(sParamRender *fractParams, bool onlyPredefined);
double IterOpacity(double step, double iters, double maxN, double trim, double opacitySp);

sRGB IndexToColour(int index, sRGB *palette);
#endif /* SHADERS_H_ */
