/*********************************************************
 /                   MANDELBULBER
 / functions loading and saving parameters
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 / many improvements done by Rayan Hitchman
 ********************************************************/

#include <cstdlib>
#include <locale.h>
#include <string.h>

#include "files.h"
#include "interface.h"
#include "shaders.h"
#include "settings.h"
#include "smartptr.h"

using namespace std;

char data_directory[1000];

const char* axis_names[] = {"X", "Y", "Z"};
const char* component_names[] = {"alfa", "beta", "gamma"};

bool paletteLoadedFromSettingsFile = false;

void fprintfDot(FILE *file, const char *string, double value, double defaultVal, bool compare)
{
	bool theSame = true;
	if(value >= 0 && (value < defaultVal*0.9999999999 || value > defaultVal*1.0000000001)) theSame = false;
	if(value < 0 && (value > defaultVal*0.9999999999 || value < defaultVal*1.0000000001)) theSame = false;

	if (compare && !theSame)
	{
		char str[G_ASCII_DTOSTR_BUF_SIZE];
		g_ascii_dtostr(str, sizeof(str), value);
		fprintf(file, "%s %s;\n", string, str);
	}

	if(!compare)
	{
		char str[G_ASCII_DTOSTR_BUF_SIZE];
		g_ascii_dtostr(str, sizeof(str), defaultVal);
		fprintf(file, "%s %s;\n", string, str);
	}
}

void fprintfInt(FILE *file, const char *string, int value, int defaultVal, bool compare)
{
	if (compare && value != defaultVal)
	{
		fprintf(file, "%s %d;\n", string, value);
	}

	if(!compare)
	{
		fprintf(file, "%s %d;\n", string, defaultVal);
	}
}

double atof2(const char *str)
{
	char *end;
	double v = g_ascii_strtod(str, &end);
	if(end && *end == localeconv()->decimal_point[0])
	{
		// end did not point to the string's terminating NUL,
		// but to the radix character of the user's locale.
		// This must be a "-o" commandline- setting, so convert
		// the value using locale-aware strtod
		v = strtod(str, NULL);
	}
	return v;
}

void MakePaletteString(const sRGB *palette, char *paletteString)
{
	int length;
	int pointer = 0;
	for (int i = 0; i < 256; i++)
	{
		int colour = palette[i].R * 65536 + palette[i].G * 256 + palette[i].B;
		colour = colour & 0x00FFFFFF;
		length = sprintf(&paletteString[pointer], "%x ", colour);
		pointer += length;
	}
}

void GetPaletteFromString(sRGB *palette, const char *paletteString)
{
	int index = 0;
	for (int i = 0; i < 2000; i++)
	{
		int colour = 0;
		sscanf(&paletteString[i], "%x", (unsigned int*)&colour);
		sRGB rgbColour;
		rgbColour.R = colour / 65536;
		rgbColour.G = (colour / 256) % 256;
		rgbColour.B = colour % 256;
		palette[index] = rgbColour;
		//printf("R = %d, G = %d, B = %d\n", rgbColour.R, rgbColour.G, rgbColour.B);

		while (i < 2000 && paletteString[i] != ' ')
		{
			i++;
		}
		index++;
		if (index == 256)
		{
			paletteLoadedFromSettingsFile = true;
			break;
		}
	}
}

void SaveSettings(const char *filename, const sParamRender& params, bool compare)
{
	char *paletteString = new char[257 * 7];
	memset(paletteString, 0, 257 * 7);
	MakePaletteString(params.palette, paletteString);

	FILE * fileSettings;
	fileSettings = fopen(filename, "w");

	char parameterName[100];

	fprintfDot(fileSettings, "Mandelbulber", MANDELBULBER_VERSION, MANDELBULBER_VERSION, false);
	fprintfInt(fileSettings, "image_width", params.image_width, 800, compare);
	fprintfInt(fileSettings, "image_height", params.image_height, 600, compare);
	fprintfInt(fileSettings, "tiles", params.noOfTiles, 1, compare);
	fprintfDot(fileSettings, "x_min", params.fractal.doubles.amin, -10.0, compare);
	fprintfDot(fileSettings, "x_max", params.fractal.doubles.amax, 10.0, compare);
	fprintfDot(fileSettings, "y_min", params.fractal.doubles.bmin, -10.0, compare);
	fprintfDot(fileSettings, "y_max", params.fractal.doubles.bmax, 10.0, compare);
	fprintfDot(fileSettings, "z_min", params.fractal.doubles.cmin, -10.0, compare);
	fprintfDot(fileSettings, "z_max", params.fractal.doubles.cmax, 10.0, compare);
	fprintfDot(fileSettings, "view_point_x", params.doubles.vp.x, 0.0, compare);
	fprintfDot(fileSettings, "view_point_y", params.doubles.vp.y, 0.0, compare);
	fprintfDot(fileSettings, "view_point_z", params.doubles.vp.z, 0.0, compare);
	fprintfDot(fileSettings, "angle_alfa", params.doubles.alpha * 180.0 / M_PI, -20, compare);
	fprintfDot(fileSettings, "angle_beta", params.doubles.beta * 180.0 / M_PI, 30, compare);
	fprintfDot(fileSettings, "angle_gamma", params.doubles.gamma * 180.0 / M_PI, 0.0, compare);
	fprintfDot(fileSettings, "zoom", params.doubles.zoom, 2.5, compare);
	fprintfDot(fileSettings, "perspective", params.doubles.persp, 0.5, compare);
	fprintfInt(fileSettings, "formula", params.fractal.formula, trig_optim, compare);
	fprintfDot(fileSettings, "power", params.fractal.doubles.power, 9.0, compare);
	fprintfInt(fileSettings, "N", params.fractal.doubles.N, 250, compare);
	fprintfInt(fileSettings, "minN", params.fractal.minN, 1, compare);
	fprintfDot(fileSettings, "fractal_constant_factor", params.fractal.doubles.constantFactor, 1.0, compare);
	fprintfDot(fileSettings, "quality", params.doubles.quality, 1.0, compare);
	fprintfDot(fileSettings, "smoothness", params.doubles.smoothness, 1.0, compare);
	fprintfInt(fileSettings, "julia_mode", params.fractal.juliaMode, false, compare);
	fprintfDot(fileSettings, "julia_a", params.fractal.doubles.julia.x, 0.0, compare);
	fprintfDot(fileSettings, "julia_b", params.fractal.doubles.julia.y, 0.0, compare);
	fprintfDot(fileSettings, "julia_c", params.fractal.doubles.julia.z, 0.0, compare);
	fprintfInt(fileSettings, "tglad_folding_mode", params.fractal.tgladFoldingMode, false, compare);
	fprintfDot(fileSettings, "folding_limit", params.fractal.doubles.foldingLimit, 1.0, compare);
	fprintfDot(fileSettings, "folding_value", params.fractal.doubles.foldingValue, 2.0, compare);
	fprintfInt(fileSettings, "spherical_folding_mode", params.fractal.sphericalFoldingMode, false, compare);
	fprintfDot(fileSettings, "spherical_folding_fixed", params.fractal.doubles.foldingSphericalFixed, 1.0, compare);
	fprintfDot(fileSettings, "spherical_folding_min", params.fractal.doubles.foldingSphericalMin, 0.5, compare);
	fprintfInt(fileSettings, "IFS_folding_mode", params.fractal.IFS.foldingMode, false, compare);
	fprintfInt(fileSettings, "iteration_threshold_mode", params.fractal.iterThresh, false, compare);
	fprintfInt(fileSettings, "analityc_DE_mode", params.fractal.analitycDE, true, compare);
	fprintfDot(fileSettings, "DE_factor", params.doubles.DE_factor, 1.0, compare);
	fprintfDot(fileSettings, "brightness", params.doubles.imageAdjustments.brightness, 1.0, compare);
	fprintfDot(fileSettings, "contrast", params.doubles.imageAdjustments.contrast, 1.0, compare);
	fprintfDot(fileSettings, "gamma", params.doubles.imageAdjustments.imageGamma, 1.0, compare);
	fprintfInt(fileSettings, "hdr", params.imageSwitches.hdrEnabled, false, compare);
	fprintfDot(fileSettings, "ambient", params.doubles.imageAdjustments.ambient, 0.0, compare);
	fprintfDot(fileSettings, "reflect", params.doubles.imageAdjustments.reflect, 0.0, compare);
	fprintfDot(fileSettings, "shadows_intensity", params.doubles.imageAdjustments.directLight, 0.7, compare);
	fprintfDot(fileSettings, "shadows_cone_angle", params.doubles.shadowConeAngle, 1.0, compare);
	fprintfDot(fileSettings, "ambient_occlusion", params.doubles.imageAdjustments.globalIlum, 1.0, compare);
	fprintfInt(fileSettings, "ambient_occlusion_quality", params.globalIlumQuality, 4, compare);
	fprintfDot(fileSettings, "ambient_occlusion_fast_tune", params.doubles.fastAoTune, 1.0, compare);
	fprintfDot(fileSettings, "shading", params.doubles.imageAdjustments.shading, 1.0, compare);
	fprintfDot(fileSettings, "specular", params.doubles.imageAdjustments.specular, 1.0, compare);
	fprintfDot(fileSettings, "glow_intensity", params.doubles.imageAdjustments.glow_intensity, 1.0, compare);
	fprintfInt(fileSettings, "glow_color_1_R", params.effectColours.glow_color1.R, 40984, compare);
	fprintfInt(fileSettings, "glow_color_1_G", params.effectColours.glow_color1.G, 44713, compare);
	fprintfInt(fileSettings, "glow_color_1_B", params.effectColours.glow_color1.B, 49490, compare);
	fprintfInt(fileSettings, "glow_color_2_R", params.effectColours.glow_color2.R, 57192, compare);
	fprintfInt(fileSettings, "glow_color_2_G", params.effectColours.glow_color2.G, 60888, compare);
	fprintfInt(fileSettings, "glow_color_2_B", params.effectColours.glow_color2.B, 62408, compare);
	fprintfInt(fileSettings, "background_color_1_R", params.background_color1.R, 0, compare);
	fprintfInt(fileSettings, "background_color_1_G", params.background_color1.G, 38306, compare);
	fprintfInt(fileSettings, "background_color_1_B", params.background_color1.B, 65535, compare);
	fprintfInt(fileSettings, "background_color_2_R", params.background_color2.R, 65535, compare);
	fprintfInt(fileSettings, "background_color_2_G", params.background_color2.G, 65535, compare);
	fprintfInt(fileSettings, "background_color_2_B", params.background_color2.B, 65535, compare);
	fprintfInt(fileSettings, "background_color_3_R", params.background_color3.R, 0, compare);
	fprintfInt(fileSettings, "background_color_3_G", params.background_color3.G, 10000, compare);
	fprintfInt(fileSettings, "background_color_3_B", params.background_color3.B, 500, compare);
	fprintfInt(fileSettings, "fog_colour_1_R", params.fogColour1.R, 0, compare);
	fprintfInt(fileSettings, "fog_colour_1_G", params.fogColour1.G, 0, compare);
	fprintfInt(fileSettings, "fog_colour_1_B", params.fogColour1.B, 0, compare);
	fprintfInt(fileSettings, "fog_colour_2_R", params.fogColour2.R, 0, compare);
	fprintfInt(fileSettings, "fog_colour_2_G", params.fogColour2.G, 30000, compare);
	fprintfInt(fileSettings, "fog_colour_2_B", params.fogColour2.B, 65535, compare);
	fprintfInt(fileSettings, "fog_colour_3_R", params.fogColour3.R, 65535, compare);
	fprintfInt(fileSettings, "fog_colour_3_G", params.fogColour3.G, 65535, compare);
	fprintfInt(fileSettings, "fog_colour_3_B", params.fogColour3.B, 65535, compare);
	fprintfInt(fileSettings, "background_color_1_G", params.background_color1.G, 38306, compare);
	fprintfInt(fileSettings, "background_color_1_B", params.background_color1.B, 65535, compare);
	fprintfInt(fileSettings, "textured_background", params.texturedBackground, false, compare);
	fprintfInt(fileSettings, "background_as_fuldome", params.background_as_fulldome, false, compare);
	fprintfInt(fileSettings, "shadows_enabled", params.shadow, true, compare);
	fprintfInt(fileSettings, "ambient_occlusion_enabled", params.global_ilumination, false, compare);
	fprintfInt(fileSettings, "fast_ambient_occlusion_mode", params.fastGlobalIllumination, false, compare);
	fprintfInt(fileSettings, "fractal_color", params.imageSwitches.coloringEnabled, true, compare);
	fprintfInt(fileSettings, "coloring_random_seed", params.coloring_seed, 123456, compare);
	fprintfDot(fileSettings, "coloring_saturation", params.doubles.colourSaturation, 1.0, compare);
	fprintfDot(fileSettings, "coloring_speed", params.doubles.imageAdjustments.coloring_speed, 1.0, compare);
	fprintfDot(fileSettings, "coloring_palette_offset", params.doubles.imageAdjustments.paletteOffset, 0.0, compare);
	fprintfInt(fileSettings, "slow_shading", params.slowShading, false, compare);
	fprintfInt(fileSettings, "limits_enabled", params.fractal.limits_enabled, false, compare);
	fprintfInt(fileSettings, "post_fog_enabled", params.imageSwitches.fogEnabled, false, compare);
	fprintfDot(fileSettings, "post_fog_visibility", params.doubles.imageAdjustments.fogVisibility, 20.0, compare);
	fprintfInt(fileSettings, "post_fog_color_R", params.effectColours.fogColor.R, 59399, compare);
	fprintfInt(fileSettings, "post_fog_color_G", params.effectColours.fogColor.G, 61202, compare);
	fprintfInt(fileSettings, "post_fog_color_B", params.effectColours.fogColor.B, 65535, compare);
	fprintfInt(fileSettings, "post_SSAO_enabled", params.SSAOEnabled, true, compare);
	fprintfInt(fileSettings, "post_SSAO_quality", params.SSAOQuality, 20.0, compare);
	fprintfInt(fileSettings, "post_DOF_enabled", params.DOFEnabled, false, compare);
	fprintfDot(fileSettings, "post_DOF_focus", params.doubles.DOFFocus, 21.7, compare);
	fprintfDot(fileSettings, "post_DOF_radius", params.doubles.DOFRadius, 10.0, compare);
	fprintfDot(fileSettings, "main_light_intensity", params.doubles.imageAdjustments.mainLightIntensity, 1.0, compare);
	fprintfDot(fileSettings, "main_light_alfa", params.doubles.mainLightAlpha, -45 * M_PI / 180.0, compare);
	fprintfDot(fileSettings, "main_light_beta", params.doubles.mainLightBeta, 45 * M_PI / 180.0, compare);
	fprintfInt(fileSettings, "main_light_colour_R", params.effectColours.mainLightColour.R, 0xFFFF, compare);
	fprintfInt(fileSettings, "main_light_colour_G", params.effectColours.mainLightColour.G, 0xFFFF, compare);
	fprintfInt(fileSettings, "main_light_colour_B", params.effectColours.mainLightColour.B, 0xFFFF, compare);
	if(!compare || params.auxLightPreEnabled[0] || params.auxLightPreEnabled[1] || params.auxLightPreEnabled[2] || params.auxLightPreEnabled[3] || params.auxLightNumber >0 || params.fractal.formula == ocl_custom)
	{
		fprintfDot(fileSettings, "aux_light_intensity", params.doubles.auxLightIntensity, 1.0, compare);
		fprintfInt(fileSettings, "aux_light_random_seed", params.auxLightRandomSeed, 1234, compare);
		fprintfInt(fileSettings, "aux_light_number", params.auxLightNumber, 0, compare);
		fprintfDot(fileSettings, "aux_light_max_dist", params.doubles.auxLightMaxDist, 0.1, compare);
		fprintfDot(fileSettings, "aux_light_distribution_radius", params.doubles.auxLightDistributionRadius, 3.0, compare);
		fprintfDot(fileSettings, "aux_light_predefined_1_x", params.doubles.auxLightPre[0].x, 3, compare);
		fprintfDot(fileSettings, "aux_light_predefined_1_y", params.doubles.auxLightPre[0].y, -3, compare);
		fprintfDot(fileSettings, "aux_light_predefined_1_z", params.doubles.auxLightPre[0].z, -3, compare);
		fprintfDot(fileSettings, "aux_light_predefined_1_intensity", params.doubles.auxLightPreIntensity[0], 1.3, compare);
		fprintfDot(fileSettings, "aux_light_predefined_2_x", params.doubles.auxLightPre[1].x, -3, compare);
		fprintfDot(fileSettings, "aux_light_predefined_2_y", params.doubles.auxLightPre[1].y, -3, compare);
		fprintfDot(fileSettings, "aux_light_predefined_2_z", params.doubles.auxLightPre[1].z, 0, compare);
		fprintfDot(fileSettings, "aux_light_predefined_2_intensity", params.doubles.auxLightPreIntensity[1], 1, compare);
		fprintfDot(fileSettings, "aux_light_predefined_3_x", params.doubles.auxLightPre[2].x, -3, compare);
		fprintfDot(fileSettings, "aux_light_predefined_3_y", params.doubles.auxLightPre[2].y, 3, compare);
		fprintfDot(fileSettings, "aux_light_predefined_3_z", params.doubles.auxLightPre[2].z, -1, compare);
		fprintfDot(fileSettings, "aux_light_predefined_3_intensity", params.doubles.auxLightPreIntensity[2], 3, compare);
		fprintfDot(fileSettings, "aux_light_predefined_4_x", params.doubles.auxLightPre[3].x, 0, compare);
		fprintfDot(fileSettings, "aux_light_predefined_4_y", params.doubles.auxLightPre[3].y, -1, compare);
		fprintfDot(fileSettings, "aux_light_predefined_4_z", params.doubles.auxLightPre[3].z, 3, compare);
		fprintfDot(fileSettings, "aux_light_predefined_4_intensity", params.doubles.auxLightPreIntensity[1], 2, compare);
		fprintfInt(fileSettings, "aux_light_predefined_1_enabled", params.auxLightPreEnabled[0], false, compare);
		fprintfInt(fileSettings, "aux_light_predefined_2_enabled", params.auxLightPreEnabled[1], false, compare);
		fprintfInt(fileSettings, "aux_light_predefined_3_enabled", params.auxLightPreEnabled[2], false, compare);
		fprintfInt(fileSettings, "aux_light_predefined_4_enabled", params.auxLightPreEnabled[3], false, compare);
		fprintfInt(fileSettings, "aux_light_predefined_1_colour_R", params.auxLightPreColour[0].R, 45761, compare);
		fprintfInt(fileSettings, "aux_light_predefined_1_colour_G", params.auxLightPreColour[0].G, 53633, compare);
		fprintfInt(fileSettings, "aux_light_predefined_1_colour_B", params.auxLightPreColour[0].B, 59498, compare);
		fprintfInt(fileSettings, "aux_light_predefined_2_colour_R", params.auxLightPreColour[1].R, 62875, compare);
		fprintfInt(fileSettings, "aux_light_predefined_2_colour_G", params.auxLightPreColour[1].G, 55818, compare);
		fprintfInt(fileSettings, "aux_light_predefined_2_colour_B", params.auxLightPreColour[1].B, 50083, compare);
		fprintfInt(fileSettings, "aux_light_predefined_3_colour_R", params.auxLightPreColour[2].R, 64884, compare);
		fprintfInt(fileSettings, "aux_light_predefined_3_colour_G", params.auxLightPreColour[2].G, 64928, compare);
		fprintfInt(fileSettings, "aux_light_predefined_3_colour_B", params.auxLightPreColour[2].B, 48848, compare);
		fprintfInt(fileSettings, "aux_light_predefined_4_colour_R", params.auxLightPreColour[3].R, 52704, compare);
		fprintfInt(fileSettings, "aux_light_predefined_4_colour_G", params.auxLightPreColour[3].G, 62492, compare);
		fprintfInt(fileSettings, "aux_light_predefined_4_colour_B", params.auxLightPreColour[3].B, 45654, compare);
		fprintfDot(fileSettings, "aux_light_visibility", params.doubles.auxLightVisibility, 1, compare);
		fprintfDot(fileSettings, "aux_light_random_center_X", params.doubles.auxLightRandomCenter.x, 0, compare);
		fprintfDot(fileSettings, "aux_light_random_center_Y", params.doubles.auxLightRandomCenter.y, 0, compare);
		fprintfDot(fileSettings, "aux_light_random_center_Z", params.doubles.auxLightRandomCenter.z, 0, compare);
	}

	if(!compare || params.fractal.formula == kaleidoscopic || params.fractal.formula == hybrid || params.fractal.IFS.foldingMode || params.fractal.formula == ocl_custom)
	{
		fprintfDot(fileSettings, "IFS_scale", params.fractal.IFS.doubles.scale, 2, compare);
		fprintfDot(fileSettings, "IFS_rot_alfa", params.fractal.IFS.doubles.rotationAlfa, 0, compare);
		fprintfDot(fileSettings, "IFS_rot_beta", params.fractal.IFS.doubles.rotationBeta, 0, compare);
		fprintfDot(fileSettings, "IFS_rot_gamma", params.fractal.IFS.doubles.rotationGamma, 0, compare);
		fprintfDot(fileSettings, "IFS_offsetX", params.fractal.IFS.doubles.offset.x, 1, compare);
		fprintfDot(fileSettings, "IFS_offsetY", params.fractal.IFS.doubles.offset.y, 0, compare);
		fprintfDot(fileSettings, "IFS_offsetZ", params.fractal.IFS.doubles.offset.z, 0, compare);
		fprintfDot(fileSettings, "IFS_edgeX", params.fractal.IFS.doubles.edge.x, 0, compare);
		fprintfDot(fileSettings, "IFS_edgeY", params.fractal.IFS.doubles.edge.y, 0, compare);
		fprintfDot(fileSettings, "IFS_edgeZ", params.fractal.IFS.doubles.edge.z, 0, compare);
		fprintfInt(fileSettings, "IFS_absX", params.fractal.IFS.absX, false, compare);
		fprintfInt(fileSettings, "IFS_absY", params.fractal.IFS.absY, false, compare);
		fprintfInt(fileSettings, "IFS_absZ", params.fractal.IFS.absZ, false, compare);
		fprintfInt(fileSettings, "IFS_menger_sponge_mode", params.fractal.IFS.mengerSpongeMode, false, compare);

		for (int i = 0; i < IFS_VECTOR_COUNT; i++)
		{
			sprintf(parameterName, "IFS_%d_x", i);
			fprintfDot(fileSettings, parameterName, params.fractal.IFS.doubles.direction[i].x, 1, compare);
			sprintf(parameterName, "IFS_%d_y", i);
			fprintfDot(fileSettings, parameterName, params.fractal.IFS.doubles.direction[i].y, 0, compare);
			sprintf(parameterName, "IFS_%d_z", i);
			fprintfDot(fileSettings, parameterName, params.fractal.IFS.doubles.direction[i].z, 0, compare);
			sprintf(parameterName, "IFS_%d_alfa", i);
			fprintfDot(fileSettings, parameterName, params.fractal.IFS.doubles.alfa[i], 0, compare);
			sprintf(parameterName, "IFS_%d_beta", i);
			fprintfDot(fileSettings, parameterName, params.fractal.IFS.doubles.beta[i], 0, compare);
			sprintf(parameterName, "IFS_%d_gamma", i);
			fprintfDot(fileSettings, parameterName, params.fractal.IFS.doubles.gamma[i], 0, compare);
			sprintf(parameterName, "IFS_%d_distance", i);
			fprintfDot(fileSettings, parameterName, params.fractal.IFS.doubles.distance[i], 0, compare);
			sprintf(parameterName, "IFS_%d_intensity", i);
			fprintfDot(fileSettings, parameterName, params.fractal.IFS.doubles.intensity[i], 1, compare);
			sprintf(parameterName, "IFS_%d_enabled", i);
			fprintfInt(fileSettings, parameterName, params.fractal.IFS.enabled[i], false, compare);
		}
	}
	fprintfInt(fileSettings, "start_frame", params.startFrame, 0, compare);
	fprintfInt(fileSettings, "end_frame", params.endFrame, 1000, compare);
	fprintfInt(fileSettings, "frames_per_keyframe", params.framesPerKeyframe, 100, compare);

	if (!compare || params.fractal.formula == hybrid || params.fractal.formula == ocl_custom)
	{
		for (int i = 1; i <= HYBRID_COUNT; ++i)
		{
			sprintf(parameterName, "hybrid_formula_%d", i);
			if (i == 5) fprintfInt(fileSettings, parameterName, params.fractal.hybridFormula[i - 1], 2, compare);
			else fprintfInt(fileSettings, parameterName, params.fractal.hybridFormula[i - 1], 0, compare);
		}

		for (int i = 1; i <= HYBRID_COUNT; ++i)
		{
			sprintf(parameterName, "hybrid_iterations_%d", i);
			fprintfInt(fileSettings, parameterName, params.fractal.hybridIters[i - 1], 1, compare);
		}

		for (int i = 1; i <= HYBRID_COUNT; ++i)
		{
			sprintf(parameterName, "hybrid_power_%d", i);
			fprintfDot(fileSettings, parameterName, params.fractal.doubles.hybridPower[i - 1], 2, compare);
		}

		fprintfInt(fileSettings, "hybrid_cyclic", params.fractal.hybridCyclic, false, compare);
	}

	fprintfInt(fileSettings, "fish_eye", params.perspectiveType, false, compare);
	fprintfInt(fileSettings, "fish_eye_180cut", params.fishEyeCut, false, compare);
	fprintfDot(fileSettings, "stereo_eye_distance", params.doubles.stereoEyeDistance, 0.1, compare);
	fprintfInt(fileSettings, "stereo_enabled", params.stereoEnabled, false, compare);

	if (!compare || params.fractal.formula == tglad || params.fractal.formula == smoothMandelbox || params.fractal.formula == mandelboxVaryScale4D || params.fractal.formula == generalizedFoldBox || params.fractal.formula == hybrid || params.fractal.formula == ocl_custom)
	{
		fprintfDot(fileSettings, "mandelbox_scale", params.fractal.mandelbox.doubles.scale, 2.0, compare);
		fprintfDot(fileSettings, "mandelbox_folding_limit", params.fractal.mandelbox.doubles.foldingLimit, 1.0, compare);
		fprintfDot(fileSettings, "mandelbox_folding_value", params.fractal.mandelbox.doubles.foldingValue, 2.0, compare);
		fprintfDot(fileSettings, "mandelbox_folding_min_radius", params.fractal.mandelbox.doubles.foldingSphericalMin, 0.5, compare);
		fprintfDot(fileSettings, "mandelbox_folding_fixed_radius", params.fractal.mandelbox.doubles.foldingSphericalFixed, 1.0, compare);
		fprintfDot(fileSettings, "mandelbox_sharpness", params.fractal.mandelbox.doubles.sharpness, 3.0, compare);
		fprintfDot(fileSettings, "mandelbox_offset_X", params.fractal.mandelbox.doubles.offset.x, 0.0, compare);
		fprintfDot(fileSettings, "mandelbox_offset_Y", params.fractal.mandelbox.doubles.offset.y, 0.0, compare);
		fprintfDot(fileSettings, "mandelbox_offset_Z", params.fractal.mandelbox.doubles.offset.z, 0.0, compare);

		for (int component = 0; component < 3; ++component)
		{
			sprintf(parameterName, "mandelbox_rotation_main_%s", component_names[component]);
			fprintfDot(fileSettings, parameterName, params.fractal.mandelbox.doubles.rotationMain[component] * 180.0 / M_PI, 0, compare);
		}

		for (int fold = 0; fold < MANDELBOX_FOLDS; ++fold)
			for (int axis = 0; axis < 3; ++axis)
				for (int component = 0; component < 3; ++component)
				{
					sprintf(parameterName, "mandelbox_rotation_%s%d_%s", axis_names[axis], fold + 1, component_names[component]);
					fprintfDot(fileSettings, parameterName, params.fractal.mandelbox.doubles.rotation[fold][axis][component] * 180.0 / M_PI, 0, compare);
				}

		fprintfDot(fileSettings, "mandelbox_color_R", params.fractal.mandelbox.doubles.colorFactorR, 0, compare);
		fprintfDot(fileSettings, "mandelbox_color_X", params.fractal.mandelbox.doubles.colorFactorX, 0.03, compare);
		fprintfDot(fileSettings, "mandelbox_color_Y", params.fractal.mandelbox.doubles.colorFactorY, 0.05, compare);
		fprintfDot(fileSettings, "mandelbox_color_Z", params.fractal.mandelbox.doubles.colorFactorZ, 0.07, compare);
		fprintfDot(fileSettings, "mandelbox_color_Sp1", params.fractal.mandelbox.doubles.colorFactorSp1, 0.2, compare);
		fprintfDot(fileSettings, "mandelbox_color_Sp2", params.fractal.mandelbox.doubles.colorFactorSp2, 0.2, compare);
		fprintfInt(fileSettings, "mandelbox_rotation_enabled", params.fractal.mandelbox.rotationsEnabled, false, compare);
		fprintfInt(fileSettings, "mandelbox_fold_mode", params.fractal.genFoldBox.type, true, compare);
		fprintfDot(fileSettings, "mandelbox_solid", params.fractal.mandelbox.doubles.solid, 1.0, compare);
		fprintfDot(fileSettings, "mandelbox_melt", params.fractal.mandelbox.doubles.melt, 0.0, compare);
	}

	fprintfDot(fileSettings, "view_distance_max", params.doubles.viewDistanceMax, 50, compare);
	fprintfDot(fileSettings, "view_distance_min", params.doubles.viewDistanceMin, 1e-15, compare);
	fprintfInt(fileSettings, "interior_mode", params.fractal.interiorMode, false, compare);
	fprintfDot(fileSettings, "FoldingIntPow_folding_factor", params.fractal.doubles.FoldingIntPowFoldFactor, 2.0, compare);
	fprintfDot(fileSettings, "FoldingIntPow_z_factor", params.fractal.doubles.FoldingIntPowZfactor, 5, compare);
	fprintfInt(fileSettings, "linear_DE_mode", params.fractal.linearDEmode, false, compare);
	fprintfInt(fileSettings, "constant_DE_threshold", params.fractal.constantDEThreshold, false, compare);

	for(int i=0; i<5; i++)
	{
		sprintf(parameterName, "volumetric_light_intensity_%d", i);
		fprintfDot(fileSettings, parameterName, params.doubles.volumetricLightIntensity[i], 100, compare);
		sprintf(parameterName, "volumetric_light_enabled_%d", i);
		fprintfInt(fileSettings, parameterName, params.volumetricLightEnabled[i], false, compare);
	}
	fprintfInt(fileSettings, "penetrating_lights", params.penetratingLights, true, compare);
	fprintfInt(fileSettings, "raytraced_reflections", params.imageSwitches.raytracedReflections, false, compare);
	fprintfInt(fileSettings, "reflections_max", params.reflectionsMax, 5, compare);

	fprintfDot(fileSettings, "mandelbox_vary_scale_vary", params.fractal.mandelbox.doubles.vary4D.scaleVary, 0.1, compare);
	fprintfDot(fileSettings, "mandelbox_vary_fold", params.fractal.mandelbox.doubles.vary4D.fold, 1, compare);
	fprintfDot(fileSettings, "mandelbox_vary_minr", params.fractal.mandelbox.doubles.vary4D.minR, 0.5, compare);
	fprintfDot(fileSettings, "mandelbox_vary_rpower", params.fractal.mandelbox.doubles.vary4D.rPower, 1, compare);
	fprintfDot(fileSettings, "mandelbox_vary_wadd", params.fractal.mandelbox.doubles.vary4D.wadd, 0, compare);

	fprintfDot(fileSettings, "c_add", params.fractal.doubles.cadd, -1.3, compare);

	fprintfDot(fileSettings, "volumetric_fog_density", params.doubles.fogDensity, 0.0, compare);
	fprintfDot(fileSettings, "volumetric_fog_colour_1_distance", params.doubles.fogColour1Distance, 1.0, compare);
	fprintfDot(fileSettings, "volumetric_fog_colour_2_distance", params.doubles.fogColour2Distance, 2.0, compare);
	fprintfDot(fileSettings, "volumetric_fog_distance_factor", params.doubles.fogDistanceFactor, 1.0, compare);

	fprintfInt(fileSettings, "iteration_fog_enable", params.imageSwitches.iterFogEnabled, false, compare);
	fprintfDot(fileSettings, "iteration_fog_opacity", params.doubles.iterFogOpacity, 1000.0, compare);
	fprintfDot(fileSettings, "iteration_fog_opacity_trim", params.doubles.iterFogOpacityTrim, 4.0, compare);

	fprintfInt(fileSettings, "primitive_only_plane", params.fractal.primitives.onlyPlane, false, compare);

	if (!compare || params.fractal.primitives.planeEnable || params.fractal.formula == ocl_custom)
	{
		fprintfInt(fileSettings, "primitive_plane_enabled", params.fractal.primitives.planeEnable, false, compare);
		fprintfDot(fileSettings, "primitive_plane_centre_X", params.fractal.doubles.primitives.planeCentre.x, 0.0, compare);
		fprintfDot(fileSettings, "primitive_plane_centre_Y", params.fractal.doubles.primitives.planeCentre.y, 0.0, compare);
		fprintfDot(fileSettings, "primitive_plane_centre_Z", params.fractal.doubles.primitives.planeCentre.z, 0.0, compare);
		fprintfDot(fileSettings, "primitive_plane_normal_X", params.fractal.doubles.primitives.planeNormal.x, 0.0, compare);
		fprintfDot(fileSettings, "primitive_plane_normal_Y", params.fractal.doubles.primitives.planeNormal.y, 0.0, compare);
		fprintfDot(fileSettings, "primitive_plane_normal_Z", params.fractal.doubles.primitives.planeNormal.z, -1.0, compare);
		fprintfInt(fileSettings, "primitive_plane_colour_R", params.primitivePlaneColour.R, 20000, compare);
		fprintfInt(fileSettings, "primitive_plane_colour_G", params.primitivePlaneColour.G, 20000, compare);
		fprintfInt(fileSettings, "primitive_plane_colour_B", params.primitivePlaneColour.B, 20000, compare);
		fprintfDot(fileSettings, "primitive_plane_reflect", params.doubles.primitivePlaneReflect, 0.0, compare);
	}

	if (!compare || params.fractal.primitives.boxEnable || params.fractal.formula == ocl_custom)
	{
		fprintfInt(fileSettings, "primitive_box_enabled", params.fractal.primitives.boxEnable, false, compare);
		fprintfDot(fileSettings, "primitive_box_centre_X", params.fractal.doubles.primitives.boxCentre.x, 0.0, compare);
		fprintfDot(fileSettings, "primitive_box_centre_Y", params.fractal.doubles.primitives.boxCentre.y, 0.0, compare);
		fprintfDot(fileSettings, "primitive_box_centre_Z", params.fractal.doubles.primitives.boxCentre.z, 0.0, compare);
		fprintfDot(fileSettings, "primitive_box_size_X", params.fractal.doubles.primitives.boxSize.x, 2.0, compare);
		fprintfDot(fileSettings, "primitive_box_size_Y", params.fractal.doubles.primitives.boxSize.y, 2.0, compare);
		fprintfDot(fileSettings, "primitive_box_size_Z", params.fractal.doubles.primitives.boxSize.z, 2.0, compare);
		fprintfInt(fileSettings, "primitive_box_colour_R", params.primitiveBoxColour.R, 20000, compare);
		fprintfInt(fileSettings, "primitive_box_colour_G", params.primitiveBoxColour.G, 20000, compare);
		fprintfInt(fileSettings, "primitive_box_colour_B", params.primitiveBoxColour.B, 20000, compare);
		fprintfDot(fileSettings, "primitive_box_reflect", params.doubles.primitiveBoxReflect, 0.0, compare);
	}

	if (!compare || params.fractal.primitives.invertedBoxEnable || params.fractal.formula == ocl_custom)
	{
		fprintfInt(fileSettings, "primitive_invertedBox_enabled", params.fractal.primitives.invertedBoxEnable, false, compare);
		fprintfDot(fileSettings, "primitive_invertedBox_centre_X", params.fractal.doubles.primitives.invertedBoxCentre.x, 0.0, compare);
		fprintfDot(fileSettings, "primitive_invertedBox_centre_Y", params.fractal.doubles.primitives.invertedBoxCentre.y, 0.0, compare);
		fprintfDot(fileSettings, "primitive_invertedBox_centre_Z", params.fractal.doubles.primitives.invertedBoxCentre.z, 0.0, compare);
		fprintfDot(fileSettings, "primitive_invertedBox_size_X", params.fractal.doubles.primitives.invertedBoxSize.x, 10.0, compare);
		fprintfDot(fileSettings, "primitive_invertedBox_size_Y", params.fractal.doubles.primitives.invertedBoxSize.y, 10.0, compare);
		fprintfDot(fileSettings, "primitive_invertedBox_size_Z", params.fractal.doubles.primitives.invertedBoxSize.z, 10.0, compare);
		fprintfInt(fileSettings, "primitive_invertedBox_colour_R", params.primitiveInvertedBoxColour.R, 20000, compare);
		fprintfInt(fileSettings, "primitive_invertedBox_colour_G", params.primitiveInvertedBoxColour.G, 20000, compare);
		fprintfInt(fileSettings, "primitive_invertedBox_colour_B", params.primitiveInvertedBoxColour.B, 20000, compare);
		fprintfDot(fileSettings, "primitive_invertedBox_reflect", params.doubles.primitiveInvertedBoxReflect, 0.0, compare);
	}

	if (!compare || params.fractal.primitives.sphereEnable || params.fractal.formula == ocl_custom)
	{
		fprintfInt(fileSettings, "primitive_sphere_enabled", params.fractal.primitives.sphereEnable, false, compare);
		fprintfDot(fileSettings, "primitive_sphere_centre_X", params.fractal.doubles.primitives.sphereCentre.x, 0.0, compare);
		fprintfDot(fileSettings, "primitive_sphere_centre_Y", params.fractal.doubles.primitives.sphereCentre.y, 0.0, compare);
		fprintfDot(fileSettings, "primitive_sphere_centre_Z", params.fractal.doubles.primitives.sphereCentre.z, 0.0, compare);
		fprintfDot(fileSettings, "primitive_sphere_radius", params.fractal.doubles.primitives.sphereRadius, 1.5, compare);
		fprintfInt(fileSettings, "primitive_sphere_colour_R", params.primitiveSphereColour.R, 20000, compare);
		fprintfInt(fileSettings, "primitive_sphere_colour_G", params.primitiveSphereColour.G, 20000, compare);
		fprintfInt(fileSettings, "primitive_sphere_colour_B", params.primitiveSphereColour.B, 20000, compare);
		fprintfDot(fileSettings, "primitive_sphere_reflect", params.doubles.primitiveSphereReflect, 0.0, compare);
	}

	if (!compare || params.fractal.primitives.invertedSphereEnable)
	{
		fprintfInt(fileSettings, "primitive_invertedSphere_enabled", params.fractal.primitives.invertedSphereEnable, false, compare);
		fprintfDot(fileSettings, "primitive_invertedSphere_centre_X", params.fractal.doubles.primitives.invertedSphereCentre.x, 0.0, compare);
		fprintfDot(fileSettings, "primitive_invertedSphere_centre_Y", params.fractal.doubles.primitives.invertedSphereCentre.y, 0.0, compare);
		fprintfDot(fileSettings, "primitive_invertedSphere_centre_Z", params.fractal.doubles.primitives.invertedSphereCentre.z, 0.0, compare);
		fprintfDot(fileSettings, "primitive_invertedSphere_radius", params.fractal.doubles.primitives.invertedSphereRadius, 5.0, compare);
		fprintfInt(fileSettings, "primitive_invertedSphere_colour_R", params.primitiveInvertedSphereColour.R, 20000, compare);
		fprintfInt(fileSettings, "primitive_invertedSphere_colour_G", params.primitiveInvertedSphereColour.G, 20000, compare);
		fprintfInt(fileSettings, "primitive_invertedSphere_colour_B", params.primitiveInvertedSphereColour.B, 20000, compare);
		fprintfDot(fileSettings, "primitive_invertedSphere_reflect", params.doubles.primitiveInvertedSphereReflect, 0.0, compare);
	}

	if (!compare || params.fractal.primitives.waterEnable || params.fractal.formula == ocl_custom)
	{
		fprintfInt(fileSettings, "primitive_water_enabled", params.fractal.primitives.waterEnable, false, compare);
		fprintfDot(fileSettings, "primitive_water_level", params.fractal.doubles.primitives.waterHeight, 0.0, compare);
		fprintfDot(fileSettings, "primitive_water_amplitude", params.fractal.doubles.primitives.waterAmplitude, 0.02, compare);
		fprintfDot(fileSettings, "primitive_water_length", params.fractal.doubles.primitives.waterLength, 0.2, compare);
		fprintfInt(fileSettings, "primitive_water_iterations", params.fractal.primitives.waterIterations, 5, compare);
		fprintfDot(fileSettings, "primitive_water_rotation", params.fractal.doubles.primitives.waterRotation, 0.0, compare);
		fprintfDot(fileSettings, "primitive_water_anim_speed", params.fractal.doubles.primitives.waterAnimSpeed, 0.1, compare);
		fprintfInt(fileSettings, "primitive_water_colour_R", params.primitiveWaterColour.R, 0, compare);
		fprintfInt(fileSettings, "primitive_water_colour_G", params.primitiveWaterColour.G, 5000, compare);
		fprintfInt(fileSettings, "primitive_water_colour_B", params.primitiveWaterColour.B, 10000, compare);
		fprintfDot(fileSettings, "primitive_water_reflect", params.doubles.primitiveWaterReflect, 0.7, compare);
	}

	if (!compare || params.fakeLightsEnabled || params.fractal.formula == ocl_custom)
	{
		fprintfInt(fileSettings, "fake_lights_enabled", params.fakeLightsEnabled, false, compare);
		fprintfDot(fileSettings, "fake_lights_intensity", params.doubles.fakeLightsIntensity, 1.0, compare);
		fprintfDot(fileSettings, "fake_lights_visibility", params.doubles.fakeLightsVisibility, 1.0, compare);
		fprintfDot(fileSettings, "fake_lights_visibility_size", params.doubles.fakeLightsVisibilitySize, 5.0, compare);
		fprintfDot(fileSettings, "fake_lights_orbit_trap_X", params.fractal.doubles.fakeLightsOrbitTrap.x, 2.0, compare);
		fprintfDot(fileSettings, "fake_lights_orbit_trap_Y", params.fractal.doubles.fakeLightsOrbitTrap.y, 0.0, compare);
		fprintfDot(fileSettings, "fake_lights_orbit_trap_Z", params.fractal.doubles.fakeLightsOrbitTrap.z, 0.0, compare);
		fprintfInt(fileSettings, "fake_lights_min_iter", params.fractal.fakeLightsMinIter, 1, compare);
		fprintfInt(fileSettings, "fake_lights_max_iter", params.fractal.fakeLightsMaxIter, 2, compare);
	}

#ifdef CLSUPPORT
	if (!compare || params.fractal.formula == ocl_custom)
	{
		fprintfInt(fileSettings, "ocl_custom_DE_mode", params.fractal.customOCLFormulaDEMode, false, compare);

		if(!compare)
			fprintf(fileSettings, "ocl_custom_formula_name %s;\n", "example");
		else
			fprintf(fileSettings, "ocl_custom_formula_name %s;\n", params.fractal.customOCLFormulaName);

		for (int i = 0; i < 15; ++i)
		{
			sprintf(parameterName, "ocl_custom_par_%d", i);
			fprintfDot(fileSettings, parameterName, params.fractal.doubles.customParameters[i], 0.0, compare);
		}
	}
	fprintfDot(fileSettings, "ocl_delta_DE_step", params.fractal.doubles.deltaDEStep, 1.0e-5, compare);
	fprintfInt(fileSettings, "ocl_DOF_method", params.OpenCLDOFMethod, 0, compare);
#endif

	fprintfInt(fileSettings, "frame_no", params.fractal.frameNo, 0, compare);
	fprintfInt(fileSettings, "tile_no", params.tileCount, 0, compare);

	if(strcmp(filename,"settings/.clipboard"))
	{
		if(!compare)
		{
			fprintf(fileSettings, "file_destination %s;\n", "images/image");
			fprintf(fileSettings, "file_background %s;\n", (string(sharedDir)+"textures/background.jpg").c_str());
			fprintf(fileSettings, "file_envmap %s;\n", (string(sharedDir)+"textures/envmap.jpg").c_str());
			fprintf(fileSettings, "file_lightmap %s;\n", (string(sharedDir)+"textures/lightmap.jpg").c_str());
			fprintf(fileSettings, "file_animation_path %s;\n", "paths/path.txt");
			fprintf(fileSettings, "file_keyframes %s;\n", "keyframes/keyframe");
		}
		else
		{
			fprintf(fileSettings, "file_destination %s;\n", params.file_destination);
			if(params.texturedBackground) fprintf(fileSettings, "file_background %s;\n", params.file_background);
			if(params.doubles.imageAdjustments.reflect > 0 && !params.imageSwitches.raytracedReflections) fprintf(fileSettings, "file_envmap %s;\n", params.file_envmap);
			if(params.global_ilumination && !params.fastGlobalIllumination) fprintf(fileSettings, "file_lightmap %s;\n", params.file_lightmap);
			fprintf(fileSettings, "file_animation_path %s;\n", params.file_path);
			fprintf(fileSettings, "file_keyframes %s;\n", params.file_keyframes);
		}
		fprintf(fileSettings, "palette %s;\n", paletteString);
	}
	fclose(fileSettings);

	delete[] paletteString;
}

void SaveAppSettings(const char *filename, const sAppSettings& appParams)
{
	FILE * fileSettings;
	fileSettings = fopen(filename, "w");

	fprintfInt(fileSettings, "absolute_movement_enable", appParams.absoluteMovementModeEnabled, -1, true);
	fprintfInt(fileSettings, "zoom_by_mouse_click_enable", appParams.zoomByMouseClickEnabled, -1, true);
	fprintfInt(fileSettings, "go_close_to_surface", appParams.goCloseToSurfaceEnabled, -1, true);
	fprintfDot(fileSettings, "camera_movement_relative_step", appParams.cameraMoveStepRelative, -1, true);
	fprintfDot(fileSettings, "camera_movement_absolute_step", appParams.cameraMoveStepAbsolute, -1, true);
	fprintfDot(fileSettings, "camera_rotation_step", appParams.rotationStep, -1, true);
	fprintfDot(fileSettings, "mouse_close_up_ratio", appParams.mouseCloseUpRatio, -1, true);
	fprintf(fileSettings, "net_render_client_port %s;\n", appParams.netRenderClientPort.c_str());
	fprintf(fileSettings, "net_render_client_IP %s;\n", appParams.netRenderClientIP.c_str());
	fprintf(fileSettings, "net_render_server_port %s;\n", appParams.netRenderServerPort.c_str());


#ifdef CLSUPPORT
	fprintfInt(fileSettings, "openCL_use_CPU", appParams.oclUseCPU, -1, true);
	fprintfInt(fileSettings, "openCL_platform_index", appParams.oclPlatformIndex, -1, true);
	fprintfInt(fileSettings, "openCL_device_index", appParams.oclDeviceIndex, -1, true);
	fprintfInt(fileSettings, "openCL_engine", appParams.oclEngineSelection, -1, true);
	fprintfDot(fileSettings, "openCL_cycle_time", appParams.oclCycleTime, -1, true);
	fprintfInt(fileSettings, "openCL_memory_limit", appParams.oclMemoryLimit, -1, true);
	fprintf(fileSettings, "openCL_text_editor %s;\n", appParams.oclTextEditor.c_str());
#endif

	fclose(fileSettings);
}

bool LoadSettings(const char *filename, sParamRender &params, bool disableMessages)
{
	string defaultsFilename = string(data_directory);
	defaultsFilename+="/.defaults";

	if(FileIfExists(defaultsFilename.c_str()))
	{
		LoadSettings2(defaultsFilename.c_str(), params, true);
	}
	else
	{
		printf("ERROR! Missed reference file with defaults: %s\n", defaultsFilename.c_str());
		abort();
	}
	return LoadSettings2(filename, params, disableMessages);
}

bool LoadSettings2(const char *filename, sParamRender &params, bool disableMessages)
{
	paletteLoadedFromSettingsFile = false;

	char str1[100];
	char str2[2000];

	FILE * fileSettings;
	fileSettings = fopen(filename, "r");

	int lineCounter = 0;

	params.settingsVersion = -1;

	if (fileSettings)
	{
		while (!feof(fileSettings))
		{
			lineCounter++;
			int c = fscanf(fileSettings, "%s", str1);
			if (c > 0)
			{
				c = fscanf(fileSettings, "%[ ]", str2);
				c = fscanf(fileSettings, "%[^;]", str2);

				LoadOneSetting(str1, str2, params, disableMessages);
				c = fscanf(fileSettings, "%[^\n]", str2);
			}
		}
		fclose(fileSettings);

		//overriding of parameters defined by command line
		for(std::vector<const char *>::const_iterator it=noGUIdata.overrideStrings.begin();
				it != noGUIdata.overrideStrings.end(); it++)
		{
			char str1[100], str2[2000];
			int c = sscanf(*it, "%99[^= \t]%*1[= ]%1999s", str1, str2);
			if(c != 2)
			{
				printf("Warning! Bad override string: %s [c=%d]\n", *it, c);
				WriteLog("Warning! Bad override string:");
				WriteLog(*it);
			}
			else
			{
				LoadOneSetting(str1, str2, params);
				printf("Prameter overrided: %s = %s\n", str1, str2);
			}
		}

		LoadSettingsPost(params);

		if (params.settingsVersion > MANDELBULBER_VERSION)
		{
			printf("Settings file was created in newer version of Mandelbulber (v. %f)\n", params.settingsVersion);
			if (!noGUI && !disableMessages)
			{
				GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_interface), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
						"Warning! Settings file was created in newer version of Mandelbulber\nfile: %s\nversion: %f", filename, params.settingsVersion);
				gtk_dialog_run(GTK_DIALOG(dialog));
				gtk_widget_destroy(dialog);
			}
		}
		return true;
	}
	else
	{
		//printf("Can't open settings file: %s\n", filename);
		return false;
	}
}

bool LoadOneSetting(const char* str1, const char *str2, sParamRender &params, bool disableMessages)
{
	char str3[100];

	//IFS params
	bool IFSresult = false;
	for (int i = 0; i < IFS_VECTOR_COUNT; i++)
	{
		sprintf(str3, "IFS_%d_x", i);
		if (!strcmp(str1, str3))
		{
			params.fractal.IFS.doubles.direction[i].x = atof2(str2);
			IFSresult = true;
			break;
		}
		sprintf(str3, "IFS_%d_y", i);
		if (!strcmp(str1, str3))
		{
			params.fractal.IFS.doubles.direction[i].y = atof2(str2);
			IFSresult = true;
			break;
		}
		sprintf(str3, "IFS_%d_z", i);
		if (!strcmp(str1, str3))
		{
			params.fractal.IFS.doubles.direction[i].z = atof2(str2);
			IFSresult = true;
			break;
		}
		sprintf(str3, "IFS_%d_alfa", i);
		if (!strcmp(str1, str3))
		{
			params.fractal.IFS.doubles.alfa[i] = atof2(str2);
			IFSresult = true;
			break;
		}
		sprintf(str3, "IFS_%d_beta", i);
		if (!strcmp(str1, str3))
		{
			params.fractal.IFS.doubles.beta[i] = atof2(str2);
			IFSresult = true;
			break;
		}
		sprintf(str3, "IFS_%d_gamma", i);
		if (!strcmp(str1, str3))
		{
			params.fractal.IFS.doubles.gamma[i] = atof2(str2);
			IFSresult = true;
			break;
		}
		sprintf(str3, "IFS_%d_distance", i);
		if (!strcmp(str1, str3))
		{
			params.fractal.IFS.doubles.distance[i] = atof2(str2);
			IFSresult = true;
			break;
		}
		sprintf(str3, "IFS_%d_intensity", i);
		if (!strcmp(str1, str3))
		{
			params.fractal.IFS.doubles.intensity[i] = atof2(str2);
			IFSresult = true;
			break;
		}
		sprintf(str3, "IFS_%d_enabled", i);
		if (!strcmp(str1, str3))
		{
			params.fractal.IFS.enabled[i] = atoi(str2);
			IFSresult = true;
			break;
		}
	}

	if (IFSresult) IFSresult = false;
	else if (!strcmp(str1, "Mandelbulber")) params.settingsVersion = atof2(str2);
	else if (!strcmp(str1, "image_width")) params.image_width = atoi(str2);
	else if (!strcmp(str1, "image_height")) params.image_height = atoi(str2);
	else if (!strcmp(str1, "tiles")) params.noOfTiles = atoi(str2);
	else if (!strcmp(str1, "x_min")) params.fractal.doubles.amin = atof2(str2);
	else if (!strcmp(str1, "x_max")) params.fractal.doubles.amax = atof2(str2);
	else if (!strcmp(str1, "y_min")) params.fractal.doubles.bmin = atof2(str2);
	else if (!strcmp(str1, "y_max")) params.fractal.doubles.bmax = atof2(str2);
	else if (!strcmp(str1, "z_min")) params.fractal.doubles.cmin = atof2(str2);
	else if (!strcmp(str1, "z_max")) params.fractal.doubles.cmax = atof2(str2);
	else if (!strcmp(str1, "view_point_x")) params.doubles.vp.x = atof2(str2);
	else if (!strcmp(str1, "view_point_y")) params.doubles.vp.y = atof2(str2);
	else if (!strcmp(str1, "view_point_z")) params.doubles.vp.z = atof2(str2);
	else if (!strcmp(str1, "angle_alfa")) params.doubles.alpha = atof2(str2) / 180.0 * M_PI;
	else if (!strcmp(str1, "angle_beta")) params.doubles.beta = atof2(str2) / 180.0 * M_PI;
	else if (!strcmp(str1, "angle_gamma")) params.doubles.gamma = atof2(str2) / 180.0 * M_PI;
	else if (!strcmp(str1, "zoom")) params.doubles.zoom = atof2(str2);
	else if (!strcmp(str1, "perspective")) params.doubles.persp = atof2(str2);
	else if (!strcmp(str1, "formula")) params.fractal.formula = (enumFractalFormula) atoi(str2);
	else if (!strcmp(str1, "power")) params.fractal.doubles.power = atof2(str2);
	else if (!strcmp(str1, "N")) params.fractal.doubles.N = atoi(str2);
	else if (!strcmp(str1, "minN")) params.fractal.minN = atoi(str2);
	else if (!strcmp(str1, "fractal_constant_factor")) params.fractal.doubles.constantFactor = atof2(str2);
	else if (!strcmp(str1, "quality")) params.doubles.quality = atof2(str2);
	else if (!strcmp(str1, "smoothness")) params.doubles.smoothness = atof2(str2);
	else if (!strcmp(str1, "julia_mode")) params.fractal.juliaMode = atoi(str2);
	else if (!strcmp(str1, "julia_a")) params.fractal.doubles.julia.x = atof2(str2);
	else if (!strcmp(str1, "julia_b")) params.fractal.doubles.julia.y = atof2(str2);
	else if (!strcmp(str1, "julia_c")) params.fractal.doubles.julia.z = atof2(str2);
	else if (!strcmp(str1, "tglad_folding_mode")) params.fractal.tgladFoldingMode = atoi(str2);
	else if (!strcmp(str1, "folding_limit")) params.fractal.doubles.foldingLimit = atof2(str2);
	else if (!strcmp(str1, "folding_value")) params.fractal.doubles.foldingValue = atof2(str2);
	else if (!strcmp(str1, "spherical_folding_mode")) params.fractal.sphericalFoldingMode = atoi(str2);
	else if (!strcmp(str1, "spherical_folding_fixed")) params.fractal.doubles.foldingSphericalFixed = atof2(str2);
	else if (!strcmp(str1, "spherical_folding_min")) params.fractal.doubles.foldingSphericalMin = atof2(str2);
	else if (!strcmp(str1, "IFS_folding_mode")) params.fractal.IFS.foldingMode = atoi(str2);
	else if (!strcmp(str1, "iteration_threshold_mode")) params.fractal.iterThresh = atoi(str2);
	else if (!strcmp(str1, "analityc_DE_mode")) params.fractal.analitycDE = atoi(str2);
	else if (!strcmp(str1, "DE_factor")) params.doubles.DE_factor = atof2(str2);
	else if (!strcmp(str1, "brightness")) params.doubles.imageAdjustments.brightness = atof2(str2);
	else if (!strcmp(str1, "contrast")) params.doubles.imageAdjustments.contrast = atof2(str2);
	else if (!strcmp(str1, "gamma")) params.doubles.imageAdjustments.imageGamma = atof2(str2);
	else if (!strcmp(str1, "hdr")) params.imageSwitches.hdrEnabled = atoi(str2);
	else if (!strcmp(str1, "ambient")) params.doubles.imageAdjustments.ambient = atof2(str2);
	else if (!strcmp(str1, "reflect")) params.doubles.imageAdjustments.reflect = atof2(str2);
	else if (!strcmp(str1, "shadows_intensity")) params.doubles.imageAdjustments.directLight = atof2(str2);
	else if (!strcmp(str1, "shadows_cone_angle")) params.doubles.shadowConeAngle = atof2(str2);
	else if (!strcmp(str1, "ambient_occlusion")) params.doubles.imageAdjustments.globalIlum = atof2(str2);
	else if (!strcmp(str1, "ambient_occlusion_quality")) params.globalIlumQuality = atoi(str2);
	else if (!strcmp(str1, "ambient_occlusion_fast_tune")) params.doubles.fastAoTune = atof2(str2);
	else if (!strcmp(str1, "shading")) params.doubles.imageAdjustments.shading = atof2(str2);
	else if (!strcmp(str1, "specular")) params.doubles.imageAdjustments.specular = atof2(str2);
	else if (!strcmp(str1, "glow_intensity")) params.doubles.imageAdjustments.glow_intensity = atof2(str2);
	else if (!strcmp(str1, "glow_color_1_R")) params.effectColours.glow_color1.R = atoi(str2);
	else if (!strcmp(str1, "glow_color_1_G")) params.effectColours.glow_color1.G = atoi(str2);
	else if (!strcmp(str1, "glow_color_1_B")) params.effectColours.glow_color1.B = atoi(str2);
	else if (!strcmp(str1, "glow_color_2_R")) params.effectColours.glow_color2.R = atoi(str2);
	else if (!strcmp(str1, "glow_color_2_G")) params.effectColours.glow_color2.G = atoi(str2);
	else if (!strcmp(str1, "glow_color_2_B")) params.effectColours.glow_color2.B = atoi(str2);
	else if (!strcmp(str1, "background_color_1_R")) params.background_color1.R = atoi(str2);
	else if (!strcmp(str1, "background_color_1_G")) params.background_color1.G = atoi(str2);
	else if (!strcmp(str1, "background_color_1_B")) params.background_color1.B = atoi(str2);
	else if (!strcmp(str1, "background_color_2_R")) params.background_color2.R = atoi(str2);
	else if (!strcmp(str1, "background_color_2_G")) params.background_color2.G = atoi(str2);
	else if (!strcmp(str1, "background_color_2_B")) params.background_color2.B = atoi(str2);
	else if (!strcmp(str1, "background_color_3_R")) params.background_color3.R = atoi(str2);
	else if (!strcmp(str1, "background_color_3_G")) params.background_color3.G = atoi(str2);
	else if (!strcmp(str1, "background_color_3_B")) params.background_color3.B = atoi(str2);
	else if (!strcmp(str1, "fog_colour_1_R")) params.fogColour1.R = atoi(str2);
	else if (!strcmp(str1, "fog_colour_1_G")) params.fogColour1.G = atoi(str2);
	else if (!strcmp(str1, "fog_colour_1_B")) params.fogColour1.B = atoi(str2);
	else if (!strcmp(str1, "fog_colour_2_R")) params.fogColour2.R = atoi(str2);
	else if (!strcmp(str1, "fog_colour_2_G")) params.fogColour2.G = atoi(str2);
	else if (!strcmp(str1, "fog_colour_2_B")) params.fogColour2.B = atoi(str2);
	else if (!strcmp(str1, "fog_colour_3_R")) params.fogColour3.R = atoi(str2);
	else if (!strcmp(str1, "fog_colour_3_G")) params.fogColour3.G = atoi(str2);
	else if (!strcmp(str1, "fog_colour_3_B")) params.fogColour3.B = atoi(str2);
	else if (!strcmp(str1, "textured_background")) params.texturedBackground = atoi(str2);
	else if (!strcmp(str1, "background_as_fuldome")) params.background_as_fulldome = atoi(str2);
	else if (!strcmp(str1, "shadows_enabled")) params.shadow = atoi(str2);
	else if (!strcmp(str1, "ambient_occlusion_enabled")) params.global_ilumination = atoi(str2);
	else if (!strcmp(str1, "fast_ambient_occlusion_mode")) params.fastGlobalIllumination = atoi(str2);
	else if (!strcmp(str1, "fractal_color")) params.imageSwitches.coloringEnabled = atoi(str2);
	else if (!strcmp(str1, "coloring_random_seed")) params.coloring_seed = atoi(str2);
	else if (!strcmp(str1, "coloring_saturation")) params.doubles.colourSaturation = atof2(str2);
	else if (!strcmp(str1, "coloring_speed")) params.doubles.imageAdjustments.coloring_speed = atof2(str2);
	else if (!strcmp(str1, "coloring_palette_offset")) params.doubles.imageAdjustments.paletteOffset = atof2(str2);
	else if (!strcmp(str1, "slow_shading")) params.slowShading = atoi(str2);
	else if (!strcmp(str1, "limits_enabled")) params.fractal.limits_enabled = atoi(str2);
	else if (!strcmp(str1, "post_fog_enabled")) params.imageSwitches.fogEnabled = atoi(str2);
	else if (!strcmp(str1, "post_fog_visibility")) params.doubles.imageAdjustments.fogVisibility = atof2(str2);
	else if (!strcmp(str1, "post_fog_color_R")) params.effectColours.fogColor.R = atoi(str2);
	else if (!strcmp(str1, "post_fog_color_G")) params.effectColours.fogColor.G = atoi(str2);
	else if (!strcmp(str1, "post_fog_color_B")) params.effectColours.fogColor.B = atoi(str2);
	else if (!strcmp(str1, "post_SSAO_enabled")) params.SSAOEnabled = atoi(str2);
	else if (!strcmp(str1, "post_SSAO_quality")) params.SSAOQuality = atoi(str2);
	else if (!strcmp(str1, "post_DOF_enabled")) params.DOFEnabled = atoi(str2);
	else if (!strcmp(str1, "post_DOF_focus")) params.doubles.DOFFocus = atof2(str2);
	else if (!strcmp(str1, "post_DOF_radius")) params.doubles.DOFRadius = atof2(str2);
	else if (!strcmp(str1, "main_light_intensity")) params.doubles.imageAdjustments.mainLightIntensity = atof2(str2);
	else if (!strcmp(str1, "main_light_alfa")) params.doubles.mainLightAlpha = atof2(str2);
	else if (!strcmp(str1, "main_light_beta")) params.doubles.mainLightBeta = atof2(str2);
	else if (!strcmp(str1, "main_light_colour_R")) params.effectColours.mainLightColour.R = atoi(str2);
	else if (!strcmp(str1, "main_light_colour_G")) params.effectColours.mainLightColour.G = atoi(str2);
	else if (!strcmp(str1, "main_light_colour_B")) params.effectColours.mainLightColour.B = atoi(str2);
	else if (!strcmp(str1, "aux_light_intensity")) params.doubles.auxLightIntensity = atof2(str2);
	else if (!strcmp(str1, "aux_light_random_seed")) params.auxLightRandomSeed = atoi(str2);
	else if (!strcmp(str1, "aux_light_number")) params.auxLightNumber = atoi(str2);
	else if (!strcmp(str1, "aux_light_max_dist")) params.doubles.auxLightMaxDist = atof2(str2);
	else if (!strcmp(str1, "aux_light_distribution_radius")) params.doubles.auxLightDistributionRadius = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_1_x")) params.doubles.auxLightPre[0].x = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_1_y")) params.doubles.auxLightPre[0].y = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_1_z")) params.doubles.auxLightPre[0].z = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_1_intensity")) params.doubles.auxLightPreIntensity[0] = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_2_x")) params.doubles.auxLightPre[1].x = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_2_y")) params.doubles.auxLightPre[1].y = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_2_z")) params.doubles.auxLightPre[1].z = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_2_intensity")) params.doubles.auxLightPreIntensity[1] = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_3_x")) params.doubles.auxLightPre[2].x = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_3_y")) params.doubles.auxLightPre[2].y = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_3_z")) params.doubles.auxLightPre[2].z = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_3_intensity")) params.doubles.auxLightPreIntensity[2] = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_4_x")) params.doubles.auxLightPre[3].x = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_4_y")) params.doubles.auxLightPre[3].y = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_4_z")) params.doubles.auxLightPre[3].z = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_4_intensity")) params.doubles.auxLightPreIntensity[3] = atof2(str2);
	else if (!strcmp(str1, "aux_light_predefined_1_enabled")) params.auxLightPreEnabled[0] = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_2_enabled")) params.auxLightPreEnabled[1] = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_3_enabled")) params.auxLightPreEnabled[2] = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_4_enabled")) params.auxLightPreEnabled[3] = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_1_colour_R")) params.auxLightPreColour[0].R = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_1_colour_G")) params.auxLightPreColour[0].G = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_1_colour_B")) params.auxLightPreColour[0].B = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_2_colour_R")) params.auxLightPreColour[1].R = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_2_colour_G")) params.auxLightPreColour[1].G = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_2_colour_B")) params.auxLightPreColour[1].B = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_3_colour_R")) params.auxLightPreColour[2].R = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_3_colour_G")) params.auxLightPreColour[2].G = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_3_colour_B")) params.auxLightPreColour[2].B = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_4_colour_R")) params.auxLightPreColour[3].R = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_4_colour_G")) params.auxLightPreColour[3].G = atoi(str2);
	else if (!strcmp(str1, "aux_light_predefined_4_colour_B")) params.auxLightPreColour[3].B = atoi(str2);
	else if (!strcmp(str1, "aux_light_visibility")) params.doubles.auxLightVisibility = atof2(str2);
	else if (!strcmp(str1, "aux_light_random_center_X")) params.doubles.auxLightRandomCenter.x = atof2(str2);
	else if (!strcmp(str1, "aux_light_random_center_Y")) params.doubles.auxLightRandomCenter.y = atof2(str2);
	else if (!strcmp(str1, "aux_light_random_center_Z")) params.doubles.auxLightRandomCenter.z = atof2(str2);

	else if (!strcmp(str1, "IFS_scale")) params.fractal.IFS.doubles.scale = atof2(str2);
	else if (!strcmp(str1, "IFS_rot_alfa")) params.fractal.IFS.doubles.rotationAlfa = atof2(str2);
	else if (!strcmp(str1, "IFS_rot_beta")) params.fractal.IFS.doubles.rotationBeta = atof2(str2);
	else if (!strcmp(str1, "IFS_rot_gamma")) params.fractal.IFS.doubles.rotationGamma = atof2(str2);
	else if (!strcmp(str1, "IFS_offsetX")) params.fractal.IFS.doubles.offset.x = atof2(str2);
	else if (!strcmp(str1, "IFS_offsetY")) params.fractal.IFS.doubles.offset.y = atof2(str2);
	else if (!strcmp(str1, "IFS_offsetZ")) params.fractal.IFS.doubles.offset.z = atof2(str2);
	else if (!strcmp(str1, "IFS_edgeX")) params.fractal.IFS.doubles.edge.x = atof2(str2);
	else if (!strcmp(str1, "IFS_edgeY")) params.fractal.IFS.doubles.edge.y = atof2(str2);
	else if (!strcmp(str1, "IFS_edgeZ")) params.fractal.IFS.doubles.edge.z = atof2(str2);
	else if (!strcmp(str1, "IFS_absX")) params.fractal.IFS.absX = atof(str2);
	else if (!strcmp(str1, "IFS_absY")) params.fractal.IFS.absY = atof(str2);
	else if (!strcmp(str1, "IFS_absZ")) params.fractal.IFS.absZ = atof(str2);
	else if (!strcmp(str1, "IFS_menger_sponge_mode")) params.fractal.IFS.mengerSpongeMode = atof(str2);

	else if (!strcmp(str1, "start_frame")) params.startFrame = atoi(str2);
	else if (!strcmp(str1, "end_frame")) params.endFrame = atoi(str2);
	else if (!strcmp(str1, "frames_per_keyframe")) params.framesPerKeyframe = atoi(str2);

  else if (!strcmp(str1, "hybrid_cyclic")) params.fractal.hybridCyclic = atoi(str2);

	else if (!strcmp(str1, "fish_eye")) params.perspectiveType = (enumPerspectiveType)atoi(str2);
	else if (!strcmp(str1, "fish_eye_180cut")) params.fishEyeCut = atoi(str2);

	else if (!strcmp(str1, "stereo_enabled")) params.stereoEnabled = atoi(str2);
	else if (!strcmp(str1, "stereo_eye_distance")) params.doubles.stereoEyeDistance = atof2(str2);

	else if (!strcmp(str1, "mandelbox_scale")) params.fractal.mandelbox.doubles.scale = atof2(str2);
	else if (!strcmp(str1, "mandelbox_folding_limit")) params.fractal.mandelbox.doubles.foldingLimit = atof2(str2);
	else if (!strcmp(str1, "mandelbox_folding_value")) params.fractal.mandelbox.doubles.foldingValue = atof2(str2);
	else if (!strcmp(str1, "mandelbox_folding_min_radius")) params.fractal.mandelbox.doubles.foldingSphericalMin = atof2(str2);
	else if (!strcmp(str1, "mandelbox_folding_fixed_radius")) params.fractal.mandelbox.doubles.foldingSphericalFixed = atof2(str2);
	else if (!strcmp(str1, "mandelbox_sharpness")) params.fractal.mandelbox.doubles.sharpness = atof2(str2);
	else if (!strcmp(str1, "mandelbox_offset_X")) params.fractal.mandelbox.doubles.offset.x = atof2(str2);
	else if (!strcmp(str1, "mandelbox_offset_Y")) params.fractal.mandelbox.doubles.offset.y = atof2(str2);
	else if (!strcmp(str1, "mandelbox_offset_Z")) params.fractal.mandelbox.doubles.offset.z = atof2(str2);
	else if (!strcmp(str1, "mandelbox_color_R")) params.fractal.mandelbox.doubles.colorFactorR = atof2(str2);
	else if (!strcmp(str1, "mandelbox_color_X")) params.fractal.mandelbox.doubles.colorFactorX = atof2(str2);
	else if (!strcmp(str1, "mandelbox_color_Y")) params.fractal.mandelbox.doubles.colorFactorY = atof2(str2);
	else if (!strcmp(str1, "mandelbox_color_Z")) params.fractal.mandelbox.doubles.colorFactorZ = atof2(str2);
	else if (!strcmp(str1, "mandelbox_color_Sp1")) params.fractal.mandelbox.doubles.colorFactorSp1 = atof2(str2);
	else if (!strcmp(str1, "mandelbox_color_Sp2")) params.fractal.mandelbox.doubles.colorFactorSp2 = atof2(str2);
	else if (!strcmp(str1, "mandelbox_rotation_enabled")) params.fractal.mandelbox.rotationsEnabled = atoi(str2);
	else if (!strcmp(str1, "mandelbox_fold_mode")) params.fractal.genFoldBox.type = (enumGeneralizedFoldBoxType)atoi(str2);
	else if (!strcmp(str1, "mandelbox_solid")) params.fractal.mandelbox.doubles.solid = atof2(str2);
	else if (!strcmp(str1, "mandelbox_melt")) params.fractal.mandelbox.doubles.melt = atof2(str2);

	else if (!strcmp(str1, "view_distance_max")) params.doubles.viewDistanceMax = atof2(str2);
	else if (!strcmp(str1, "view_distance_min")) params.doubles.viewDistanceMin = atof2(str2);
	else if (!strcmp(str1, "interior_mode")) params.fractal.interiorMode = atoi(str2);
	else if (!strcmp(str1, "linear_DE_mode")) params.fractal.linearDEmode = atoi(str2);
	else if (!strcmp(str1, "constant_DE_threshold")) params.fractal.constantDEThreshold = atoi(str2);

	else if (!strcmp(str1, "FoldingIntPow_folding_factor")) params.fractal.doubles.FoldingIntPowFoldFactor = atof2(str2);
	else if (!strcmp(str1, "FoldingIntPow_z_factor")) params.fractal.doubles.FoldingIntPowZfactor = atof2(str2);
	else if (!strcmp(str1, "penetrating_lights")) params.penetratingLights = atoi(str2);

	else if (!strcmp(str1, "raytraced_reflections")) params.imageSwitches.raytracedReflections = atoi(str2);
	else if (!strcmp(str1, "reflections_max")) params.reflectionsMax = atoi(str2);

	else if (!strcmp(str1, "mandelbox_vary_scale_vary")) params.fractal.mandelbox.doubles.vary4D.scaleVary = atof2(str2);
	else if (!strcmp(str1, "mandelbox_vary_fold")) params.fractal.mandelbox.doubles.vary4D.fold = atof2(str2);
	else if (!strcmp(str1, "mandelbox_vary_minr")) params.fractal.mandelbox.doubles.vary4D.minR = atof2(str2);
	else if (!strcmp(str1, "mandelbox_vary_rpower")) params.fractal.mandelbox.doubles.vary4D.rPower = atof2(str2);
	else if (!strcmp(str1, "mandelbox_vary_wadd")) params.fractal.mandelbox.doubles.vary4D.wadd = atof2(str2);

	else if (!strcmp(str1, "c_add")) params.fractal.doubles.cadd = atof2(str2);

	else if (!strcmp(str1, "volumetric_fog_density")) params.doubles.fogDensity = atof2(str2);
	else if (!strcmp(str1, "volumetric_fog_colour_1_distance")) params.doubles.fogColour1Distance = atof2(str2);
	else if (!strcmp(str1, "volumetric_fog_colour_2_distance")) params.doubles.fogColour2Distance = atof2(str2);
	else if (!strcmp(str1, "volumetric_fog_distance_factor")) params.doubles.fogDistanceFactor = atof2(str2);

	else if (!strcmp(str1, "iteration_fog_enable")) params.imageSwitches.iterFogEnabled = atoi(str2);
	else if (!strcmp(str1, "iteration_fog_opacity")) params.doubles.iterFogOpacity = atof2(str2);
	else if (!strcmp(str1, "iteration_fog_opacity_trim")) params.doubles.iterFogOpacityTrim = atof2(str2);

	else if (!strcmp(str1, "primitive_only_plane")) params.fractal.primitives.onlyPlane = atoi(str2);

	else if (!strcmp(str1, "primitive_plane_enabled")) params.fractal.primitives.planeEnable = atoi(str2);
	else if (!strcmp(str1, "primitive_plane_centre_X")) params.fractal.doubles.primitives.planeCentre.x = atof2(str2);
	else if (!strcmp(str1, "primitive_plane_centre_Y")) params.fractal.doubles.primitives.planeCentre.y = atof2(str2);
	else if (!strcmp(str1, "primitive_plane_centre_Z")) params.fractal.doubles.primitives.planeCentre.z = atof2(str2);
	else if (!strcmp(str1, "primitive_plane_normal_X")) params.fractal.doubles.primitives.planeNormal.x = atof2(str2);
	else if (!strcmp(str1, "primitive_plane_normal_Y")) params.fractal.doubles.primitives.planeNormal.y = atof2(str2);
	else if (!strcmp(str1, "primitive_plane_normal_Z")) params.fractal.doubles.primitives.planeNormal.z = atof2(str2);
	else if (!strcmp(str1, "primitive_plane_colour_R")) params.primitivePlaneColour.R = atoi(str2);
	else if (!strcmp(str1, "primitive_plane_colour_G")) params.primitivePlaneColour.G = atoi(str2);
	else if (!strcmp(str1, "primitive_plane_colour_B")) params.primitivePlaneColour.B = atoi(str2);
	else if (!strcmp(str1, "primitive_plane_reflect")) params.doubles.primitivePlaneReflect = atof2(str2);

	else if (!strcmp(str1, "primitive_box_enabled")) params.fractal.primitives.boxEnable = atoi(str2);
	else if (!strcmp(str1, "primitive_box_centre_X")) params.fractal.doubles.primitives.boxCentre.x = atof2(str2);
	else if (!strcmp(str1, "primitive_box_centre_Y")) params.fractal.doubles.primitives.boxCentre.y = atof2(str2);
	else if (!strcmp(str1, "primitive_box_centre_Z")) params.fractal.doubles.primitives.boxCentre.z = atof2(str2);
	else if (!strcmp(str1, "primitive_box_size_X")) params.fractal.doubles.primitives.boxSize.x = atof2(str2);
	else if (!strcmp(str1, "primitive_box_size_Y")) params.fractal.doubles.primitives.boxSize.y = atof2(str2);
	else if (!strcmp(str1, "primitive_box_size_Z")) params.fractal.doubles.primitives.boxSize.z = atof2(str2);
	else if (!strcmp(str1, "primitive_box_colour_R")) params.primitiveBoxColour.R = atoi(str2);
	else if (!strcmp(str1, "primitive_box_colour_G")) params.primitiveBoxColour.G = atoi(str2);
	else if (!strcmp(str1, "primitive_box_colour_B")) params.primitiveBoxColour.B = atoi(str2);
	else if (!strcmp(str1, "primitive_box_reflect")) params.doubles.primitiveBoxReflect = atof2(str2);

	else if (!strcmp(str1, "primitive_invertedBox_enabled")) params.fractal.primitives.invertedBoxEnable = atoi(str2);
	else if (!strcmp(str1, "primitive_invertedBox_centre_X")) params.fractal.doubles.primitives.invertedBoxCentre.x = atof2(str2);
	else if (!strcmp(str1, "primitive_invertedBox_centre_Y")) params.fractal.doubles.primitives.invertedBoxCentre.y = atof2(str2);
	else if (!strcmp(str1, "primitive_invertedBox_centre_Z")) params.fractal.doubles.primitives.invertedBoxCentre.z = atof2(str2);
	else if (!strcmp(str1, "primitive_invertedBox_size_X")) params.fractal.doubles.primitives.invertedBoxSize.x = atof2(str2);
	else if (!strcmp(str1, "primitive_invertedBox_size_Y")) params.fractal.doubles.primitives.invertedBoxSize.y = atof2(str2);
	else if (!strcmp(str1, "primitive_invertedBox_size_Z")) params.fractal.doubles.primitives.invertedBoxSize.z = atof2(str2);
	else if (!strcmp(str1, "primitive_invertedBox_colour_R")) params.primitiveInvertedBoxColour.R = atoi(str2);
	else if (!strcmp(str1, "primitive_invertedBox_colour_G")) params.primitiveInvertedBoxColour.G = atoi(str2);
	else if (!strcmp(str1, "primitive_invertedBox_colour_B")) params.primitiveInvertedBoxColour.B = atoi(str2);
	else if (!strcmp(str1, "primitive_invertedBox_reflect")) params.doubles.primitiveInvertedBoxReflect = atof2(str2);

	else if (!strcmp(str1, "primitive_sphere_enabled")) params.fractal.primitives.sphereEnable = atoi(str2);
	else if (!strcmp(str1, "primitive_sphere_centre_X")) params.fractal.doubles.primitives.sphereCentre.x = atof2(str2);
	else if (!strcmp(str1, "primitive_sphere_centre_Y")) params.fractal.doubles.primitives.sphereCentre.y = atof2(str2);
	else if (!strcmp(str1, "primitive_sphere_centre_Z")) params.fractal.doubles.primitives.sphereCentre.z = atof2(str2);
	else if (!strcmp(str1, "primitive_sphere_radius")) params.fractal.doubles.primitives.sphereRadius = atof2(str2);
	else if (!strcmp(str1, "primitive_sphere_colour_R")) params.primitiveSphereColour.R = atoi(str2);
	else if (!strcmp(str1, "primitive_sphere_colour_G")) params.primitiveSphereColour.G = atoi(str2);
	else if (!strcmp(str1, "primitive_sphere_colour_B")) params.primitiveSphereColour.B = atoi(str2);
	else if (!strcmp(str1, "primitive_sphere_reflect")) params.doubles.primitiveSphereReflect = atof2(str2);

	else if (!strcmp(str1, "primitive_invertedSphere_enabled")) params.fractal.primitives.invertedSphereEnable = atoi(str2);
	else if (!strcmp(str1, "primitive_invertedSphere_centre_X")) params.fractal.doubles.primitives.invertedSphereCentre.x = atof2(str2);
	else if (!strcmp(str1, "primitive_invertedSphere_centre_Y")) params.fractal.doubles.primitives.invertedSphereCentre.y = atof2(str2);
	else if (!strcmp(str1, "primitive_invertedSphere_centre_Z")) params.fractal.doubles.primitives.invertedSphereCentre.z = atof2(str2);
	else if (!strcmp(str1, "primitive_invertedSphere_radius")) params.fractal.doubles.primitives.invertedSphereRadius = atof2(str2);
	else if (!strcmp(str1, "primitive_invertedSphere_colour_R")) params.primitiveInvertedSphereColour.R = atoi(str2);
	else if (!strcmp(str1, "primitive_invertedSphere_colour_G")) params.primitiveInvertedSphereColour.G = atoi(str2);
	else if (!strcmp(str1, "primitive_invertedSphere_colour_B")) params.primitiveInvertedSphereColour.B = atoi(str2);
	else if (!strcmp(str1, "primitive_invertedSphere_reflect")) params.doubles.primitiveInvertedSphereReflect = atof2(str2);

	else if (!strcmp(str1, "primitive_water_enabled")) params.fractal.primitives.waterEnable = atoi(str2);
	else if (!strcmp(str1, "primitive_water_level")) params.fractal.doubles.primitives.waterHeight = atof2(str2);
	else if (!strcmp(str1, "primitive_water_amplitude")) params.fractal.doubles.primitives.waterAmplitude = atof2(str2);
	else if (!strcmp(str1, "primitive_water_length")) params.fractal.doubles.primitives.waterLength = atof2(str2);
	else if (!strcmp(str1, "primitive_water_iterations")) params.fractal.primitives.waterIterations = atoi(str2);
	else if (!strcmp(str1, "primitive_water_rotation")) params.fractal.doubles.primitives.waterRotation = atof2(str2);
	else if (!strcmp(str1, "primitive_water_anim_speed")) params.fractal.doubles.primitives.waterAnimSpeed = atof2(str2);
	else if (!strcmp(str1, "primitive_water_colour_R")) params.primitiveWaterColour.R = atoi(str2);
	else if (!strcmp(str1, "primitive_water_colour_G")) params.primitiveWaterColour.G = atoi(str2);
	else if (!strcmp(str1, "primitive_water_colour_B")) params.primitiveWaterColour.B = atoi(str2);
	else if (!strcmp(str1, "primitive_water_reflect")) params.doubles.primitiveWaterReflect = atof2(str2);

	else if (!strcmp(str1, "fake_lights_enabled")) params.fakeLightsEnabled = atoi(str2);
	else if (!strcmp(str1, "fake_lights_min_iter")) params.fractal.fakeLightsMinIter = atoi(str2);
	else if (!strcmp(str1, "fake_lights_max_iter")) params.fractal.fakeLightsMaxIter = atoi(str2);
	else if (!strcmp(str1, "fake_lights_visibility")) params.doubles.fakeLightsVisibility = atof2(str2);
	else if (!strcmp(str1, "fake_lights_visibility_size")) params.doubles.fakeLightsVisibilitySize = atof2(str2);
	else if (!strcmp(str1, "fake_lights_intensity")) params.doubles.fakeLightsIntensity = atof2(str2);
	else if (!strcmp(str1, "fake_lights_orbit_trap_X")) params.fractal.doubles.fakeLightsOrbitTrap.x = atof2(str2);
	else if (!strcmp(str1, "fake_lights_orbit_trap_Y")) params.fractal.doubles.fakeLightsOrbitTrap.y = atof2(str2);
	else if (!strcmp(str1, "fake_lights_orbit_trap_Z")) params.fractal.doubles.fakeLightsOrbitTrap.z = atof2(str2);

#ifdef CLSUPPORT
	else if (!strcmp(str1, "ocl_custom_DE_mode")) params.fractal.customOCLFormulaDEMode = (enumOCLDEMode)atoi(str2);
	else if (!strcmp(str1, "ocl_custom_formula_name")) strcpy(params.fractal.customOCLFormulaName,str2);
	else if (!strcmp(str1, "ocl_delta_DE_step")) params.fractal.doubles.deltaDEStep = atof2(str2);
	else if (!strcmp(str1, "ocl_DOF_method")) params.OpenCLDOFMethod = atoi(str2);
#endif

	else if (!strcmp(str1, "frame_no")) params.fractal.frameNo = atoi(str2);
	else if (!strcmp(str1, "tile_no")) params.tileCount = atoi(str2);

	else if (!strcmp(str1, "file_destination")) strcpy(params.file_destination, str2);
	else if (!strcmp(str1, "file_background")) strcpy(params.file_background, str2);
	else if (!strcmp(str1, "file_envmap")) strcpy(params.file_envmap, str2);
	else if (!strcmp(str1, "file_lightmap")) strcpy(params.file_lightmap, str2);
	else if (!strcmp(str1, "file_animation_path")) strcpy(params.file_path, str2);
	else if (!strcmp(str1, "file_keyframes")) strcpy(params.file_keyframes, str2);
	else if (!strcmp(str1, "palette")) GetPaletteFromString(params.palette, str2);
	else
	{
		int matched = false;
		char buf[100];
		for (int i = 0; i < 5; ++i) {
			sprintf(buf, "volumetric_light_enabled_%d", i);
			if (!strcmp(str1, buf)) {
				params.volumetricLightEnabled[i] = atoi(str2);
				matched = true;
				break;
			}
			sprintf(buf, "volumetric_light_intensity_%d", i);
			if (!strcmp(str1, buf)) {
				params.doubles.volumetricLightIntensity[i] = atof2(str2);
				matched = true;
				break;
			}
		}

		for (int i = 1; i <= HYBRID_COUNT; ++i)
		{
			sprintf(buf, "hybrid_formula_%d", i);
			if (!strcmp(str1, buf))
			{
				params.fractal.hybridFormula[i - 1] = (enumFractalFormula) atoi(str2);
				matched = true;
				break;
			}
			sprintf(buf, "hybrid_iterations_%d", i);
			if (!strcmp(str1, buf))
			{
				params.fractal.hybridIters[i - 1] = atoi(str2);
				matched = true;
				break;
			}
			sprintf(buf, "hybrid_power_%d", i);
			if (!strcmp(str1, buf))
			{
				params.fractal.doubles.hybridPower[i - 1] = atof2(str2);
				matched = true;
				break;
			}
		}

		for (int component = 0; component < 3; ++component) {
			sprintf(buf, "mandelbox_rotation_main_%s", component_names[component]);
			if (!strcmp(str1, buf)) {
				params.fractal.mandelbox.doubles.rotationMain[component] = atof2(str2)/ 180.0 * M_PI;
				matched = true;
				break;
			}
		}

		for (int fold = 0; fold < MANDELBOX_FOLDS; ++fold) {
			for (int axis = 0; axis < 3; ++axis) {
				for (int component = 0; component < 3; ++component) {
					sprintf(buf, "mandelbox_rotation_%s%d_%s", axis_names[axis], fold + 1, component_names[component]);
					if (!strcmp(str1, buf)) {
						params.fractal.mandelbox.doubles.rotation[fold][axis][component] = atof2(str2)/ 180.0 * M_PI;
						matched = true;
						break;
					}
				}
				if (matched) break;
			}
			if (matched) break;
		}

#ifdef CLSUPPORT
		for (int i = 0; i < 15; ++i)
		{
			sprintf(buf, "ocl_custom_par_%d", i);
			if (!strcmp(str1, buf))
			{
				params.fractal.doubles.customParameters[i] = atof2(str2);
				matched = true;
			}
		}
#endif

		if (!matched) {
			printf("Warning! Unknown parameter: %s %s\n", str1, str2);
			WriteLog("Warning! Unknown parameter:");
			WriteLog(str1);
			return false;
		}
	}
	return true;
}

void LoadSettingsPost(sParamRender &params)
{
	if (!paletteLoadedFromSettingsFile)
	{
		printf("Palette not found in settings file. Generating random palette\n");
		srand(params.coloring_seed);
		NewPalette(params.palette, params.doubles.colourSaturation);
	}

	lightsPlaced = 0;
}

bool LoadAppSettings(char *filename, sAppSettings &appParams)
{
	char str1[100];
	char str2[2000];

	FILE * fileSettings;
	fileSettings = fopen(filename, "r");

	int lineCounter = 0;

	if (fileSettings)
	{
		while (!feof(fileSettings))
		{
			lineCounter++;
			int c = fscanf(fileSettings, "%s", str1);
			if (c > 0)
			{
				c = fscanf(fileSettings, "%[ ]", str2);
				c = fscanf(fileSettings, "%[^;]", str2);

				     if (!strcmp(str1, "absolute_movement_enable")) appParams.absoluteMovementModeEnabled = atoi(str2);
				else if (!strcmp(str1, "zoom_by_mouse_click_enable")) appParams.zoomByMouseClickEnabled = atoi(str2);
				else if (!strcmp(str1, "go_close_to_surface")) appParams.goCloseToSurfaceEnabled = atoi(str2);
				else if (!strcmp(str1, "camera_movement_relative_step")) appParams.cameraMoveStepRelative = atof2(str2);
				else if (!strcmp(str1, "camera_movement_absolute_step")) appParams.cameraMoveStepAbsolute = atof2(str2);
				else if (!strcmp(str1, "camera_rotation_step")) appParams.rotationStep = atof2(str2);
				else if (!strcmp(str1, "mouse_close_up_ratio")) appParams.mouseCloseUpRatio = atof2(str2);

				else if (!strcmp(str1, "net_render_client_port")) appParams.netRenderClientPort = std::string(str2);
				else if (!strcmp(str1, "net_render_client_IP")) appParams.netRenderClientIP = std::string(str2);
				else if (!strcmp(str1, "net_render_server_port")) appParams.netRenderServerPort = std::string(str2);

#ifdef CLSUPPORT
				else if (!strcmp(str1, "openCL_use_CPU")) appParams.oclUseCPU = atoi(str2);
				else if (!strcmp(str1, "openCL_platform_index")) appParams.oclPlatformIndex = atoi(str2);
				else if (!strcmp(str1, "openCL_device_index")) appParams.oclDeviceIndex = atoi(str2);
				else if (!strcmp(str1, "openCL_engine")) appParams.oclEngineSelection = atoi(str2);
				else if (!strcmp(str1, "openCL_cycle_time")) appParams.oclCycleTime = atof2(str2);
				else if (!strcmp(str1, "openCL_memory_limit")) appParams.oclMemoryLimit = atoi(str2);

				else if (!strcmp(str1, "openCL_text_editor")) appParams.oclTextEditor = std::string(str2);
#endif
				else
				{
					printf("Warning! Unknown parameter: %s %s\n", str1, str2);
					WriteLog("Warning! Unknown parameter:");
					WriteLog(str1);
				}

				c = fscanf(fileSettings, "%[^\n]", str2);
			}
		}
		fclose(fileSettings);
		return true;
	}
	else
	{
		printf("Can't open application settings file: %s\n", filename);
		return false;
	}
}

void KeepOtherSettings(sParamRender *params)
{
	double *doublesParamRender = new double[sizeof(sParamRenderD)];
	double *doublesFractal = new double[sizeof(sFractalD)];
	double *doublesMandelbox = new double[sizeof(sFractalMandelboxD)];
	double *doublesIFS = new double[sizeof(sFractalIFSD)];

	memcpy(doublesParamRender,&params->doubles,sizeof(sParamRenderD));
	memcpy(doublesFractal,&params->fractal.doubles,sizeof(sFractalD));
	memcpy(doublesMandelbox,&params->fractal.mandelbox.doubles,sizeof(sFractalMandelboxD));
	memcpy(doublesIFS,&params->fractal.IFS.doubles,sizeof(sFractalIFSD));

	ReadInterface(params);

	memcpy(&params->doubles,doublesParamRender,sizeof(sParamRenderD));
	memcpy(&params->fractal.doubles,doublesFractal,sizeof(sFractalD));
	memcpy(&params->fractal.mandelbox.doubles,doublesMandelbox,sizeof(sFractalMandelboxD));
	memcpy(&params->fractal.IFS.doubles,doublesIFS,sizeof(sFractalIFSD));

	delete[] doublesParamRender;
	delete[] doublesFractal;
	delete[] doublesMandelbox;
	delete[] doublesIFS;
}
