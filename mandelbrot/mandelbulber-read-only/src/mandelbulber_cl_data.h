/*********************************************************
 /                   MANDELBULBER
 / data structures for data exchange between maon program
 / and OpenCL kernels
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

typedef enum
{
	clObjFractal = 0,
	clObjPlane = 1,
	clObjWater = 2,
	clObjSphere = 3,
	clObjSphereInv = 4,
	clObjBox = 5,
	clObjBoxInv = 6
}enumClObjectType;

typedef struct
{
	cl_float3 m1;
	cl_float3 m2;
	cl_float3 m3;
} matrix33;

typedef struct
{
	cl_float distance;
	cl_float distThresh;
	cl_float step;
	cl_float3 point;
	cl_int iters;
} sClStep;

typedef struct
{
	cl_float scale;
	cl_float foldingLimit;
	cl_float foldingValue;
	cl_float fixedRadius;
	cl_float minRadius;
	cl_float colorFactorX;
	cl_float colorFactorY;
	cl_float colorFactorZ;
	matrix33 mainRot;
	matrix33 rot[6];
	matrix33 rotinv[6];
	cl_int rotEnabled;
} sClMandelbox;

typedef struct
{
	cl_float3 position;
	cl_float3 colour;
	cl_float intensity;
	cl_int enabled;
} sClLight;

typedef struct
{
	cl_int absX;
	cl_int absY;
	cl_int absZ;
	cl_int enabled[9];
	cl_int mengerSpongeMode;
	matrix33 mainRot;
	matrix33 rot[9];
	cl_float rotationAlpha;
	cl_float rotationBeta;
	cl_float rotationGamma;
	cl_float scale;
	cl_float distance[9];
	cl_float alpha[9];
	cl_float beta[9];
	cl_float gamma[9];
	cl_float intensity[9];
	cl_float3 offset;
	cl_float3 direction[9];
	cl_float3 edge;
} sClIFS;

typedef struct
{
	cl_float3 planeCentre;
	cl_float3 planeNormal;
	cl_float3 boxCentre;
	cl_float3 boxSize;
	cl_float3 invertedBoxCentre;
	cl_float3 invertedBoxSize;
	cl_float3 sphereCentre;
	cl_float3 invertedSphereCentre;
	cl_float sphereRadius;
	cl_float invertedSphereRadius;
	cl_float waterHeight;
	cl_float waterAmplitude;
	cl_float waterLength;
	cl_float waterRotation;
	cl_float waterAnimSpeed;

	cl_float3 primitivePlaneColour;
	cl_float3 primitiveBoxColour;
	cl_float3 primitiveInvertedBoxColour;
	cl_float3 primitiveSphereColour;
	cl_float3 primitiveInvertedSphereColour;
	cl_float3 primitiveWaterColour;

	cl_float primitivePlaneReflect;
	cl_float primitiveBoxReflect;
	cl_float primitiveInvertedBoxReflect;
	cl_float primitiveSphereReflect;
	cl_float primitiveInvertedSphereReflect;
	cl_float primitiveWaterReflect;

	cl_int planeEnable;
	cl_int boxEnable;
	cl_int invertedBoxEnable;
	cl_int sphereEnable;
	cl_int invertedSphereEnable;
	cl_int waterEnable;
	cl_int waterIterations;
} sClPrimitives;

typedef struct
{
	cl_uint N;
	cl_int formula;
	cl_int juliaMode;
	cl_int constantDEThreshold;
	cl_int fakeLightsMinIter;
	cl_int fakeLightsMaxIter;
	cl_int customOCLFormulaDEMode;
	cl_int linearDEmode;
	cl_int limitsEnabled;
	cl_int interiorMode;
	cl_int frameNo;
	cl_float power;
	cl_float opacity;
	cl_float opacityTrim;
	cl_float fractalConstantFactor;
	cl_float custom[15];
	cl_float amin;  //fractal limits
	cl_float amax;
	cl_float bmin;
	cl_float bmax;
	cl_float cmin;
	cl_float cmax;
	cl_float deltaDEStep;
	cl_float3 julia;
	sClMandelbox mandelbox;
	sClIFS ifs;
	sClPrimitives primitives;
} sClFractal;

typedef struct
{
	cl_int width;
	cl_int height;
	cl_int AmbientOcclusionNoOfVectors;
	cl_int colouringEnabled;
	cl_int fastAmbientOcclusionEnabled;
	cl_int slowAmbientOcclusionEnabled;
	cl_int DOFEnabled;
	cl_int slowShading;
	cl_int penetratingLights;
	cl_int shadow;
	cl_int reflectionsMax;
	cl_int globalIlumQuality;
	cl_int fogEnabled;
	cl_int auxLightNumber;
	cl_int volumetricLightEnabled[5];
	cl_int volumetricLightEnabledAny;
	cl_int perspectiveType;
	cl_int fishEyeCut;
	cl_int iterFogEnabled;
	cl_int hdrEnabled;
	cl_int randomSeed;
	cl_int fakeLightsEnabled;
	cl_int texturedBackground;
	cl_int SSAOquality;
	cl_int DOFmethod;
	cl_float alpha;
	cl_float beta;
	cl_float gamma;
	cl_float zoom;
	cl_float persp;
	cl_float DEfactor;
	cl_float quality;
	cl_float mainLightAlfa;
	cl_float mainLightBeta;
	cl_float colouringSpeed;
	cl_float colouringOffset;
	cl_float ambientOcclusionIntensity;
	cl_float specularIntensity;
	cl_float shading;
	cl_float mainLightIntensity;
	cl_float glowIntensity;
	cl_float fogColour1Distance;
	cl_float fogColour2Distance;
	cl_float fogDistanceFactor;
	cl_float fogDensity;
	cl_float DOFFocus;
	cl_float DOFRadius;
	cl_float shadowConeAngle;
	cl_float viewDistanceMax;
	cl_float reflect;
	cl_float fastAoTune;
	cl_float fogVisibility;
	cl_float auxLightIntensity;
	cl_float volumetricLightIntensity[5];
	cl_float auxLightVisibility;
	cl_float imageBrightness;
	cl_float imageContrast;
	cl_float imageGamma;
	cl_float iterFogOpacity;
	cl_float iterFogOpacityTrim;
	cl_float fakeLightsIntensity;
	cl_float fakeLightsVisibility;
	cl_float fakeLightsVisibilitySize;

	cl_float3 vp;
	cl_float3 glowColour1;
	cl_float3 glowColour2;
	cl_float3 backgroundColour1;
	cl_float3 backgroundColour2;
	cl_float3 backgroundColour3;
	cl_float3 mainLightColour;
	cl_float3 fogColour;
	cl_float3 fogColour1;
	cl_float3 fogColour2;
	cl_float3 fogColour3;
	cl_float3 fakeLightsOrbitTrap;
} sClParams;

typedef struct
{
	cl_float R;
	cl_float G;
	cl_float B;
	cl_float zBuffer;
	cl_ushort opacity;
	cl_ushort alpha;
	cl_uchar colR;
	cl_uchar colG;
	cl_uchar colB;
} sClPixel;

typedef struct
{
	cl_float3 palette[256];
	cl_float3 vectorsAround[10000];
	cl_float3 vectorsAroundColours[10000];
	sClLight lights[10000];
} sClInBuff;

typedef struct
{
	sClParams params;
	sClFractal fractal;
} sClInConstants;

typedef struct
{
	cl_uint N;
	cl_int randomSeed;
	cl_float3 orbitTrap;
	cl_float distThresh;
	cl_float normalCalculationMode;
	enumClObjectType objectType;
} sClCalcParams;

typedef struct
{
	sClCalcParams *calcParam;
	cl_float3 point;
	cl_float3 startPoint;
	cl_float3 eyePoint;
	cl_float3 viewVector;
	cl_float3 normal;
	cl_float3 lightVect;
	cl_float dist_thresh;
	cl_float lastDist;
	cl_float delta;
	cl_float resolution;
	cl_int vectorsCount;
	//cTexture *envMappingTexture;
	cl_float depth;
	cl_int stepCount;
	enumClObjectType objectType;
} sClShaderInputData;

typedef struct
{
	cl_float3 start;
	cl_float3 point;
	cl_float3 viewVector;
	cl_float3 distance;
	cl_int stepCount;
	cl_float depth;
	cl_float lastDist;
	cl_float distThresh;
	enumClObjectType objectType;
	cl_float reflect;
	cl_int found;
} sClReflect;
