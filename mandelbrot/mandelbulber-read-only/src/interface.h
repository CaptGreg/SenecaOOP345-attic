/*********************************************************
 /                   MANDELBULBER
 / definition of user interface and functions
 / for parameters handling
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

#ifndef INTERFACE_H_
#define INTERFACE_H_

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "fractparams.h"
#include "cl_support.hpp"
#include "callbacks.h"
#include "shaders.h"
#include "morph.hpp"

#define MANDELBULBER_VERSION 1.211
#define MANDELBULBER_VERSION_STR "1.21-1"

#ifndef SHARED_DIR
	#ifndef WIN32
		#define SHARED_DIR "/usr/share/mandelbulber"
	#endif
#endif

extern char *sharedDir;

extern int x_mouse;
extern int y_mouse;
extern double last_z_mouse;
extern double smooth_last_z_mouse;

enum enumImageFormat
{
	imgFormatJPG = 0,
	imgFormatPNG = 1,
	imgFormatPNG16 = 2,
	imgFormatPNG16Alpha = 3
};

struct sMainWindow
{
	GtkWidget *window;
	GtkWidget *mainBox;
	GtkWidget *drawingArea;
	GtkWidget *scrolled_window;
	GtkWidget *comboImageScale;
	GtkWidget *comboMouseClickMode;
	GtkWidget *boxButtons;
	GtkWidget *labelImageScale;
	GtkWidget *labelMouseClickMode;

	GtkObject *hadjustment;
	GtkObject *vadjustment;

	int scrollbarSize;
	int lastWindowWidth;
	int lastWindowHeight;
};

struct sInterfaceIFS
{
	GtkWidget *editIFSx;
	GtkWidget *editIFSy;
	GtkWidget *editIFSz;
	GtkWidget *editIFSalfa;
	GtkWidget *editIFSbeta;
	GtkWidget *editIFSgamma;
	GtkWidget *editIFSdistance;
	GtkWidget *editIFSintensity;
	GtkWidget *checkIFSenabled;
};

struct sInterface
{
	GtkWidget *tabs;
	GtkWidget *tab_label_view;
	GtkWidget *tab_label_fractal;
	GtkWidget *tab_label_image;
	GtkWidget *tab_label_animation;
	GtkWidget *tab_label_shaders2;
	GtkWidget *tab_label_lights;
	GtkWidget *tab_label_IFS;
	GtkWidget *tab_label_about;
	GtkWidget *tab_label_shaders;
	GtkWidget *tab_label_hybrid;
	GtkWidget *tab_label_mandelbox;
	GtkWidget *tab_label_engine;
	GtkWidget *tabsPrimitives;
	GtkWidget *tab_label_primitivePlane;
	GtkWidget *tab_label_primitiveWater;
	GtkWidget *tab_label_primitiveBox;
	GtkWidget *tab_label_primitiveBoxInv;
	GtkWidget *tab_label_primitiveSphere;
	GtkWidget *tab_label_primitiveSphereInv;
	GtkWidget *tab_label_openCL;
	GtkWidget *tabsNetRender;
	GtkWidget *tab_label_server;
	GtkWidget *tab_label_client;
	GtkWidget *tabsOpenCL;
	GtkWidget *tab_label_openclEngine;
	GtkWidget *tab_label_openclCustom;

	GtkWidget *tab_box_view;
	GtkWidget *tab_box_fractal;
	GtkWidget *tab_box_image;
	GtkWidget *tab_box_animation;
	GtkWidget *tab_box_shaders2;
	GtkWidget *tab_box_lights;
	GtkWidget *tab_box_IFS;
	GtkWidget *tab_box_about;
	GtkWidget *tab_box_shaders;
	GtkWidget *tab_box_hybrid;
	GtkWidget *tab_box_mandelbox;
	GtkWidget *tab_box_engine;
	GtkWidget *tab_box_primitivePlane;
	GtkWidget *tab_box_primitiveWater;
	GtkWidget *tab_box_primitiveBox;
	GtkWidget *tab_box_primitiveBoxInv;
	GtkWidget *tab_box_primitiveSphere;
	GtkWidget *tab_box_primitiveSphereInv;
	GtkWidget *tab_box_openCL;
	GtkWidget *tab_box_server;
	GtkWidget *tab_box_client;
	GtkWidget *tab_box_openclEngine;
	GtkWidget *tab_box_openclCustom;

	GtkWidget *boxMain;
	GtkWidget *boxButtons;
	GtkWidget *boxView;
	GtkWidget *boxCoordinates;
	GtkWidget *boxAngle;
	GtkWidget *boxZoom;
	GtkWidget *boxArrows;
	GtkWidget *boxArrows2;
	GtkWidget *boxArrows3;
	GtkWidget *boxNavigation;
	GtkWidget *boxNavigationButtons;
	GtkWidget *boxNavigationZooming;
	GtkWidget *boxFractalFormula;
	GtkWidget *boxFractalFolding;
	GtkWidget *boxFractalRayMarching;
	GtkWidget *boxFractalFoldingIntPow;
	GtkWidget *boxFractalPower;
	GtkWidget *boxFractalSwitches;
	GtkWidget *boxLimits;
	GtkWidget *boxJulia;
	GtkWidget *boxQuality;
	GtkWidget *boxImage;
	GtkWidget *boxImageRes;
	GtkWidget *boxEffects;
	GtkWidget *boxBrightness;
	GtkWidget *boxShading;
	GtkWidget *boxShading2;
	GtkWidget *boxEffectsChecks;
	GtkWidget *boxEffectsChecks2;
	GtkWidget *boxEffectsColoring;
	GtkWidget *boxColors;
	GtkWidget *boxGlowColor;
	GtkWidget *boxFilenames;
	GtkWidget *boxLoadSave;
	GtkWidget *boxAnimation;
	GtkWidget *boxAnimationButtons;
	GtkWidget *boxAnimationEdits;
	GtkWidget *boxAnimationEdits2;
	GtkWidget *boxTgladFolding;
	GtkWidget *boxSphericalFolding;
	GtkWidget *boxSaveImage;
	GtkWidget *boxPostFog;
	GtkWidget *boxFogButtons;
	GtkWidget *boxFogSlider;
	GtkWidget *boxFogSlider2;
	GtkWidget *boxPostSSAO;
	GtkWidget *boxSSAOSlider;
	GtkWidget *boxSSAOButtons;
	GtkWidget *boxPostDOF;
	GtkWidget *boxDOFSlider1;
	GtkWidget *boxDOFSlider2;
	GtkWidget *boxDOFButtons;
	GtkWidget *boxLightBallance;
	GtkWidget *boxLightsParameters;
	GtkWidget *boxPredefinedLights;
	GtkWidget *boxLightBrightness;
	GtkWidget *boxLightDistribution;
	GtkWidget *boxLightDistribution2;
	GtkWidget *boxLightPre1;
	GtkWidget *boxLightPre2;
	GtkWidget *boxLightPre3;
	GtkWidget *boxLightPre4;
	GtkWidget *boxLightCommon;
	GtkWidget *boxMainLight;
	GtkWidget *boxMainLightPosition;
	GtkWidget *boxIFSMain;
	GtkWidget *boxIFSMainEdit;
	GtkWidget *boxIFSMainEdit2;
	GtkWidget *boxIFSParams;
	GtkWidget *boxIFSButtons;
	GtkWidget *boxIFSEdge;
	GtkWidget *boxKeyframeAnimation;
	GtkWidget *boxKeyframeAnimationButtons;
	GtkWidget *boxKeyframeAnimationButtons2;
	GtkWidget *boxKeyframeAnimationEdits;
	GtkWidget *boxBottomKeyframeAnimation;
	GtkWidget *boxPalette;
	GtkWidget *boxPaletteOffset;
	GtkWidget *boxImageSaving;
	GtkWidget *boxImageAutoSave;
	GtkWidget *boxHybrid;
	GtkWidget *boxStereoscopic;
	GtkWidget *boxStereoParams;
	GtkWidget *boxMandelboxMainParams;
	GtkWidget *boxMandelboxRotations;
	GtkWidget *boxMandelboxColoring;
	GtkWidget *boxMandelboxMainParams1;
	GtkWidget *boxMandelboxMainParams2;
	GtkWidget *boxMandelboxRotationMain;
	GtkWidget *boxMandelboxColor1;
	GtkWidget *boxMandelboxColor2;
	GtkWidget *boxMandelboxColor3;
	GtkWidget *boxMandelboxOffset;
	GtkWidget *boxViewDistance;
	GtkWidget *boxIFSDefaults;
	GtkWidget *boxVolumetricLight;
	GtkWidget *boxVolumetricLightGeneral;
	GtkWidget *boxVolumetricLightMain;
	GtkWidget *boxVolumetricLightAux;
	GtkWidget *boxMandelboxVary;
	GtkWidget *boxVolumetricFog;
	GtkWidget *boxPrimitives;
	GtkWidget *boxPrimitivePlane;
	GtkWidget *boxPrimitivePlane1;
	GtkWidget *boxPrimitivePlane2;
	GtkWidget *boxPrimitiveBox;
	GtkWidget *boxPrimitiveBox1;
	GtkWidget *boxPrimitiveBox2;
	GtkWidget *boxPrimitiveInvertedBox;
	GtkWidget *boxPrimitiveInvertedBox1;
	GtkWidget *boxPrimitiveInvertedBox2;
	GtkWidget *boxPrimitiveSphere;
	GtkWidget *boxPrimitiveSphere1;
	GtkWidget *boxPrimitiveSphere2;
	GtkWidget *boxPrimitiveInvertedSphere;
	GtkWidget *boxPrimitiveInvertedSphere1;
	GtkWidget *boxPrimitiveInvertedSphere2;
	GtkWidget *boxPrimitiveWater;
	GtkWidget *boxPrimitiveWater1;
	GtkWidget *boxPrimitiveWater2;
	GtkWidget *boxMeasure;
	GtkWidget *boxMeasure1;
	GtkWidget *boxOpenClSettings;
	GtkWidget *boxOpenClSwitches1;
	GtkWidget *boxOpenClInformation;
	GtkWidget *boxOpenClEngineSettingsV;
	GtkWidget *boxOpenClEngineSettingsH1;
	GtkWidget *boxOpenClEngineSettingsH2;
	GtkWidget *boxOpenClEngineSettingsH3;
	GtkWidget *boxOpenClEngineSettingsH4;
	GtkWidget *boxOpenClCustomV1;
	GtkWidget *boxOpenClCustomH11;
	GtkWidget *boxOpenClCustomH12;
	GtkWidget *boxOpenClCustomH13;
	GtkWidget *boxOpenClCustomV2;

	GtkWidget *boxNetRenderServerV;
	GtkWidget *boxNetRenderClientV;
	GtkWidget *boxNetRenderServerH1;
	GtkWidget *boxNetRenderClientH1;
	GtkWidget *boxFakeLightsV;
	GtkWidget *boxFakeLightsH1;
	GtkWidget *boxFakeLightsH2;
	GtkWidget *boxImageAdjustmentsV;
	GtkWidget *boxImageAdjustmentsH1;
	GtkWidget *boxShadersSurfaceV;
	GtkWidget *boxShadersSurfaceH1;
	GtkWidget *boxShadersSurfaceH2;
	GtkWidget *boxShadersSurfaceH3;
	GtkWidget *boxShadersSurfaceH4;
	GtkWidget *boxShadersVolumetricV;
	GtkWidget *boxShadersVolumetricH1;
	GtkWidget *boxShadersVolumetricH2;
	GtkWidget *boxShadersVolumetricH3;
	GtkWidget *boxShadersVolumetricH4;

	GtkWidget *tableLimits;
	GtkWidget *tableArrows;
	GtkWidget *tableArrows2;
	GtkWidget *tableIFSParams;
	GtkWidget *tableHybridParams;
	GtkWidget *tableMandelboxRotations;
	GtkWidget *tableOpenCLCustom;

	GtkWidget *frCoordinates;
	GtkWidget *fr3Dnavigator;
	GtkWidget *frLimits;
	GtkWidget *frImage;
	GtkWidget *frEffects;
	GtkWidget *frColors;
	GtkWidget *frFilenames;
	GtkWidget *frLoadSave;
	GtkWidget *frAnimation;
	GtkWidget *frAnimationFrames;
	GtkWidget *frPostFog;
	GtkWidget *frPostSSAO;
	GtkWidget *frPostDOF;
	GtkWidget *frLightBallance;
	GtkWidget *frLightsParameters;
	GtkWidget *frLightsCommon;
	GtkWidget *frPredefinedLights;
	GtkWidget *frMainLight;
	GtkWidget *frIFSMain;
	GtkWidget *frIFSParams;
	GtkWidget *frIFSDefaults;
	GtkWidget *frKeyframeAnimation;
	GtkWidget *frKeyframeAnimation2;
	GtkWidget *frPalette;
	GtkWidget *frImageSaving;
	GtkWidget *frHybrid;
	GtkWidget *frStereo;
	GtkWidget *frMandelboxMainParams;
	GtkWidget *frMandelboxRotations;
	GtkWidget *frMandelboxColoring;
	GtkWidget *frFractalFormula;
	GtkWidget *frFractalFolding;
	GtkWidget *frFractalRayMarching;
	GtkWidget *frFractalFoldingIntPow;
	GtkWidget *frVolumetricLight;
	GtkWidget *frMandelboxVary;
	GtkWidget *frPrimitives;
	GtkWidget *frPrimitivePlane;
	GtkWidget *frPrimitiveBox;
	GtkWidget *frPrimitiveInvertedBox;
	GtkWidget *frPrimitiveSphere;
	GtkWidget *frPrimitiveInvertedSphere;
	GtkWidget *frPrimitiveWater;
	GtkWidget *frMeasure;
	GtkWidget *frOpenClSettings;
	GtkWidget *frOpenClInformation;
	GtkWidget *frOpenClEngineSettings;
	GtkWidget *frOpenClCustomSelection;
	GtkWidget *frOpenClCustomParams;
	GtkWidget *frNetRender;
	GtkWidget *frFakeLights;
	GtkWidget *frImageAdjustments;
	GtkWidget *frShadersSurface;
	GtkWidget *frShadersVolumetric;

	GtkWidget *hSeparator1;
	GtkWidget *hSeparator2;
	GtkWidget *hSeparator3;
	GtkWidget *hSeparator4;
	GtkWidget *vSeparator1;

	GtkWidget *buRender;
	GtkWidget *buStop;
	GtkWidget *buColorGlow1;
	GtkWidget *buColorGlow2;
	GtkWidget *buColorBackgroud1;
	GtkWidget *buColorBackgroud2;
	GtkWidget *buColorBackgroud3;
	GtkWidget *buColorFog1;
	GtkWidget *buColorFog2;
	GtkWidget *buColorFog3;
	GtkWidget *buApplyImageAdjustments;
	GtkWidget *buSaveImage;
	GtkWidget *buSavePNG;
	GtkWidget *buSavePNG16;
	GtkWidget *buSavePNG16Alpha;
	GtkWidget *buFiles;
	GtkWidget *buLoadSettings;
	GtkWidget *buSaveSettings;
	GtkWidget *buUp;
	GtkWidget *buDown;
	GtkWidget *buLeft;
	GtkWidget *buRight;
	GtkWidget *buRotateLeft;
	GtkWidget *buRotateRight;
	GtkWidget *buMoveUp;
	GtkWidget *buMoveDown;
	GtkWidget *buMoveLeft;
	GtkWidget *buMoveRight;
	GtkWidget *buForward;
	GtkWidget *buBackward;
	GtkWidget *buInitNavigator;
	GtkWidget *buAnimationRecordTrack;
	GtkWidget *buAnimationContinueRecord;
	GtkWidget *buAnimationRenderTrack;
	GtkWidget *buColorFog;
	GtkWidget *buColorSSAO;
	GtkWidget *buUpdateSSAO;
	GtkWidget *buUpdateDOF;
	GtkWidget *buColorAuxLightPre1;
	GtkWidget *buColorAuxLightPre2;
	GtkWidget *buColorAuxLightPre3;
	GtkWidget *buColorAuxLightPre4;
	GtkWidget *buColorMainLight;
	GtkWidget *buDistributeLights;
	GtkWidget *buIFSNormalizeOffset;
	GtkWidget *buIFSNormalizeVectors;
	GtkWidget *buIFSReset;
	GtkWidget *buAnimationRecordKey;
	GtkWidget *buAnimationRenderFromKeys;
	GtkWidget *buUndo;
	GtkWidget *buRedo;
	GtkWidget *buBuddhabrot;
	GtkWidget *buRandomPalette;
	GtkWidget *buGetPaletteFromImage;
	GtkWidget *buTimeline;
	GtkWidget *buIFSDefaultDodeca;
	GtkWidget *buIFSDefaultIcosa;
	GtkWidget *buIFSDefaultOcta;
	GtkWidget *buIFSDefaultMengerSponge;
	GtkWidget *buAutoDEStep;
	GtkWidget *buAutoDEStepHQ;
	GtkWidget *buCopyToClipboard;
	GtkWidget *buGetFromClipboard;
	GtkWidget *buLoadExample;
	GtkWidget *buColorPrimitivePlane;
	GtkWidget *buColorPrimitiveBox;
	GtkWidget *buColorPrimitiveInvertedBox;
	GtkWidget *buColorPrimitiveSphere;
	GtkWidget *buColorPrimitiveInvertedSphere;
	GtkWidget *buColorPrimitiveWater;
	GtkWidget *buAutoFog;
	GtkWidget *buMeasureActivation;
	GtkWidget *buSaveAllImageLayers;
	GtkWidget *buOpenCLNewFormula;
	GtkWidget *buOpenCLDeleteFormula;
	GtkWidget *buOpenCLEditFormula;
	GtkWidget *buOpenCLEditFormulaInit;
	GtkWidget *buOpenCLRecompile;
	GtkWidget *buConvertPathToKeyframes;

	GtkWidget *edit_va;
	GtkWidget *edit_vb;
	GtkWidget *edit_vc;
	GtkWidget *edit_julia_a;
	GtkWidget *edit_julia_b;
	GtkWidget *edit_julia_c;
	GtkWidget *edit_amin;
	GtkWidget *edit_amax;
	GtkWidget *edit_bmin;
	GtkWidget *edit_bmax;
	GtkWidget *edit_cmin;
	GtkWidget *edit_cmax;
	GtkWidget *edit_alfa;
	GtkWidget *edit_beta;
	GtkWidget *edit_gammaAngle;
	GtkWidget *edit_zoom;
	GtkWidget *edit_persp;
	GtkWidget *edit_maxDepth;
	GtkWidget *edit_maxN;
	GtkWidget *edit_minN;
	GtkWidget *edit_power;
	GtkWidget *edit_DE_thresh;
	GtkWidget *edit_DE_stepFactor;
	GtkWidget *edit_imageWidth;
	GtkWidget *edit_imageHeight;
	GtkWidget *edit_shading;
	GtkWidget *edit_shadows;
	GtkWidget *edit_glow;
	GtkWidget *edit_ambient_occlusion;
	GtkWidget *edit_ambient;
	GtkWidget *edit_brightness;
	GtkWidget *edit_gamma;
	GtkWidget *edit_contrast;
	GtkWidget *edit_specular;
	GtkWidget *edit_reflect;
	GtkWidget *edit_AmbientOcclusionQuality;
	GtkWidget *edit_fileOutput;
	GtkWidget *edit_fileEnvMap;
	GtkWidget *edit_fileAmbient;
	GtkWidget *edit_fileBackground;
	GtkWidget *edit_fileKeyframes;
	GtkWidget *edit_step_forward;
	GtkWidget *edit_step_rotation;
	GtkWidget *edit_mouse_click_distance;
	GtkWidget *edit_animationDESpeed;
	GtkWidget *edit_color_seed;
	GtkWidget *edit_colour_saturation;
	GtkWidget *edit_tglad_folding_1;
	GtkWidget *edit_tglad_folding_2;
	GtkWidget *edit_spherical_folding_1;
	GtkWidget *edit_spherical_folding_2;
	GtkWidget *edit_color_speed;
	GtkWidget *edit_mainLightIntensity;
	GtkWidget *edit_auxLightIntensity;
	GtkWidget *edit_auxLightRandomSeed;
	GtkWidget *edit_auxLightNumber;
	GtkWidget *edit_auxLightMaxDist;
	GtkWidget *edit_auxLightDistributionRadius;
	GtkWidget *edit_auxLightPre1x;
	GtkWidget *edit_auxLightPre1y;
	GtkWidget *edit_auxLightPre1z;
	GtkWidget *edit_auxLightPre1intensity;
	GtkWidget *edit_auxLightPre2x;
	GtkWidget *edit_auxLightPre2y;
	GtkWidget *edit_auxLightPre2z;
	GtkWidget *edit_auxLightPre2intensity;
	GtkWidget *edit_auxLightPre3x;
	GtkWidget *edit_auxLightPre3y;
	GtkWidget *edit_auxLightPre3z;
	GtkWidget *edit_auxLightPre3intensity;
	GtkWidget *edit_auxLightPre4x;
	GtkWidget *edit_auxLightPre4y;
	GtkWidget *edit_auxLightPre4z;
	GtkWidget *edit_auxLightPre4intensity;
	GtkWidget *edit_auxLightVisibility;
	GtkWidget *edit_auxLightPlacementDistance;
	GtkWidget *edit_auxLightRandomCentreX;
	GtkWidget *edit_auxLightRandomCentreY;
	GtkWidget *edit_auxLightRandomCentreZ;
	GtkWidget *edit_mainLightAlfa;
	GtkWidget *edit_mainLightBeta;
	GtkWidget *edit_IFSAlfa;
	GtkWidget *edit_IFSBeta;
	GtkWidget *edit_IFSGamma;
	GtkWidget *edit_IFSScale;
	GtkWidget *edit_IFSOffsetX;
	GtkWidget *edit_IFSOffsetY;
	GtkWidget *edit_IFSOffsetZ;
	GtkWidget *edit_IFSEdgeX;
	GtkWidget *edit_IFSEdgeY;
	GtkWidget *edit_IFSEdgeZ;
	GtkWidget *edit_animationFramesPerKey;
	GtkWidget *edit_animationStartFrame;
	GtkWidget *edit_animationEndFrame;
	GtkWidget *edit_hybridIter[HYBRID_COUNT];
	GtkWidget *edit_hybridPower[HYBRID_COUNT];
	GtkWidget *edit_NavigatorAbsoluteDistance;
	GtkWidget *edit_stereoDistance;
	GtkWidget *edit_mandelboxScale;
	GtkWidget *edit_mandelboxFoldingLimit;
	GtkWidget *edit_mandelboxFoldingValue;
	GtkWidget *edit_mandelboxSpFoldingFixedRadius;
	GtkWidget *edit_mandelboxSpFoldingMinRadius;
	GtkWidget *edit_mandelboxRotation[MANDELBOX_FOLDS][3][3];
	GtkWidget *edit_mandelboxRotationMain[3];
	GtkWidget *edit_mandelboxColorFactorX;
	GtkWidget *edit_mandelboxColorFactorY;
	GtkWidget *edit_mandelboxColorFactorZ;
	GtkWidget *edit_mandelboxColorFactorR;
	GtkWidget *edit_mandelboxColorFactorSp1;
	GtkWidget *edit_mandelboxColorFactorSp2;
	GtkWidget *edit_mandelboxSharpness;
	GtkWidget *edit_mandelboxOffsetX;
	GtkWidget *edit_mandelboxOffsetY;
	GtkWidget *edit_mandelboxOffsetZ;
	GtkWidget *edit_mandelboxSolid;
	GtkWidget *edit_mandelboxMelt;
	GtkWidget *edit_viewMinDistance;
	GtkWidget *edit_viewMaxDistance;
	GtkWidget *edit_FractalConstantFactor;
	GtkWidget *edit_FoldingIntPowZFactor;
	GtkWidget *edit_FoldingIntPowFoldingFactor;
	GtkWidget *edit_roughness;
	GtkWidget *edit_volumetricLightMainIntensity;
	GtkWidget *edit_volumetricLightAux1Intensity;
	GtkWidget *edit_volumetricLightAux2Intensity;
	GtkWidget *edit_volumetricLightAux3Intensity;
	GtkWidget *edit_volumetricLightAux4Intensity;
	GtkWidget *edit_reflectionsMax;
	GtkWidget *edit_mandelboxVaryScale;
	GtkWidget *edit_mandelboxVaryRPower;
	GtkWidget *edit_mandelboxVaryFold;
	GtkWidget *edit_mandelboxVaryMinR;
	GtkWidget *edit_mandelboxVaryWAdd;
	GtkWidget *edit_cadd;
	GtkWidget *edit_volumetricFogDensity;
	GtkWidget *edit_volumetricFogColorDistance;
	GtkWidget *edit_volumetricFogColorDistance2;
	GtkWidget *edit_volumetricFogDistanceFact;
	GtkWidget *edit_fastAoTune;
	GtkWidget *edit_primitivePlaneCentreX;
	GtkWidget *edit_primitivePlaneCentreY;
	GtkWidget *edit_primitivePlaneCentreZ;
	GtkWidget *edit_primitivePlaneNormalX;
	GtkWidget *edit_primitivePlaneNormalY;
	GtkWidget *edit_primitivePlaneNormalZ;
	GtkWidget *edit_primitivePlaneReflect;
	GtkWidget *edit_primitiveBoxCentreX;
	GtkWidget *edit_primitiveBoxCentreY;
	GtkWidget *edit_primitiveBoxCentreZ;
	GtkWidget *edit_primitiveBoxSizeX;
	GtkWidget *edit_primitiveBoxSizeY;
	GtkWidget *edit_primitiveBoxSizeZ;
	GtkWidget *edit_primitiveBoxReflect;
	GtkWidget *edit_primitiveInvertedBoxCentreX;
	GtkWidget *edit_primitiveInvertedBoxCentreY;
	GtkWidget *edit_primitiveInvertedBoxCentreZ;
	GtkWidget *edit_primitiveInvertedBoxSizeX;
	GtkWidget *edit_primitiveInvertedBoxSizeY;
	GtkWidget *edit_primitiveInvertedBoxSizeZ;
	GtkWidget *edit_primitiveInvertedBoxReflect;
	GtkWidget *edit_primitiveSphereCentreX;
	GtkWidget *edit_primitiveSphereCentreY;
	GtkWidget *edit_primitiveSphereCentreZ;
	GtkWidget *edit_primitiveSphereRadius;
	GtkWidget *edit_primitiveSphereReflect;
	GtkWidget *edit_primitiveInvertedSphereCentreX;
	GtkWidget *edit_primitiveInvertedSphereCentreY;
	GtkWidget *edit_primitiveInvertedSphereCentreZ;
	GtkWidget *edit_primitiveInvertedSphereRadius;
	GtkWidget *edit_primitiveInvertedSphereReflect;
	GtkWidget *edit_primitiveWaterHeight;
	GtkWidget *edit_primitiveWaterAmplitude;
	GtkWidget *edit_primitiveWaterLength;
	GtkWidget *edit_primitiveWaterRotation;
	GtkWidget *edit_primitiveWaterIterations;
	GtkWidget *edit_primitiveWaterReflect;
	GtkWidget *edit_measureX;
	GtkWidget *edit_measureY;
	GtkWidget *edit_measureZ;
	GtkWidget *edit_tiles;
	GtkWidget *edit_OpenCLPixelsPerJob;
	GtkWidget *edit_OpenCLProcessingCycleTime;
	GtkWidget *edit_OpenCLMaxMem;
	GtkWidget *edit_OpenCLTextEditor;
	GtkWidget *edit_OpenCLDeltaDEStep;
	GtkWidget *edit_iterFogOpacityTrim;
	GtkWidget *edit_iterFogOpacity;
	GtkWidget *edit_netRenderServerPort;
	GtkWidget *edit_netRenderClientPort;
	GtkWidget *edit_netRenderClientName;
	GtkWidget *edit_fakeLightsOrbitTrapX;
	GtkWidget *edit_fakeLightsOrbitTrapY;
	GtkWidget *edit_fakeLightsOrbitTrapZ;
	GtkWidget *edit_fakeLightsIntensity;
	GtkWidget *edit_fakeLightsVisibility;
	GtkWidget *edit_fakeLightsVisibilitySize;
	GtkWidget *edit_fakeLightsMinIter;
	GtkWidget *edit_fakeLightsMaxIter;
	GtkWidget *edit_shadowConeAngle;
	GtkWidget *edit_customParameters[15];
	GtkWidget *edit_primitiveWaterAnimSpeed;

	GtkWidget *label_animationFrame;
	GtkWidget *label_animationSpeed;
	GtkWidget *label_animationDistance;
	GtkWidget *label_fog_visibility;
	GtkWidget *label_SSAO_quality;
	GtkWidget *label_DOF_focus;
	GtkWidget *label_DOF_radius;
	GtkWidget *label_about;
	GtkWidget *label_auxLightPre1;
	GtkWidget *label_auxLightPre2;
	GtkWidget *label_auxLightPre3;
	GtkWidget *label_auxLightPre4;
	GtkWidget *label_IFSx;
	GtkWidget *label_IFSy;
	GtkWidget *label_IFSz;
	GtkWidget *label_IFSalfa;
	GtkWidget *label_IFSbeta;
	GtkWidget *label_IFSgamma;
	GtkWidget *label_IFSdistance;
	GtkWidget *label_IFSintensity;
	GtkWidget *label_IFSenabled;
	GtkWidget *label_keyframeInfo;
	GtkWidget *label_paletteOffset;
	GtkWidget *label_HybridFormula[HYBRID_COUNT];
	GtkWidget *label_NavigatorEstimatedDistance;
	GtkWidget *label_DE_threshold;
	GtkWidget *label_measureDistance;
	GtkWidget *label_OpenClPlatformBy;
	GtkWidget *label_OpenClComputingUnits;
	GtkWidget *label_OpenClMaxWorkgroup;
	GtkWidget *label_OpenClMaxClock;
	GtkWidget *label_OpenClMemorySize;
	GtkWidget *label_OpenClWorkgroupSize;
	GtkWidget *label_OpenClStatus;
	GtkWidget *label_serverStatus;
	GtkWidget *label_clientStatus;
	GtkWidget *label_sliderFog;
	GtkWidget *label_sliderDOF;

	GtkWidget *comboFractType;
	GtkWidget *comboImageFormat;
	GtkWidget *comboHybridFormula[HYBRID_COUNT];
	GtkWidget *comboHybridDEMethod;
	GtkWidget *comboPerspectiveType;
	GtkWidget *comboImageProportion;
	GtkWidget *comboGeneralizedFoldBoxType;
	GtkWidget *comboOpenCLEngine;
	GtkWidget *comboOpenCLDeviceIndex;
	GtkWidget *comboOpenCLPlatformIndex;
	GtkWidget *comboOpenCLGPUCPU;
	GtkWidget *comboOpenCLCustomFormulas;
	GtkWidget *comboOpenCLDEMode;
	GtkWidget *comboOpenCLDOFMode;

	GtkWidget *progressBar;

	GtkWidget *checkAmbientOcclusion;
	GtkWidget *checkFastAmbientOcclusion;
	GtkWidget *checkShadow;
	GtkWidget *checkIterThresh;
	GtkWidget *checkJulia;
	GtkWidget *checkLimits;
	GtkWidget *checkSlowShading;
	GtkWidget *checkBitmapBackground;
	GtkWidget *checkBitmapBackgroundFulldome;
	GtkWidget *checkAnimationSpeedDE;
	GtkWidget *checkColoring;
	GtkWidget *checkTgladMode;
	GtkWidget *checkSphericalFoldingMode;
	GtkWidget *checkIFSFoldingMode;
	GtkWidget *checkFogEnabled;
	GtkWidget *checkSSAOEnabled;
	GtkWidget *checkDOFEnabled;
	GtkWidget *checkZoomClickEnable;
	GtkWidget *checkAuxLightPre1Enabled;
	GtkWidget *checkAuxLightPre2Enabled;
	GtkWidget *checkAuxLightPre3Enabled;
	GtkWidget *checkAuxLightPre4Enabled;
	GtkWidget *checkIFSAbsX;
	GtkWidget *checkIFSAbsY;
	GtkWidget *checkIFSAbsZ;
	GtkWidget *checkIFSMengerSponge;
	GtkWidget *checkAutoSaveImage;
	GtkWidget *checkHybridCyclic;
	GtkWidget *checkNavigatorAbsoluteDistance;
	GtkWidget *checkNavigatorGoToSurface;
	GtkWidget *checkStraightRotation;
	GtkWidget *checkStereoEnabled;
	GtkWidget *checkMandelboxRotationsEnable;
	GtkWidget *checkInteriorMode;
	GtkWidget *checkDELinearMode;
	GtkWidget *checkConstantDEThreshold;
	GtkWidget *checkVolumetricLightMainEnabled;
	GtkWidget *checkVolumetricLightAux1Enabled;
	GtkWidget *checkVolumetricLightAux2Enabled;
	GtkWidget *checkVolumetricLightAux3Enabled;
	GtkWidget *checkVolumetricLightAux4Enabled;
	GtkWidget *checkPenetratingLights;
	GtkWidget *checkRaytracedReflections;
	GtkWidget *checkPrimitivePlaneEnabled;
	GtkWidget *checkPrimitiveBoxEnabled;
	GtkWidget *checkPrimitiveInvertedBoxEnabled;
	GtkWidget *checkPrimitiveSphereEnabled;
	GtkWidget *checkPrimitiveInvertedSphereEnabled;
	GtkWidget *checkPrimitiveWaterEnabled;
	GtkWidget *checkOpenClEnable;
	GtkWidget *checkOpenClCustomEnable;
	GtkWidget *checkIterFogEnable;
	GtkWidget *checkNetRenderServerEnable;
	GtkWidget *checkNetRenderServerScan;
	GtkWidget *checkNetRenderClientEnable;
	GtkWidget *checkPrimitiveOnlyPlane;
	GtkWidget *checkFakeLightsEnabled;
	GtkWidget *checkHDR;

	GtkWidget *colorSelectionGlow1;
	GtkWidget *colorSelectionGlow2;

	GtkWidget *pixmap_up;
	GtkWidget *pixmap_down;
	GtkWidget *pixmap_left;
	GtkWidget *pixmap_right;
	GtkWidget *pixmap_rotate_left;
	GtkWidget *pixmap_rotate_right;
	GtkWidget *pixmap_move_up;
	GtkWidget *pixmap_move_down;
	GtkWidget *pixmap_move_left;
	GtkWidget *pixmap_move_right;

	GtkWidget *sliderFogDepth;
	GtkWidget *sliderSSAOQuality;
	GtkWidget *sliderDOFFocus;
	GtkWidget *sliderDOFRadius;
	GtkWidget *sliderPaletteOffset;

	GtkObject *adjustmentFogDepth;
	GtkObject *adjustmentSSAOQuality;
	GtkObject *adjustmentDOFFocus;
	GtkObject *adjustmentDOFRadius;
	GtkObject *adjustmentPaletteOffset;

	sInterfaceIFS IFSParams[IFS_VECTOR_COUNT];
};

struct sDialogFiles
{
	GtkWidget *window_files;
	GtkWidget *box_main;
	GtkWidget *box_buttons;

	GtkWidget *boxFilePath;
	GtkWidget *boxFileBackground;
	GtkWidget *boxFileImage;
	GtkWidget *boxFileEnvMap;
	GtkWidget *boxFileAmbient;
	GtkWidget *boxFileKeyframes;

	GtkWidget *table_edits;
	GtkWidget *label_destination;
	GtkWidget *label_envmap;
	GtkWidget *label_lightmap;
	GtkWidget *label_background;
	GtkWidget *label_path;
	GtkWidget *label_keyframes;

	GtkWidget *edit_destination;
	GtkWidget *edit_envmap;
	GtkWidget *edit_lightmap;
	GtkWidget *edit_background;
	GtkWidget *edit_path;
	GtkWidget *edit_keyframes;
	GtkWidget *bu_ok;
	GtkWidget *bu_cancel;
	GtkWidget *bu_select_destination;
	GtkWidget *bu_select_envmap;
	GtkWidget *bu_select_lightmap;
	GtkWidget *bu_select_background;
	GtkWidget *bu_select_path;
	GtkWidget *bu_select_keyframes;
};

struct sTimelineInterface
{
	GtkWidget **darea;
	GtkWidget **label;
	GtkWidget *table;
	GtkAdjustment *windowHadjustment;
	GtkAdjustment *windowVadjustment;
	GtkWidget *scrolledWindow;
	GtkWidget *layoutContainer;
	GtkWidget *boxMain;
	GtkWidget *boxTable;
	GtkWidget *boxButtons;

	GtkWidget *buAnimationRecordKey2;
	GtkWidget *buAnimationInsertKeyframe;
	GtkWidget *buAnimationDeleteKeyframe;
	GtkWidget *buNextKeyframe;
	GtkWidget *buPreviousKeyframe;
	GtkWidget *buRefresh;

	GtkWidget *editAnimationKeyNumber;
};

struct sInterface_data
{
	bool disableInitRefresh;

	double imageScale;

	bool animMode;
	bool recordMode;
	bool continueRecord;
	bool playMode;
	bool keyframeMode;

	enumImageFormat imageFormat;

	char file_destination[1000];
	char file_envmap[1000];
	char file_background[1000];
	char file_lightmap[1000];
	char file_path[1000];
	char file_keyframes[1000];

	int franeNo;
	int tileCount;

	sRGB palette[256];
};

struct sNoGUIdata
{
	char settingsFile[1000];
	bool animMode;
	bool playMode;
	bool keyframeMode;
	bool netrenderMode;
	int startFrame;
	int endFrame;
	char netRenderIPString[20];
	char netRenderPortString[6];
	enumImageFormat imageFormat;
	sParamRender fractparams;
	std::vector<const char *> overrideStrings;
};

struct sAppSettings
{
#ifdef CLSUPPORT
	bool oclUseCPU;
	int oclDeviceIndex;
	int oclPlatformIndex;
	int oclEngineSelection;
	double oclCycleTime;
	double oclMemoryLimit;
	std::string oclTextEditor;
#endif
	bool absoluteMovementModeEnabled;
	bool zoomByMouseClickEnabled;
	bool goCloseToSurfaceEnabled;
	double cameraMoveStepRelative;
	double cameraMoveStepAbsolute;
	double rotationStep;
	double mouseCloseUpRatio;
  std::string netRenderClientPort;
	std::string netRenderClientIP;
	std::string netRenderServerPort;
};

//Global variables

extern sMainWindow renderWindow;
extern sInterface Interface;
extern sTimelineInterface timelineInterface;
extern sInterface_data Interface_data;
extern sNoGUIdata noGUIdata;
extern GtkWidget *window_histogram, *window_interface;
extern GtkWidget *darea2, *darea3;
extern GtkWidget *dareaPalette;
extern int x_mouse;
extern int y_mouse;
extern bool programClosed;
extern bool interfaceCreated;
extern bool paletteViewCreated;

extern char lastFilenameImage[1000];
extern char lastFilenameSettings[1000];
extern char lastFilenamePalette[1000];

extern GtkWidget *timeLineWindow;

extern GtkClipboard *clipboard;

//functions
char* DoubleToString(double value);
char* IntToString(int value);
GtkWidget* CreateEdit(const gchar *text, const gchar *txt_label, int size, GtkWidget *edit);
GtkWidget* CreateWidgetWithLabel(const gchar *txt_label, GtkWidget *widget);
void CreateInterface(sParamRender *default_settings);
void ReadInterface(sParamRender *params);
void WriteInterface(sParamRender *params);
void CreateFilesDialog(GtkWidget *widget, gpointer data);
void CreateTooltips(void);
bool ReadComandlineParams(int argc, char *argv[]);
double atofData(const gchar* text);
void AddComboTextsFractalFormula(GtkComboBox *combo);
enumFractalFormula FormulaNumberGUI2Data(int formula);
int FormulaNumberData2GUI(enumFractalFormula formula);
void Params2InterfaceData(sParamRender *source);
void InterfaceData2Params(sParamRender *dest);
void CheckPrameters(sParamRender *params);
sRGB GdkColor2sRGB(GdkColor color);
GdkColor sRGB2GdkColor(sRGB color);
sRGB sRGBDiv256(sRGB color);
void ReadInterfaceAppSettings(sAppSettings *appParams);
void WriteInterfaceAppSettings(sAppSettings *appParams);
#ifdef CLSUPPORT
void Params2Cl(const sParamRender *params, sClInBuff *clInBuff, sClInConstants *clConstantsBuff);
matrix33 RotMatrix2matrix33(CRotationMatrix rot);
cl_float4 CVector2float3(CVector3 vect);
cl_float4 sRGB2float3(sRGB colour, double factor);
#endif
#endif /* INTERFACE_H_ */
