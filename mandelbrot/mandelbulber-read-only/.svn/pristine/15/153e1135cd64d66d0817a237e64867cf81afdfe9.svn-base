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

#include <cstdlib>

#include "shaders.h"
#include "interface.h"

sLight *Lights;
int lightsPlaced = 0;


sShaderOutput ObjectShader(sShaderInputData input, sShaderOutput *surfaceColour, sShaderOutput *specularOut)
{
	sShaderOutput output;

	//normal vector
	CVector3 vn = CalculateNormals(input);
	input.normal = vn;

	sShaderOutput mainLight;
	mainLight.R = input.param->doubles.imageAdjustments.mainLightIntensity * input.param->effectColours.mainLightColour.R / 65536.0;
	mainLight.G = input.param->doubles.imageAdjustments.mainLightIntensity * input.param->effectColours.mainLightColour.G / 65536.0;
	mainLight.B = input.param->doubles.imageAdjustments.mainLightIntensity * input.param->effectColours.mainLightColour.B / 65536.0;

	//calculate shading based on angle of incidence
	sShaderOutput shade = MainShading(input);
	shade.R = input.param->doubles.imageAdjustments.directLight * ((1.0 - input.param->doubles.imageAdjustments.shading) + input.param->doubles.imageAdjustments.shading * shade.R);
	shade.G = input.param->doubles.imageAdjustments.directLight * ((1.0 - input.param->doubles.imageAdjustments.shading) + input.param->doubles.imageAdjustments.shading * shade.G);
	shade.B = input.param->doubles.imageAdjustments.directLight * ((1.0 - input.param->doubles.imageAdjustments.shading) + input.param->doubles.imageAdjustments.shading * shade.B);

	//calculate shadow
	sShaderOutput shadow = {1.0, 1.0, 1.0, 0.0};
	if(input.param->shadow)	shadow = MainShadow(input);

	//calculate specular highlight
	sShaderOutput specular = MainSpecular(input);
	specular.R *= input.param->doubles.imageAdjustments.specular;
	specular.G *= input.param->doubles.imageAdjustments.specular;
	specular.B *= input.param->doubles.imageAdjustments.specular;

	//calculate surface colour
	sShaderOutput colour = SurfaceColour(input);
	*surfaceColour = colour;

	//ambient occlusion
	sShaderOutput ambient = {0.0, 0.0, 0.0, 0.0};
	if(input.param->global_ilumination)
	{
		//fast mode
		if(input.param->fastGlobalIllumination)
		{
			ambient = FastAmbientOcclusion2(input);
		}
		else
		{
			ambient = AmbientOcclusion(input);
		}
	}
	sShaderOutput ambient2;
	ambient2.R = ambient.R * input.param->doubles.imageAdjustments.globalIlum;
	ambient2.G = ambient.G * input.param->doubles.imageAdjustments.globalIlum;
	ambient2.B = ambient.B * input.param->doubles.imageAdjustments.globalIlum;

	//environment mapping
	sShaderOutput envMapping = {0.0, 0.0, 0.0, 0.0};
	if(!input.param->imageSwitches.raytracedReflections && input.param->doubles.imageAdjustments.reflect > 0)
	{
		envMapping = EnvMapping(input);
	}
	envMapping.R *= input.param->doubles.imageAdjustments.reflect;
	envMapping.G *= input.param->doubles.imageAdjustments.reflect;
	envMapping.B *= input.param->doubles.imageAdjustments.reflect;

	//additional lights
	sShaderOutput auxLights;
	sShaderOutput auxLightsSpecular;
	auxLights = AuxLightsShader(input, &auxLightsSpecular);

	//fake orbit trap lights
	sShaderOutput fakeLights = {0.0, 0.0, 0.0, 0.0};
	sShaderOutput fakeLightsSpecular = {0.0, 0.0, 0.0, 0.0};
	if(input.param->fakeLightsEnabled)
	{
		fakeLights = FakeLights(input, &fakeLightsSpecular);
	}

	//total shader
	output.R = envMapping.R * ambient.R + (ambient2.R + mainLight.R * shade.R * shadow.R) * colour.R;
	output.G = envMapping.G * ambient.G + (ambient2.G + mainLight.G * shade.G * shadow.G) * colour.G;
	output.B = envMapping.B * ambient.B + (ambient2.B + mainLight.B * shade.B * shadow.B) * colour.B;

	output.R += (auxLights.R + fakeLights.R) * colour.R;
	output.G += (auxLights.G + fakeLights.G) * colour.G;
	output.B += (auxLights.B + fakeLights.B) * colour.B;
	output.alpha = 1.0;

	(*specularOut).R = auxLightsSpecular.R + fakeLightsSpecular.R + mainLight.R * specular.R * shadow.R;
	(*specularOut).G = auxLightsSpecular.G + fakeLightsSpecular.G + mainLight.G * specular.G * shadow.G;
	(*specularOut).B = auxLightsSpecular.B + fakeLightsSpecular.B + mainLight.B * specular.B * shadow.B;
	(*specularOut).alpha = output.alpha;

	return output;
}

sShaderOutput BackgroundShader(sShaderInputData input)
{
	sShaderOutput pixel2;

	if (input.param->texturedBackground)
	{
		if(input.param->background_as_fulldome)
		{
			double alfaTexture = input.viewVector.GetAlpha();
			double betaTexture = input.viewVector.GetBeta();
			int texWidth = input.param->backgroundTexture->Width()*0.5;
			int texHeight = input.param->backgroundTexture->Height();
			int offset = 0;

			if(betaTexture < 0)
			{
				betaTexture = -betaTexture;
				alfaTexture += M_PI;
				offset = texWidth;
			}
			double texX = 0.5 * texWidth + cos(alfaTexture) * (1.0 - betaTexture / (0.5 * M_PI)) * texWidth * 0.5 + offset;
			double texY = 0.5 * texHeight + sin(alfaTexture) * (1.0 - betaTexture / (0.5 * M_PI)) * texHeight * 0.5;
			sRGB8 pixel = input.param->backgroundTexture->Pixel(texX, texY);
			pixel2.R = pixel.R / 256.0;
			pixel2.G = pixel.G / 256.0;
			pixel2.B = pixel.B / 256.0;
		}
		else
		{
			double alfaTexture = fmod(input.viewVector.GetAlpha() + 2.5 * M_PI, 2 * M_PI);
			double betaTexture = input.viewVector.GetBeta();
			if (betaTexture > 0.5 * M_PI) betaTexture = 0.5 * M_PI - betaTexture;
			if (betaTexture < -0.5 * M_PI) betaTexture = -0.5 * M_PI + betaTexture;
			double texX = alfaTexture / (2.0 * M_PI) * input.param->backgroundTexture->Width();
			double texY = (betaTexture / (M_PI) + 0.5) * input.param->backgroundTexture->Height();
			sRGB8 pixel = input.param->backgroundTexture->Pixel(texX, texY);
			pixel2.R = pixel.R/256.0;
			pixel2.G = pixel.G/256.0;
			pixel2.B = pixel.B/256.0;
		}
	}
	else
	{
		CVector3 vector(0.0, 0.0, -1.0);
		vector.Normalize();
		double grad = (input.viewVector.Dot(vector)+1.0);
		sRGB16 pixel;
		if(grad < 1)
		{
			double Ngrad = 1.0 - grad;
			pixel.R = (input.param->background_color3.R * Ngrad + input.param->background_color2.R * grad);
			pixel.G = (input.param->background_color3.G * Ngrad + input.param->background_color2.G * grad);
			pixel.B = (input.param->background_color3.B * Ngrad + input.param->background_color2.B * grad);
		}
		else
		{
			grad = grad - 1;
			double Ngrad = 1.0 - grad;
			pixel.R = (input.param->background_color2.R * Ngrad + input.param->background_color1.R * grad);
			pixel.G = (input.param->background_color2.G * Ngrad + input.param->background_color1.G * grad);
			pixel.B = (input.param->background_color2.B * Ngrad + input.param->background_color1.B * grad);
		}

		pixel2.R = pixel.R/65536.0;
		pixel2.G = pixel.G/65536.0;
		pixel2.B = pixel.B/65536.0;
		pixel2.alpha = 0.0;
	}
	return pixel2;
}

sShaderOutput VolumetricShader(sShaderInputData input, sShaderOutput oldPixel, sShaderOutput *opacityOut)
{
	sShaderOutput output;
	float totalOpacity = 0.0;

	output.R = oldPixel.R;
	output.G = oldPixel.G;
	output.B = oldPixel.B;
	output.alpha = oldPixel.alpha;

	//basic fog init
  double fogVisibility = pow(10.0, input.param->doubles.imageAdjustments.fogVisibility / 10 - 16.0);

  //volumetric fog init
	double colourThresh = input.param->doubles.fogColour1Distance;
	double colourThresh2 = input.param->doubles.fogColour2Distance;
	double fogReduce = input.param->doubles.fogDistanceFactor;
	double fogIntensity = input.param->doubles.fogDensity;

	//visible lights init
	int numberOfLights = lightsPlaced;
	if (numberOfLights < 4) numberOfLights = 4;

	//glow init
  double glow = input.stepCount * input.param->doubles.imageAdjustments.glow_intensity / 512.0 * input.param->doubles.DE_factor;
  double glowN = 1.0 - glow;
  if (glowN < 0.0) glowN = 0.0;
  double glowR = (input.param->effectColours.glow_color1.R * glowN + input.param->effectColours.glow_color2.R * glow) / 65536.0;
  double glowG = (input.param->effectColours.glow_color1.G * glowN + input.param->effectColours.glow_color2.G * glow) / 65536.0;
  double glowB = (input.param->effectColours.glow_color1.B * glowN + input.param->effectColours.glow_color2.B * glow) / 65536.0;

  double totalStep = 0.0;
	for(int index = input.stepCount-1; index >0; index--)
	{
		double step = input.stepBuff[index].step;
		double distance = input.stepBuff[index].distance;
		CVector3 point = input.stepBuff[index].point;
		totalStep += step;

		input.point = point;
		input.dist_thresh = input.stepBuff[index].distThresh;

		if(totalStep < input.dist_thresh * input.param->doubles.quality)
		{
			continue;
		}
		step = totalStep;
		totalStep = 0.0;

		//------------------- glow
		double glowOpacity = glow * step / input.depth;
		output.R = glowOpacity * glowR + (1.0 - glowOpacity) * output.R;
		output.G = glowOpacity * glowG + (1.0 - glowOpacity) * output.G;
		output.B = glowOpacity * glowB + (1.0 - glowOpacity) * output.B;
		output.alpha += glowOpacity;

		//------------------ visible light
		if (input.param->doubles.auxLightVisibility > 0)
		{
			double lowestLightSize = 1e10;
			double lowestLightDist = 1e10;
			for (int i = 0; i < numberOfLights; ++i)
			{
				if (i < input.param->auxLightNumber || Lights[i].enabled)
				{
					CVector3 lightDistVect = point - Lights[i].position;
					double lightDist = lightDistVect.Length();
					double lightSize = Lights[i].intensity * input.param->doubles.auxLightIntensity * input.param->doubles.auxLightVisibility;
					double r2 = lightDist / lightSize;
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
			double step2 = step;

			smallSteps = 10.0 * step / (lowestLightDist + 1.0e-20);
			if(smallSteps > 50) smallSteps = 50;
			if (smallSteps > 0)
			{
				smallStep_start = 1;
				smallStep_end = smallSteps + 1;
				step2 = step / (smallSteps + 1.0);
			}

			for (int smallStep = smallStep_start; smallStep < smallStep_end; smallStep++)
			{
				CVector3 point2 = point - input.viewVector * step2 * smallStep;

				for (int i = 0; i < numberOfLights; ++i)
				{
					if (i < input.param->auxLightNumber || Lights[i].enabled)
					{
						CVector3 lightDistVect = point2 - Lights[i].position;
						double lightDist = lightDistVect.Length();
						double lightSize = Lights[i].intensity * input.param->doubles.auxLightIntensity * input.param->doubles.auxLightVisibility;
						double r2 = lightDist / lightSize;
						if (r2 > 1.0) r2 = 1.0;
						double bellFunction = (cos(r2 * M_PI) + 1.0) / (r2 * r2 + 0.02) * 0.3;
						double lightDensity = step2 * bellFunction / lightSize;

						output.R += lightDensity * Lights[i].colour.R / 65536.0;
						output.G += lightDensity * Lights[i].colour.G / 65536.0;
						output.B += lightDensity * Lights[i].colour.B / 65536.0;
						output.alpha += lightDensity;
					}
				}
			}
		}

		//fake lights (orbit trap)
		if(input.param->fakeLightsEnabled)
		{
			double r = Compute<orbitTrap>(point, *input.calcParam);
			r = sqrt(1.0f/(r + 1.0e-30f));
			double fakeLight = 1.0 / (pow(r, 10.0 / input.param->doubles.fakeLightsVisibilitySize) * pow(10.0, 10.0 / input.param->doubles.fakeLightsVisibilitySize) + 1e-100);
			output.R += fakeLight * step * input.param->doubles.fakeLightsVisibility;
			output.G += fakeLight * step * input.param->doubles.fakeLightsVisibility;
			output.B += fakeLight * step * input.param->doubles.fakeLightsVisibility;
			output.alpha += fakeLight * step * input.param->doubles.fakeLightsVisibility;
		}

		//---------------------- volumetric lights with shadows in fog

		for (int i = 0; i < 5; i++)
		{
			if (i == 0 && input.param->volumetricLightEnabled[0])
			{
				sShaderOutput shadowOutputTemp = MainShadow(input);
				output.R += shadowOutputTemp.R * step * input.param->doubles.volumetricLightIntensity[0] * input.param->effectColours.mainLightColour.R / 65536.0;
				output.G += shadowOutputTemp.G * step * input.param->doubles.volumetricLightIntensity[0] * input.param->effectColours.mainLightColour.G / 65536.0;
				output.B += shadowOutputTemp.B * step * input.param->doubles.volumetricLightIntensity[0] * input.param->effectColours.mainLightColour.B / 65536.0;
				output.alpha += (shadowOutputTemp.R + shadowOutputTemp.G + shadowOutputTemp.B)/3.0 * step * input.param->doubles.volumetricLightIntensity[0];
			}
			if (i > 0)
			{
				if (Lights[i - 1].enabled && input.param->volumetricLightEnabled[i])
				{
					CVector3 d = Lights[i - 1].position - point;
					double distance = d.Length();
					double distance2 = distance * distance;
					CVector3 lightVectorTemp = d;
					lightVectorTemp.Normalize();
					double light = AuxShadow(input, distance, lightVectorTemp);
					output.R += light * Lights[i - 1].colour.R / 65536.0 * input.param->doubles.volumetricLightIntensity[i] * step / distance2;
					output.G += light * Lights[i - 1].colour.G / 65536.0 * input.param->doubles.volumetricLightIntensity[i] * step / distance2;
					output.B += light * Lights[i - 1].colour.B / 65536.0 * input.param->doubles.volumetricLightIntensity[i] * step / distance2;
					output.alpha += light * input.param->doubles.volumetricLightIntensity[i] * step / distance2;
				}
			}
		}

		//----------------------- basic fog
		if(input.param->imageSwitches.fogEnabled)
		{
			double fogDensity = step / fogVisibility;
			if(fogDensity > 1.0) fogDensity = 1.0;
			output.R = fogDensity * input.param->effectColours.fogColor.R / 65536.0 + (1.0 - fogDensity) * output.R;
			output.G = fogDensity * input.param->effectColours.fogColor.G / 65536.0 + (1.0 - fogDensity) * output.G;
			output.B = fogDensity * input.param->effectColours.fogColor.B / 65536.0 + (1.0 - fogDensity) * output.B;
			totalOpacity = fogDensity + (1.0 - fogDensity) * totalOpacity;
			output.alpha = fogDensity + (1.0 - fogDensity) * output.alpha;
		}

		//-------------------- volumetric fog
		if(fogIntensity > 0.0)
		{
			double densityTemp = (step * fogReduce) / (distance * distance + fogReduce * fogReduce);

			double k = distance / colourThresh;
			if (k > 1) k = 1.0;
			double kn = 1.0 - k;
			double fogRtemp = (input.param->fogColour1.R * kn + input.param->fogColour2.R * k);
			double fogGtemp = (input.param->fogColour1.G * kn + input.param->fogColour2.G * k);
			double fogBtemp = (input.param->fogColour1.B * kn + input.param->fogColour2.B * k);

			double k2 = distance / colourThresh2 * k;
			if (k2 > 1) k2 = 1.0;
			kn = 1.0 - k2;
			fogRtemp = (fogRtemp * kn + input.param->fogColour3.R * k2);
			fogGtemp = (fogGtemp * kn + input.param->fogColour3.G * k2);
			fogBtemp = (fogBtemp * kn + input.param->fogColour3.B * k2);

			double fogDensity = 0.3 * fogIntensity * densityTemp / (1.0 + fogIntensity * densityTemp);
			if(fogDensity > 1) fogDensity = 1.0;

			output.R = fogDensity * fogRtemp / 65536.0 + (1.0 - fogDensity) * output.R;
			output.G = fogDensity * fogGtemp / 65536.0 + (1.0 - fogDensity) * output.G;
			output.B = fogDensity * fogBtemp / 65536.0 + (1.0 - fogDensity) * output.B;

			totalOpacity = fogDensity + (1.0 - fogDensity) * totalOpacity;
			output.alpha = fogDensity + (1.0 - fogDensity) * output.alpha;
		}

		//iter fog
		if (input.param->imageSwitches.iterFogEnabled)
		{
			int L = input.stepBuff[index].iters;
			double opacity = IterOpacity(step, L, input.param->fractal.doubles.N, input.param->doubles.iterFogOpacityTrim, input.param->doubles.iterFogOpacity);

			sShaderOutput newColour = { 0.0, 0.0, 0.0 };
			if (opacity > 0)
			{
				//fog colour
				double iterFactor = (double) 2.0 * (L - input.param->doubles.iterFogOpacityTrim) / (input.param->fractal.doubles.N - input.param->doubles.iterFogOpacityTrim);
				double k = iterFactor;
				if (k > 1.0) k = 1.0;
				if (k < 0.0) k = 0.0;
				double kn = 1.0 - k;
				double fogColR = (input.param->fogColour1.R * kn + input.param->fogColour2.R * k);
				double fogColG = (input.param->fogColour1.G * kn + input.param->fogColour2.G * k);
				double fogColB = (input.param->fogColour1.B * kn + input.param->fogColour2.B * k);

				double k2 = iterFactor - 1.0;
				if (k2 < 0.0) k2 = 0.0;
				if (k2 > 1.0) k2 = 1.0;
				kn = 1.0 - k2;
				fogColR = (fogColR * kn + input.param->fogColour3.R * k2);
				fogColG = (fogColG * kn + input.param->fogColour3.G * k2);
				fogColB = (fogColB * kn + input.param->fogColour3.B * k2);
				//----

				for (int i = 0; i < 5; i++)
				{
					if (i == 0)
					{
						if (input.param->doubles.imageAdjustments.mainLightIntensity * input.param->doubles.imageAdjustments.directLight > 0.0)
						{
							sShaderOutput shadowOutputTemp = MainShadow(input);
							newColour.R += shadowOutputTemp.R * input.param->effectColours.mainLightColour.R / 65536.0 * input.param->doubles.imageAdjustments.mainLightIntensity
									* input.param->doubles.imageAdjustments.directLight;
							newColour.G += shadowOutputTemp.G * input.param->effectColours.mainLightColour.G / 65536.0 * input.param->doubles.imageAdjustments.mainLightIntensity
									* input.param->doubles.imageAdjustments.directLight;
							newColour.B += shadowOutputTemp.B * input.param->effectColours.mainLightColour.B / 65536.0 * input.param->doubles.imageAdjustments.mainLightIntensity
									* input.param->doubles.imageAdjustments.directLight;
						}
					}

					if (i > 0)
					{
						if (Lights[i - 1].enabled)
						{
							CVector3 d = Lights[i - 1].position - point;
							double distance = d.Length();
							double distance2 = distance * distance;
							CVector3 lightVectorTemp = d;
							lightVectorTemp.Normalize();
							double light = AuxShadow(input, distance, lightVectorTemp);
							double intensity = Lights[i - 1].intensity * 100.0;
							newColour.R += light * Lights[i - 1].colour.R / 65536.0 / distance2 * intensity;
							newColour.G += light * Lights[i - 1].colour.G / 65536.0 / distance2 * intensity;
							newColour.B += light * Lights[i - 1].colour.B / 65536.0 / distance2 * intensity;
						}
					}

				}

				if (input.param->global_ilumination && !input.param->fastGlobalIllumination)
				{
					sShaderOutput AO = AmbientOcclusion(input);
					newColour.R += AO.R * input.param->doubles.imageAdjustments.globalIlum;
					newColour.G += AO.G * input.param->doubles.imageAdjustments.globalIlum;
					newColour.B += AO.B * input.param->doubles.imageAdjustments.globalIlum;
				}

				if (opacity > 1.0) opacity = 1.0;

				output.R = output.R * (1.0 - opacity) + newColour.R * opacity * fogColR / 65536.0;
				output.G = output.G * (1.0 - opacity) + newColour.G * opacity * fogColG / 65536.0;
				output.B = output.B * (1.0 - opacity) + newColour.B * opacity * fogColB / 65536.0;
				totalOpacity = opacity + (1.0 - opacity) * totalOpacity;
				output.alpha = opacity + (1.0 - opacity) * output.alpha;
			}
		}


		if(totalOpacity > 1.0) totalOpacity = 1.0;
		if(output.alpha > 1.0) output.alpha = 1.0;
		(*opacityOut).R = totalOpacity;
		(*opacityOut).G = totalOpacity;
		(*opacityOut).B = totalOpacity;

	} //next stepCount


	return output;
}

sShaderOutput MainShadow(sShaderInputData &input)
{
	sShaderOutput shadow = { 1.0, 1.0, 1.0 };

	//starting point
	CVector3 point2;

	bool max_iter;
	double factor = input.delta / input.param->doubles.resolution;
	if (!input.param->penetratingLights) factor = input.param->doubles.viewDistanceMax;
	double dist = input.dist_thresh;
	input.calcParam->doubles.detailSize = input.dist_thresh;

	double DE_factor = input.param->doubles.DE_factor;
	if(input.param->imageSwitches.iterFogEnabled || input.param->imageSwitches.volumetricLightEnabled) DE_factor = 1.0;

	//double start = input.delta;
	double start = input.dist_thresh;
	if (input.calcParam->interiorMode) start = input.dist_thresh * DE_factor;

	double opacity = 0.0;
	double shadowTemp = 1.0;

	double softRange = tan(input.param->doubles.shadowConeAngle / 180.0 * M_PI);
	double maxSoft = 0.0;

	double bSoft = (!input.param->imageSwitches.iterFogEnabled && !input.calcParam->limits_enabled && !input.calcParam->iterThresh) && softRange > 0.0;

	for (double i = start; i < factor; i += dist * DE_factor)
	{
		point2 = input.point + input.lightVect * i;

		float dist_thresh;
		if(input.param->imageSwitches.iterFogEnabled || input.param->imageSwitches.volumetricLightEnabled)
		{
			if(!input.calcParam->constantDEThreshold)
				dist_thresh = (point2 - input.eyePoint).Length() * input.param->doubles.resolution * input.param->doubles.persp / input.param->doubles.quality;
			else
				dist_thresh = input.param->doubles.quality;
		}
		else
			dist_thresh = input.dist_thresh;

		input.calcParam->doubles.detailSize = dist_thresh;
		dist = CalculateDistance(point2, *input.calcParam, &max_iter);


		if (bSoft)
		{
			double angle = (dist - dist_thresh) / i;
			if (angle < 0) angle = 0;
			if (dist < dist_thresh) angle = 0;
			double softShadow = (1.0 - angle / softRange);
			if (input.param->penetratingLights) softShadow *= (factor - i) / factor;
			if (softShadow < 0) softShadow = 0;
			if (softShadow > maxSoft) maxSoft = softShadow;
		}

		if (input.param->imageSwitches.iterFogEnabled)
		{
			opacity = IterOpacity(dist * DE_factor, input.calcParam->itersOut, input.calcParam->doubles.N, input.param->doubles.iterFogOpacityTrim,
					input.param->doubles.iterFogOpacity);
		}
		else
		{
			opacity = 0.0;
		}
		shadowTemp -= opacity * (factor - i) / factor;

		if (dist < dist_thresh || shadowTemp < 0.0)
		{
			shadowTemp -= (factor - i) / factor;
			if (!input.param->penetratingLights) shadowTemp = 0.0;
			if (shadowTemp < 0.0) shadowTemp = 0.0;
			break;
		}
	}
	if (!bSoft)
	{
		shadow.R = shadowTemp;
		shadow.G = shadowTemp;
		shadow.B = shadowTemp;
	}
	else
	{
		shadow.R = (1.0 - maxSoft);
		shadow.G = (1.0 - maxSoft);
		shadow.B = (1.0 - maxSoft);
	}
	return shadow;
}

sShaderOutput FastAmbientOcclusion(sShaderInputData &input)
{
	sShaderOutput output;
	double min_radius = Compute<fake_AO>(input.point, *input.calcParam);
	double j = (min_radius - 0.65) * 1.0; //0.65
	if (j > 1.0) j = 1.0;
	if (j < 0) j = 0;
	output.R = j;
	output.G = j;
	output.B = j;
	return output;
}

sShaderOutput FastAmbientOcclusion2(sShaderInputData &input)
{
	//reference: http://www.iquilezles.org/www/material/nvscene2008/rwwtt.pdf (Iñigo Quilez – iq/rgba)

	double delta = input.dist_thresh;
	double aoTemp = 0;
	double quality = input.param->globalIlumQuality;
	for (int i = 1; i < quality * quality; i++)
	{
		double scan = i * i * delta;
		CVector3 pointTemp = input.point + input.normal * scan;
		bool max_iter;
		double dist = CalculateDistance(pointTemp, *input.calcParam, &max_iter);
		aoTemp += 1.0 / (pow(2.0, i)) * (scan - input.param->doubles.fastAoTune * dist) / input.dist_thresh;
		//if(dist < 0.5*dist_thresh) break;
	}
	double ao = 1.0 - 0.2 * aoTemp;
	if (ao < 0) ao = 0;
	sShaderOutput output = { ao, ao, ao };
	return output;
}

sShaderOutput AmbientOcclusion(sShaderInputData &input)
{
	sShaderOutput AO = { 0, 0, 0 };

	bool max_iter;
	double start_dist = input.delta;
	double end_dist = input.delta / input.param->doubles.resolution;
	double intense = 0;

	for (int i = 0; i < input.vectorsCount; i++)
	{
		sVectorsAround v = input.vectorsAround[i];

		double dist = input.lastDist;

		double opacity = 0.0;
		double shadowTemp = 1.0;

		for (double r = start_dist; r < end_dist; r += dist * 2.0)
		{
			CVector3 point2 = input.point + v.v * r;

			dist = CalculateDistance(point2, *input.calcParam, &max_iter);

			if (input.param->imageSwitches.iterFogEnabled)
			{
				opacity = IterOpacity(dist * 2.0, input.calcParam->itersOut, input.calcParam->doubles.N, input.param->doubles.iterFogOpacityTrim,
						input.param->doubles.iterFogOpacity);
			}
			else
			{
				opacity = 0.0;
			}
			shadowTemp -= opacity * (end_dist - r) / end_dist;

			float dist_thresh;
			if(input.param->imageSwitches.iterFogEnabled || input.param->imageSwitches.volumetricLightEnabled)
			{
				if(!input.calcParam->constantDEThreshold)
					dist_thresh = (point2 - input.eyePoint).Length() * input.param->doubles.resolution * input.param->doubles.persp / input.param->doubles.quality;
				else
					dist_thresh = input.param->doubles.quality;
			}
			else
				dist_thresh = input.dist_thresh;

			if (dist < dist_thresh || max_iter || shadowTemp < 0.0)
			{
				shadowTemp -= (end_dist - r) / end_dist;
				if (shadowTemp < 0.0) shadowTemp = 0.0;
				break;
			}
		}

		intense = shadowTemp;

		AO.R += intense * v.R;
		AO.G += intense * v.G;
		AO.B += intense * v.B;

	}
	AO.R /= (input.vectorsCount * 256.0);
	AO.G /= (input.vectorsCount * 256.0);
	AO.B /= (input.vectorsCount * 256.0);

	return AO;
}

CVector3 CalculateNormals(sShaderInputData input)
{
	CVector3 normal(0, 0, 0);
	//calculating normal vector based on distance estimation (gradient of distance function)
	if (!input.param->slowShading)
	{
		//calcParam->DE_threshold = 0;
		//double delta = param->resolution * param->zoom * wsp_persp;
		double delta = input.delta * input.param->doubles.smoothness;
		if(input.calcParam->interiorMode) delta = input.dist_thresh * input.param->doubles.quality * 0.2 * input.param->doubles.smoothness;

		double s1, s2, s3, s4;
		input.calcParam->doubles.N = input.param->fractal.doubles.N * 5;
		input.calcParam->minN = 0;
		input.calcParam->normalCalculationMode = true;

		bool maxIter;

		s1 = CalculateDistance(input.point, *input.calcParam, &maxIter);

		CVector3 deltax(delta, 0.0, 0.0);
		s2 = CalculateDistance(input.point + deltax, *input.calcParam, &maxIter);

		CVector3 deltay(0.0, delta, 0.0);
		s3 = CalculateDistance(input.point + deltay, *input.calcParam, &maxIter);

		CVector3 deltaz(0.0, 0.0, delta);
		s4 = CalculateDistance(input.point + deltaz, *input.calcParam, &maxIter);

		normal.x = s2 - s1;
		normal.y = s3 - s1;
		normal.z = s4 - s1;
		input.calcParam->doubles.N = input.param->fractal.doubles.N;
		input.calcParam->minN = input.param->fractal.minN;
		input.calcParam->normalCalculationMode = false;
		//calcParam->DE_threshold = DEthreshold_temp;
	}

	//calculating normal vector based on average value of binary central difference
	else
	{
		double result2;
		bool max_iter;

		CVector3 point2;
		CVector3 point3;
		input.calcParam->normalCalculationMode = true;

		double delta = input.delta * input.param->doubles.smoothness;
		input.calcParam->doubles.N = input.param->fractal.doubles.N * 5;
		if(input.calcParam->interiorMode) delta = input.dist_thresh * input.param->doubles.quality * 0.2 * input.param->doubles.smoothness;

		for (point2.x = -1.0; point2.x <= 1.0; point2.x += 0.2) //+0.2
		{
			for (point2.y = -1.0; point2.y <= 1.0; point2.y += 0.2)
			{
				for (point2.z = -1.0; point2.z <= 1.0; point2.z += 0.2)
				{
					point3 = input.point + point2 * delta;

					double dist = CalculateDistance(point3, *input.calcParam, &max_iter);

					//if (dist < input.dist_thresh || max_iter) result2 = 0.0;
					//else result2 = 1.0;

					normal += (point2 * dist);
				}
			}
		}
		input.calcParam->normalCalculationMode = false;
		input.calcParam->doubles.N = input.param->fractal.doubles.N;
	}
	if(normal.x == 0 && normal.y == 0 && normal.z == 0)
	{
		normal.x = 1;
	}
	else
	{
		normal.Normalize();
	}

	return normal;
}

sShaderOutput MainShading(sShaderInputData &input)
{
	sShaderOutput shading;
	double shade = input.normal.Dot(input.lightVect);
	if (shade < 0) shade = 0;
	shading.R = shade;
	shading.G = shade;
	shading.B = shade;
	return shading;
}

sShaderOutput MainSpecular(sShaderInputData &input)
{
	sShaderOutput specular;
	CVector3 half = input.lightVect - input.viewVector;
	half.Normalize();
	double shade2 = input.normal.Dot(half);
	if (shade2 < 0.0) shade2 = 0.0;
	shade2 = pow(shade2, 30.0) * 1.0;
	if (shade2 > 15.0) shade2 = 15.0;
	specular.R = shade2;
	specular.G = shade2;
	specular.B = shade2;
	return specular;
}

sShaderOutput EnvMapping(sShaderInputData &input)
{
	sShaderOutput envReflect;
	CVector3 reflect;
	double dot = -input.viewVector.Dot(input.normal);
	reflect = input.normal * 2.0 * dot + input.viewVector;

	double alfaTexture = reflect.GetAlpha() + M_PI;
	double betaTexture = reflect.GetBeta();
	double texWidth = input.envMappingTexture->Width();
	double texHeight = input.envMappingTexture->Height();

	if (betaTexture > 0.5 * M_PI) betaTexture = 0.5 * M_PI - betaTexture;

	if (betaTexture < -0.5 * M_PI) betaTexture = -0.5 * M_PI + betaTexture;

	double dtx = (alfaTexture / (2.0 * M_PI)) * texWidth + texWidth * 8.25;
	double dty = (betaTexture / (M_PI) + 0.5) * texHeight + texHeight * 8.0;
	dtx = fmod(dtx, texWidth);
	dty = fmod(dty, texHeight);
	if (dtx < 0) dtx = 0;
	if (dty < 0) dty = 0;
	envReflect.R = input.envMappingTexture->Pixel(dtx, dty).R / 256.0;
	envReflect.G = input.envMappingTexture->Pixel(dtx, dty).G / 256.0;
	envReflect.B = input.envMappingTexture->Pixel(dtx, dty).B / 256.0;
	return envReflect;
}

sShaderOutput SurfaceColour(sShaderInputData &input)
{
#ifdef FAST_MAND
	double zx = point.x;
	double zy = point.y;
	int N = calcParam->doubles.N * 10.0;
	double p = 2.0;
	double smooth = 0.0;
	int L = 0;
	double r = 0.0;
	int nrKol = 253 * 256;
  for(L=0; L<N; L++)
  {
  	double temp = zx * zx - zy * zy + point.x;
  	zy = 2.0 * zx * zy + point.y;
  	zx = temp;
  	r = zx * zx + zy * zy;
  	if(r > 1e20)
  	{
    	smooth = (L - log(log(sqrt(r)) / log(N)) / log(p));
      nrKol = smooth * 50.0;
      nrKol = abs(nrKol) % (248*256);
  		break;
  	}
  }

	N_counter += L + 1;
	Loop_counter++;

	if (L/10 < 64)
		histogram[L/10]++;
	else
		histogram[63]++;
  return nrKol;

#else
	sShaderOutput out;

	switch(input.objectType)
	{
		case objFractal:
		{
			input.calcParam->doubles.N *= 10;
			int nrCol = floor(Compute<colouring>(input.point, *input.calcParam));
			input.calcParam->doubles.N /= 10;
			nrCol = abs(nrCol) % (248 * 256);

			sRGB colour = { 256, 256, 256 };
			if (input.param->imageSwitches.coloringEnabled)
			{
				int color_number;
				if (nrCol >= 248 * 256)
				{
					color_number = nrCol;
				}
				else
				{
					color_number = (int) (nrCol * input.param->doubles.imageAdjustments.coloring_speed + 256 * input.param->doubles.imageAdjustments.paletteOffset) % 65536;
				}
				colour = IndexToColour(color_number, input.param->palette);
			}

			out.R = colour.R / 256.0;
			out.G = colour.G / 256.0;
			out.B = colour.B / 256.0;
			break;
		}
		case objWater:
		{
			out.R = input.param->primitiveWaterColour.R / 65536.0;
			out.G = input.param->primitiveWaterColour.G / 65536.0;
			out.B = input.param->primitiveWaterColour.B / 65536.0;
			break;
		}
		case objBox:
		{
			out.R = input.param->primitiveBoxColour.R / 65536.0;
			out.G = input.param->primitiveBoxColour.G / 65536.0;
			out.B = input.param->primitiveBoxColour.B / 65536.0;
			break;
		}
		case objBoxInv:
		{
			out.R = input.param->primitiveInvertedBoxColour.R / 65536.0;
			out.G = input.param->primitiveInvertedBoxColour.G / 65536.0;
			out.B = input.param->primitiveInvertedBoxColour.B / 65536.0;
			break;
		}
		case objSphere:
		{
			out.R = input.param->primitiveSphereColour.R / 65536.0;
			out.G = input.param->primitiveSphereColour.G / 65536.0;
			out.B = input.param->primitiveSphereColour.B / 65536.0;
			break;
		}
		case objSphereInv:
		{
			out.R = input.param->primitiveInvertedSphereColour.R / 65536.0;
			out.G = input.param->primitiveInvertedSphereColour.G / 65536.0;
			out.B = input.param->primitiveInvertedSphereColour.B / 65536.0;
			break;
		}
		case objPlane:
		{
			out.R = input.param->primitivePlaneColour.R / 65536.0;
			out.G = input.param->primitivePlaneColour.G / 65536.0;
			out.B = input.param->primitivePlaneColour.B / 65536.0;
			break;
		}
	};

	return out;
#endif
}

sShaderOutput TexturedBackground(sParamRender *param, CVector3 viewVector)
{
	sShaderOutput pixel2;
	if (param->texturedBackground)
	{
		if(param->background_as_fulldome)
		{
			double alfaTexture = viewVector.GetAlpha();
			double betaTexture = viewVector.GetBeta();
			int texWidth = param->backgroundTexture->Width()*0.5;
			int texHeight = param->backgroundTexture->Height();
			int offset = 0;

			if(betaTexture < 0)
			{
				betaTexture = -betaTexture;
				alfaTexture += M_PI;
				offset = texWidth;
			}
			double texX = 0.5 * texWidth + cos(alfaTexture) * (1.0 - betaTexture / (0.5 * M_PI)) * texWidth * 0.5 + offset;
			double texY = 0.5 * texHeight + sin(alfaTexture) * (1.0 - betaTexture / (0.5 * M_PI)) * texHeight * 0.5;
			sRGB8 pixel = param->backgroundTexture->Pixel(texX, texY);
			pixel2.R = pixel.R / 256.0;
			pixel2.G = pixel.G / 256.0;
			pixel2.B = pixel.B / 256.0;
		}
		else
		{
			double alfaTexture = fmod(viewVector.GetAlpha() + 2.5 * M_PI, 2 * M_PI);
			double betaTexture = viewVector.GetBeta();
			if (betaTexture > 0.5 * M_PI) betaTexture = 0.5 * M_PI - betaTexture;
			if (betaTexture < -0.5 * M_PI) betaTexture = -0.5 * M_PI + betaTexture;
			double texX = alfaTexture / (2.0 * M_PI) * param->backgroundTexture->Width();
			double texY = (betaTexture / (M_PI) + 0.5) * param->backgroundTexture->Height();
			sRGB8 pixel = param->backgroundTexture->Pixel(texX, texY);
			pixel2.R = pixel.R/256.0;
			pixel2.G = pixel.G/256.0;
			pixel2.B = pixel.B/256.0;
		}
	}
	else
	{
		CVector3 vector(0.5, 0.5, -0.5);
		vector.Normalize();
		double grad = (viewVector.Dot(vector)+1.0);
		sRGB16 pixel;
		if(grad < 1)
		{
			double Ngrad = 1.0 - grad;
			pixel.R = (param->background_color3.R * Ngrad + param->background_color2.R * grad);
			pixel.G = (param->background_color3.G * Ngrad + param->background_color2.G * grad);
			pixel.B = (param->background_color3.B * Ngrad + param->background_color2.B * grad);
		}
		else
		{
			grad = grad - 1;
			double Ngrad = 1.0 - grad;
			pixel.R = (param->background_color2.R * Ngrad + param->background_color1.R * grad);
			pixel.G = (param->background_color2.G * Ngrad + param->background_color1.G * grad);
			pixel.B = (param->background_color2.B * Ngrad + param->background_color1.B * grad);
		}

		pixel2.R = pixel.R/65536.0;
		pixel2.G = pixel.G/65536.0;
		pixel2.B = pixel.B/65536.0;
	}
	return pixel2;
}

sShaderOutput LightShading(sShaderInputData &input, sLight light, int number, sShaderOutput *outSpecular)
{
	sShaderOutput shading;

	CVector3 d = light.position - input.point;

	double distance = d.Length();

	//angle of incidence
	CVector3 lightVector = d;
	lightVector.Normalize();

	double intensity = input.param->doubles.auxLightIntensity * 100.0 * light.intensity / (distance * distance) / number;
	double shade = input.normal.Dot(lightVector);
	if (shade < 0) shade = 0;
	shade = shade * intensity;
	if (shade > 500.0) shade = 500.0;

	//specular
	CVector3 half = lightVector - input.viewVector;
	half.Normalize();
	double shade2 = input.normal.Dot(half);
	if (shade2 < 0.0) shade2 = 0.0;
	shade2 = pow(shade2, 30.0) * 1.0;
	shade2 *= intensity * input.param->doubles.imageAdjustments.specular;
	if (shade2 > 15.0) shade2 = 15.0;

	//calculate shadow
	if ((shade > 0.01 || shade2 > 0.01) && input.param->shadow)
	{
		double light = AuxShadow(input, distance, lightVector);
		shade *= light;
		shade2 *= light;
	}
	else
	{
		if (input.param->shadow)
		{
			shade = 0;
			shade2 = 0;
		}
	}

	shading.R = shade * light.colour.R / 65536.0;
	shading.G = shade * light.colour.G / 65536.0;
	shading.B = shade * light.colour.B / 65536.0;

	outSpecular->R = shade2 * light.colour.R / 65536.0;
	outSpecular->G = shade2 * light.colour.G / 65536.0;
	outSpecular->B = shade2 * light.colour.B / 65536.0;

	return shading;
}

sShaderOutput AuxLightsShader(sShaderInputData &input, sShaderOutput *specularOut)
{
	int numberOfLights = lightsPlaced;
	if (numberOfLights < 4) numberOfLights = 4;
	sShaderOutput shadeAuxSum = { 0, 0, 0 };
	sShaderOutput specularAuxSum = { 0, 0, 0 };
	for (int i = 0; i < numberOfLights; i++)
	{
		if (i < input.param->auxLightNumber || Lights[i].enabled)
		{
			sShaderOutput specularAuxOutTemp;
			sShaderOutput shadeAux = LightShading(input, Lights[i], numberOfLights, &specularAuxOutTemp);
			shadeAuxSum.R += shadeAux.R;
			shadeAuxSum.G += shadeAux.G;
			shadeAuxSum.B += shadeAux.B;
			specularAuxSum.R += specularAuxOutTemp.R;
			specularAuxSum.G += specularAuxOutTemp.G;
			specularAuxSum.B += specularAuxOutTemp.B;
		}
	}
	*specularOut = specularAuxSum;
	return shadeAuxSum;
}

double AuxShadow(sShaderInputData &input, double distance, CVector3 lightVector)
{
	double step = input.delta;
	double dist = step;
	double light = 1.0;

	double opacity = 0.0;
	double shadowTemp = 1.0;

	double DE_factor = input.param->doubles.DE_factor;
	if(input.param->imageSwitches.iterFogEnabled || input.param->imageSwitches.volumetricLightEnabled) DE_factor = 1.0;

	for (double i = input.delta; i < distance; i += dist * DE_factor)
	{
		CVector3 point2 = input.point + lightVector * i;

		bool max_iter;
		dist = CalculateDistance(point2, *input.calcParam, &max_iter);

		if (input.param->imageSwitches.iterFogEnabled)
		{
			opacity = IterOpacity(dist * DE_factor, input.calcParam->itersOut, input.calcParam->doubles.N, input.param->doubles.iterFogOpacityTrim,
					input.param->doubles.iterFogOpacity);
		}
		else
		{
			opacity = 0.0;
		}
		shadowTemp -= opacity * (distance - i) / distance;

		float dist_thresh;
		if(input.param->imageSwitches.iterFogEnabled || input.param->imageSwitches.volumetricLightEnabled)
		{
			if(!input.calcParam->constantDEThreshold)
				dist_thresh = (point2 - input.eyePoint).Length() * input.param->doubles.resolution * input.param->doubles.persp / input.param->doubles.quality;
			else
				dist_thresh = input.param->doubles.quality;
		}
		else
			dist_thresh = input.dist_thresh;

		if (dist < dist_thresh || max_iter || shadowTemp < 0.0)
		{
			if (input.param->penetratingLights)
			{
				shadowTemp -= (distance - i) / distance;
				if (shadowTemp < 0.0) shadowTemp = 0.0;
			}
			else
			{
				shadowTemp = 0.0;
			}
			break;
		}
	}
	light = shadowTemp;
	return light;
}

void PlaceRandomLights(sParamRender *fractParams, bool onlyPredefined)
{
	srand(fractParams->auxLightRandomSeed);

	if(!onlyPredefined)
	{
		delete[] Lights;
		Lights = new sLight[fractParams->auxLightNumber + 4];
	}
	sFractal calcParam = fractParams->fractal;
	bool max_iter;

	int trial_number = 0;
	double radius_multiplier = 1.0;


	if(!onlyPredefined)
	{
	for (int i = 0; i < fractParams->auxLightNumber; i++)
		{
			trial_number++;

			CVector3 random;
			random.x = (Random(2000000) / 1000000.0 - 1.0) + (Random(1000000) / 1.0e12);
			random.y = (Random(2000000) / 1000000.0 - 1.0) + (Random(1000000) / 1.0e12);
			random.z = (Random(2000000) / 1000000.0 - 1.0) + (Random(1000000) / 1.0e12);

			CVector3 position = fractParams->doubles.auxLightRandomCenter + random * fractParams->doubles.auxLightDistributionRadius * radius_multiplier;

			double distance = CalculateDistance(position, calcParam, &max_iter);

			if (trial_number > 1000)
			{
				radius_multiplier *= 1.1;
				trial_number = 0;
			}

			if (distance > 0 && !max_iter && distance < fractParams->doubles.auxLightMaxDist * radius_multiplier)
			{
				radius_multiplier = 1.0;

				Lights[i].position = position;

				sRGB colour = { 20000 + Random(80000), 20000 + Random(80000), 20000 + Random(80000) };
				double maxColour = dMax(colour.R, colour.G, colour.B);
				colour.R = colour.R * 65536.0 / maxColour;
				colour.G = colour.G * 65536.0 / maxColour;
				colour.B = colour.B * 65536.0 / maxColour;
				Lights[i].colour = colour;

				Lights[i].intensity = distance * distance / fractParams->doubles.auxLightMaxDist;
				Lights[i].enabled = true;
				printf("Light no. %d: x=%f, y=%f, z=%f, distance=%f\n", i, position.x, position.y, position.z, distance);
			}
			else
			{
				i--;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (fractParams->auxLightPreEnabled[i])
		{
			Lights[i].position = fractParams->doubles.auxLightPre[i];
			Lights[i].intensity = fractParams->doubles.auxLightPreIntensity[i];
			Lights[i].colour = fractParams->auxLightPreColour[i];
			Lights[i].enabled = true;
		}
		else
		{
			Lights[i].enabled = false;
		}
	}

	lightsPlaced = fractParams->auxLightNumber;
	if (lightsPlaced < 4) lightsPlaced = 4;
}

double IterOpacity(double step, double iters, double maxN, double trim, double opacitySp)
{
	double opacity = ((double)iters - trim) / maxN;
	if(opacity < 0.0) opacity = 0.0;
	opacity*=opacity;
	opacity*=step  * opacitySp;
	if(opacity > 1.0) opacity = 1.0;
	return opacity;
}


sShaderOutput FakeLights(sShaderInputData &input, sShaderOutput *fakeSpec)
{
	sShaderOutput fakeLights;
	double delta = input.dist_thresh * input.param->doubles.smoothness;
	double rr = Compute<orbitTrap>(input.point, *input.calcParam);
	double fakeLight = input.param->doubles.fakeLightsIntensity/rr;
	double r = 1.0/(rr + 1e-30);

	CVector3 deltax(delta, 0.0, 0.0);
	double rx = 1.0/(Compute<orbitTrap>(input.point + deltax, *input.calcParam) + 1e-30);
	CVector3 deltay(0.0, delta, 0.0);
	double ry = 1.0/(Compute<orbitTrap>(input.point + deltay, *input.calcParam) + 1e-30);
	CVector3 deltaz(0.0, 0.0, delta);
	double rz = 1.0/(Compute<orbitTrap>(input.point + deltaz, *input.calcParam) + 1e-30);

	CVector3 fakeLightNormal;
	fakeLightNormal.x = r - rx;
	fakeLightNormal.y = r - ry;
	fakeLightNormal.z = r - rz;

	if(fakeLightNormal.x == 0 && fakeLightNormal.y == 0 && fakeLightNormal.z == 0)
	{
		fakeLightNormal.x = 0.0;
	}
	else
	{
		fakeLightNormal.Normalize();
	}
	double fakeLight2 = fakeLight * input.normal.Dot(fakeLightNormal);
	if(fakeLight2 < 0) fakeLight2 = 0;

	fakeLights.R = fakeLight2;
	fakeLights.G = fakeLight2;
	fakeLights.B = fakeLight2;

	CVector3 half = fakeLightNormal - input.viewVector;
	half.Normalize();
	double fakeSpecular = input.normal.Dot(half);
	if (fakeSpecular < 0.0) fakeSpecular = 0.0;
	fakeSpecular = pow(fakeSpecular, 30.0) * fakeLight;
	if (fakeSpecular > 15.0) fakeSpecular = 15.0;
	fakeSpec->R = fakeSpecular;
	fakeSpec->G = fakeSpecular;
	fakeSpec->B = fakeSpecular;

	return fakeLights;
}

sRGB IndexToColour(int index, sRGB *palette)
{
	double R1, R2, G1, G2, B1, B2;
	double RK, GK, BK;
	sRGB colour = { 255, 255, 255 };
	int kol, delta;
	if (index < 0)
	{
		colour = palette[255];
	}
	else
	{
		index = index % (255 * 256);
		kol = index / 256;
		if (kol < 255)
		{
			R1 = palette[kol].R;
			G1 = palette[kol].G;
			B1 = palette[kol].B;
			R2 = palette[kol + 1].R;
			G2 = palette[kol + 1].G;
			B2 = palette[kol + 1].B;
			RK = (R2 - R1) / 256.0;
			GK = (G2 - G1) / 256.0;
			BK = (B2 - B1) / 256.0;
			delta = index % 256;
			colour.R = R1 + (RK * delta);
			colour.G = G1 + (GK * delta);
			colour.B = B1 + (BK * delta);
		}
	}
	return colour;
}

