/*********************************************************
 /                   MANDELBULBER
 / classes for OpenCL support
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

#include <string>
#include "interface.h"
#include "settings.h"
#include "cl_support.hpp"

#include <dirent.h>
#include <sstream>
#include <sys/stat.h>
#include <glib.h>

#include "files.h"
#include "common_math.h"

#ifdef WIN32
#include <process.h>
#endif

CclSupport *clSupport;

#ifdef CLSUPPORT

struct sClParamsSSAO
{
	cl_int width;
	cl_int height;
	cl_int quality;
	cl_float fov;
	cl_float intensity;
};

struct sClParamsDOF
{
	cl_int width;
	cl_int height;
	cl_float focus;
	cl_float radius;
};


CclSupport::CclSupport(void)
{
	enabled = false;
	ready = false;
	width = 800;
	height = 600;
	steps = 10;
	recompileRequest = true;
	lastFormula = trig_optim;
	lastEngineNumber = 0;
	lastStepSize = 0;
	inBuffer1 = new sClInBuff;
	constantsBuffer1 = new sClInConstants;
	auxReflectBuffer = NULL;
	buffSize = 0;
	context = NULL;
	kernel = NULL;
	outCL = NULL;
	rgbbuff = NULL;
	inCLBuffer1 = NULL;
	reflectBuffer = NULL;
	inCLConstBuffer1 = NULL;
	program = NULL;
	queue = NULL;
	reflectBufferSize = 0;
	maxClockFrequency = 0;
	maxAllocMemSize = 0;
	platformIndex = 0;
	memoryLimitByUser = 0;
	useCPU = false;
	deviceIndex = 0;
	workGroupSize = 0;
	numberOfComputeUnits = 0;
	stepSize = 0;
	memorySize = 0;
	maxWorkItemDimmensions = 0;
	maxConstantBufferSize = 0;
	memset(&lastParams, 0, sizeof(lastParams));
	memset(&lastFractal, 0, sizeof(lastFractal));
	isNVIDIA = false;
	customFormulas = NULL;
	backgroundImage2D = NULL;
	backgroundImage2DHeight = 10;
	backgroundImage2DWidth = 10;
	backgroungImageBuffer = new cl_uchar4[10*10];
	backgroundTextureSource = NULL;

	programSSAO = NULL;
	kernelSSAO = NULL;
	queueSSAO = NULL;
	SSAOprepared = false;

	programDOF = NULL;
	kernelDOF = NULL;
	queueDOF = NULL;
	DOFprepared = false;
}

bool CclSupport::checkErr(cl_int err, const char * name)
{
	if (err != CL_SUCCESS)
	{
		std::stringstream errorMessageStream;
		errorMessageStream << "ERROR: " << name << " (" << err << ")";
		std::string errorMessage;
		errorMessage = errorMessageStream.str();
		std::cerr << errorMessage << std::endl;
		GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_interface), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, errorMessage.c_str());
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		ready = false;
		enabled = false;
		recompileRequest = true;

#ifdef WIN32
		SetCurrentDirectory(data_directory);
#else
		chdir(data_directory);
#endif
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Interface.checkOpenClEnable), false);
		return false;
	}
	else
		return true;
}

void CclSupport::InitDevice(void)
{
	cl_int err = 0;

#ifdef WIN32
	const std::wstring opencldll( L"OpenCL.dll" );
	err = clewInit(opencldll.c_str());
	std::cout << clewErrorString(err) << std::endl;
#endif

	cl::Platform::get(&platformList);
	if(!checkErr(platformList.size() != 0 ? CL_SUCCESS : -1, "cl::Platform::get")) return;
	std::cout << "OpenCL Platform number is: " << platformList.size() << std::endl;

	useCPU = gtk_combo_box_get_active(GTK_COMBO_BOX(Interface.comboOpenCLGPUCPU));
	deviceIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(Interface.comboOpenCLDeviceIndex));
	platformIndex = gtk_combo_box_get_active(GTK_COMBO_BOX(Interface.comboOpenCLPlatformIndex));

	if(platformIndex > platformList.size()-1)
	{
		platformIndex = platformList.size()-1;
		gtk_combo_box_set_active(GTK_COMBO_BOX(Interface.comboOpenCLPlatformIndex), platformIndex);
	}

	platformList[platformIndex].getInfo((cl_platform_info) CL_PLATFORM_VENDOR, &platformVendor);
	std::cout << "OpenCL Platform is by: " << platformVendor << "\n";
	cl_context_properties cprops[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties) (platformList[platformIndex])(), 0 };
	if(platformVendor.find("NVIDIA") != std::string::npos)
	{
		isNVIDIA = true;
		printf("nVidia OpenCL library is detected\n");
	}
	if(platformVendor.find("Advanced") != std::string::npos)
	{
		isNVIDIA = false;
		printf("AMD OpenCL library is detected\n");
	}

	if(useCPU)
	{
		context = new cl::Context(CL_DEVICE_TYPE_CPU, cprops, NULL, NULL, &err);
	}
	else
	{
		context = new cl::Context(CL_DEVICE_TYPE_GPU, cprops, NULL, NULL, &err);
	}
	if(!checkErr(err, "Context::Context()")) return;
	printf("OpenCL contexts created\n");

	devices = context->getInfo<CL_CONTEXT_DEVICES>();
	if(deviceIndex > devices.size()-1)
	{
		deviceIndex = devices.size()-1;
		gtk_combo_box_set_active(GTK_COMBO_BOX(Interface.comboOpenCLDeviceIndex), deviceIndex);
	}

	if(!checkErr(devices.size() > 0 ? CL_SUCCESS : -1, "devices.size() > 0")) return;

	devices[deviceIndex].getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &numberOfComputeUnits);;
	printf("OpenCL Number of compute units: %d\n", numberOfComputeUnits);

	devices[deviceIndex].getInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, &maxWorkItemDimmensions);
	printf("OpenCL Max work item dimmensions: %d\n", maxWorkItemDimmensions);

	devices[deviceIndex].getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &maxMaxWorkGroupSize);
	printf("OpenCL Max work group size: %d\n", maxMaxWorkGroupSize[0]);

	devices[deviceIndex].getInfo(CL_DEVICE_MAX_CLOCK_FREQUENCY, &maxClockFrequency);
	printf("OpenCL Max clock frequency  %d MHz\n", maxClockFrequency);

	devices[deviceIndex].getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &memorySize);
	printf("OpenCL Memory size  %ld MB\n", memorySize/1024/1024);

	devices[deviceIndex].getInfo(CL_DEVICE_MAX_MEM_ALLOC_SIZE, &maxAllocMemSize);
	printf("OpenCL Max size of memory object allocation %ld MB\n", maxAllocMemSize/1024/1024);

	devices[deviceIndex].getInfo(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, &maxConstantBufferSize);
	printf("OpenCL Max constant buffer size  %ld kB\n", maxConstantBufferSize/1024);

	printf("OpenCL Constant buffer used  %ld kB\n", sizeof(sClInConstants)/1024);

	char text[1000];
	sprintf(text,"OpenCL platform by: %s", platformVendor.c_str());
	gtk_label_set_text(GTK_LABEL(Interface.label_OpenClPlatformBy), text);
	sprintf(text,"GPU frequency: %d MHz", maxClockFrequency);
	gtk_label_set_text(GTK_LABEL(Interface.label_OpenClMaxClock), text);
	sprintf(text,"GPU memory: %ld MB", memorySize/1024/1024);
	gtk_label_set_text(GTK_LABEL(Interface.label_OpenClMemorySize), text);
	sprintf(text,"Number of computing units: %d", numberOfComputeUnits);
	gtk_label_set_text(GTK_LABEL(Interface.label_OpenClComputingUnits), text);

	clDir = std::string(sharedDir) + "cl/";
}

void CclSupport::InitFractal(void)
{
	cl_int err;
	char text[1000];

	ready = false;

	char progressText[1000];
	sprintf(progressText, "OpenCL - initialization");
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), progressText);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(Interface.progressBar), 0.0);
	while (gtk_events_pending())
		gtk_main_iteration();

	if(!customFormulas)
	{
		customFormulas = new CCustomFormulas(data_directory);
	}


	memoryLimitByUser = atoi(gtk_entry_get_text(GTK_ENTRY(Interface.edit_OpenCLMaxMem))) * 1024 * 1024;

	std::string strFormula = "mandelbulb";

	if(lastFormula == trig_optim) strFormula = "mandelbulb";
	if(lastFormula == trig_DE) strFormula = "mandelbulb2";
	if(lastFormula == tglad)
	{
		if(lastFractal.mandelbox.rotEnabled)
			strFormula = "mandelbox_full";
		else
			strFormula = "mandelbox";
	}
	if(lastFormula == menger_sponge) strFormula = "mengersponge";
	if(lastFormula == hypercomplex) strFormula = "hypercomplex";
	if(lastFormula == quaternion) strFormula = "quaternion";
	if(lastFormula == kaleidoscopic) strFormula = "kaleidoscopic";
	if(lastFormula == xenodreambuie) strFormula = "xenodreambuie";

	std::string strFileEngine = clDir;
	int engineNumber = gtk_combo_box_get_active(GTK_COMBO_BOX(Interface.comboOpenCLEngine));
	if		 (engineNumber == 0) 	strFileEngine += "cl_engine_fast.cl";
	else if(engineNumber == 1)	strFileEngine += "cl_engine.cl";
	else if(engineNumber == 2)	strFileEngine += "cl_engine_full.cl";
	else if(engineNumber == 3)	strFileEngine += "cl_engine_noDE.cl";

	std::ifstream fileEngine(strFileEngine.c_str());
	if(!checkErr(fileEngine.is_open() ? CL_SUCCESS : -1, ("Can't open file:" + strFileEngine).c_str())) return;

	std::string strFilePrimitives = clDir + "cl_primitives.cl";
	std::ifstream filePrimitives(strFilePrimitives.c_str());
	if(!checkErr(filePrimitives.is_open() ? CL_SUCCESS : -1, ("Can't open file:" + strFilePrimitives).c_str())) return;

	std::string strFileDistance = clDir;
	if(lastFormula == ocl_custom)
	{
		switch ((enumOCLDEMode) lastFractal.customOCLFormulaDEMode)
		{
			case calculated:
			{
				strFileDistance += "cl_distance.cl";
				break;
			}
			case deltaDE:
			{
				strFileDistance += "cl_distance_deltaDE.cl";
				break;
			}
			case noDE:
			{
				strFileDistance += "cl_distance_noDE.cl";
				break;
			}
		}
	}
	else
	{
		if(lastFormula == xenodreambuie || lastFormula == hypercomplex)
			strFileDistance += "cl_distance_deltaDE.cl";
		else
			strFileDistance += "cl_distance.cl";
	}
	std::ifstream fileDistance(strFileDistance.c_str());
	if(!checkErr(fileDistance.is_open() ? CL_SUCCESS : -1, ("Can't open file:" + strFileDistance).c_str())) return;

	std::string strFileFormulaBegin;
	if(lastFractal.juliaMode) strFileFormulaBegin  = clDir + "cl_formulaBegin" + "Julia" + ".cl";
	else  strFileFormulaBegin = clDir + "cl_formulaBegin" + ".cl";
	std::ifstream fileFormulaBegin(strFileFormulaBegin.c_str());
	if(!checkErr(fileFormulaBegin.is_open() ? CL_SUCCESS : -1, ("Can't open file:" + strFileFormulaBegin).c_str())) return;

	std::string strFileFormulaInit;
	std::string strFileFormula;
	if(lastFormula == ocl_custom)
	{
		customFormulas->GetActual(&strFormula, &strFileFormula, &strFileFormulaInit);

	}
	else
	{
		strFileFormulaInit = clDir + "cl_" + strFormula + "Init.cl";
		strFileFormula = clDir + "cl_" + strFormula + ".cl";
	}
	std::ifstream fileFormulaInit(strFileFormulaInit.c_str());
	if(!checkErr(fileFormulaInit.is_open() ? CL_SUCCESS : -1, ("Can't open file:" + strFileFormulaInit).c_str())) return;

	std::string strFileFormulaFor = clDir + "cl_formulaFor.cl";
	std::ifstream fileFormulaFor(strFileFormulaFor.c_str());
	if(!checkErr(fileFormulaFor.is_open() ? CL_SUCCESS : -1, ("Can't open file:" + strFileFormulaFor).c_str())) return;

	std::ifstream fileFormula(strFileFormula.c_str());
	if(!checkErr(fileFormula.is_open() ? CL_SUCCESS : -1, ("Can't open file:" + strFileFormula).c_str())) return;

	std::string strFileFormulaEnd = clDir + "cl_formulaEnd.cl";
	std::ifstream fileFormulaEnd(strFileFormulaEnd.c_str());
	if(!checkErr(fileFormulaEnd.is_open() ? CL_SUCCESS : -1, ("Can't open file:" + strFileFormulaEnd).c_str())) return;

	std::string progPathHeader("#define INCLUDE_PATH_CL_DATA \"");
#ifdef WIN32
	progPathHeader += std::string(sharedDir) + "cl\\mandelbulber_cl_data.h\"";
#else
	progPathHeader += std::string(sharedDir) + "cl/mandelbulber_cl_data.h\"";
#endif
	printf("OpenCL progHeader: %s\n", progPathHeader.c_str());

	std::string progEngine(std::istreambuf_iterator<char>(fileEngine), (std::istreambuf_iterator<char>()));
	std::string progPrimitives(std::istreambuf_iterator<char>(filePrimitives), (std::istreambuf_iterator<char>()));
	std::string progDistance(std::istreambuf_iterator<char>(fileDistance), (std::istreambuf_iterator<char>()));
	std::string progFormulaBegin(std::istreambuf_iterator<char>(fileFormulaBegin), (std::istreambuf_iterator<char>()));
	std::string progFormulaInit(std::istreambuf_iterator<char>(fileFormulaInit), (std::istreambuf_iterator<char>()));
	std::string progFormulaFor(std::istreambuf_iterator<char>(fileFormulaFor), (std::istreambuf_iterator<char>()));
	std::string progFormula(std::istreambuf_iterator<char>(fileFormula), (std::istreambuf_iterator<char>()));
	std::string progFormulaEnd(std::istreambuf_iterator<char>(fileFormulaEnd), (std::istreambuf_iterator<char>()));

	cl::Program::Sources sources;
	sources.push_back(std::make_pair(progPathHeader.c_str(), progPathHeader.length()));
	sources.push_back(std::make_pair(progEngine.c_str(), progEngine.length()));
	sources.push_back(std::make_pair(progPrimitives.c_str(), progPrimitives.length()));
	sources.push_back(std::make_pair(progDistance.c_str(), progDistance.length()));
	sources.push_back(std::make_pair(progFormulaBegin.c_str(), progFormulaBegin.length()));
	sources.push_back(std::make_pair(progFormulaInit.c_str(), progFormulaInit.length()));
	sources.push_back(std::make_pair(progFormulaFor.c_str(), progFormulaFor.length()));
	sources.push_back(std::make_pair(progFormula.c_str(), progFormula.length()));
	sources.push_back(std::make_pair(progFormulaEnd.c_str(), progFormulaEnd.length()+1));
	printf("OpenCL Number of loaded sources %ld\n", sources.size());

	program = new cl::Program(*context, sources, &err);
	if(!checkErr(err, "Program()")) return;
	//program->getInfo(CL_PROGRAM_SOURCE, )
	//std::cout << "Program source:\t" << program->getInfo<CL_PROGRAM_SOURCE>() << std::endl;

#ifdef WIN32
	SetCurrentDirectory(clDir.c_str());
#else
	chdir(clDir.c_str());
#endif

	std::string buildParams;
	buildParams = "-w -cl-single-precision-constant -cl-denorms-are-zero ";

	if(lastParams.DOFEnabled && lastParams.DOFmethod == 1) buildParams += "-D_DOFEnabled ";
	if(lastParams.slowAmbientOcclusionEnabled) buildParams += "-D_SlowAOEnabled ";
	if(lastParams.fakeLightsEnabled) buildParams += "-D_orbitTrapsEnabled ";
	if(lastParams.auxLightNumber > 0) buildParams += "-D_AuxLightsEnabled ";
	if(lastParams.texturedBackground) buildParams += "-D_texturedBackground ";
	if(lastFractal.limitsEnabled) buildParams += "-D_LimitsEnabled ";
	if(lastFractal.primitives.planeEnable) buildParams += "-D_primitivePlane ";
	if(lastFractal.primitives.boxEnable) buildParams += "-D_primitiveBox ";
	if(lastFractal.primitives.invertedBoxEnable) buildParams += "-D_primitiveInvertedBox ";
	if(lastFractal.primitives.sphereEnable) buildParams += "-D_primitiveSphere ";
	if(lastFractal.primitives.invertedSphereEnable) buildParams += "-D_primitiveInvertedSphere ";
	if(lastFractal.primitives.waterEnable) buildParams += "-D_primitiveWater ";
	printf("OpenCL build params:%s\n", buildParams.c_str());
	err = program->build(devices, buildParams.c_str());

	std::stringstream errorMessageStream;
	errorMessageStream << "OpenCL Build log:\t" << program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[deviceIndex]) << std::endl;
	std::string buildLogText;
	buildLogText = errorMessageStream.str();
	std::cout << buildLogText;

	if(!checkErr(err, "Program::build()"))
	{
		GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window_interface), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK,
				"Program build log:");
		GtkWidget *messageArea = gtk_message_dialog_get_message_area(GTK_MESSAGE_DIALOG(dialog));

		GtkTextBuffer *textBuffer = gtk_text_buffer_new(NULL);
		gtk_text_buffer_set_text(textBuffer, buildLogText.c_str(), buildLogText.length());

		GtkWidget *textView = gtk_text_view_new_with_buffer(textBuffer);
		gtk_box_pack_start(GTK_BOX(messageArea), textView, false, false, 1);
		gtk_widget_show(textView);

		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}

	printf("OpenCL program built done\n");

#ifdef WIN32
	SetCurrentDirectory(data_directory);
#else
  chdir(data_directory);
#endif

	kernel = new cl::Kernel(*program, "fractal3D", &err);
	if(!checkErr(err, "Kernel::Kernel()")) return;
	printf("OpenCL kernel opened\n");

	kernel->getWorkGroupInfo(devices[deviceIndex], CL_KERNEL_WORK_GROUP_SIZE, &workGroupSize);
	printf("OpenCL workgroup size: %ld\n", workGroupSize);

	int pixelsPerJob =  workGroupSize * numberOfComputeUnits;
	steps = height * width / pixelsPerJob + 1;
	stepSize = (width * height / steps / workGroupSize + 1) * workGroupSize;
	printf("OpenCL Job size: %d\n", stepSize);
	buffSize = stepSize * sizeof(sClPixel);
	rgbbuff = new sClPixel[buffSize];

	reflectBufferSize = sizeof(sClReflect) * 10 * stepSize;
	reflectBuffer = new sClReflect[reflectBufferSize];
	auxReflectBuffer = new cl::Buffer(*context, CL_MEM_READ_WRITE, reflectBufferSize, NULL, &err);
	if(!checkErr(err, "Buffer::Buffer(*context, CL_MEM_READ_WRITE, reflectBufferSize, NULL, &err)")) return;

	inCLConstBuffer1 = new cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(sClInConstants), constantsBuffer1, &err);
	if(!checkErr(err, "Buffer::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(sClInConstants), constantsBuffer1, &err)")) return;

	inCLBuffer1 = new cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(sClInBuff), inBuffer1, &err);
	if(!checkErr(err, "Buffer::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(sClInBuff), inBuffer1, &err)")) return;

	outCL = new cl::Buffer(*context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, buffSize,rgbbuff,&err);
	if(!checkErr(err, "*context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, buffSize, rgbbuff, &err")) return;

	printf("OpenCL buffers created\n");

	queue = new cl::CommandQueue(*context, devices[deviceIndex], 0, &err);
	if(!checkErr(err, "CommandQueue::CommandQueue()"))return;
	printf("OpenCL command queue prepared\n");

	sprintf(progressText, "OpenCL - ready");
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), progressText);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(Interface.progressBar), 1.0);

	sprintf(text,"Max workgroup size: %d", maxMaxWorkGroupSize[0]);
	gtk_label_set_text(GTK_LABEL(Interface.label_OpenClMaxWorkgroup), text);
	sprintf(text,"Actual workgroup size: %ld", workGroupSize);
	gtk_label_set_text(GTK_LABEL(Interface.label_OpenClWorkgroupSize), text);

	ready = true;
}

void CclSupport::SetParams(sClInBuff *inBuff, sClInConstants *inConstants, enumFractalFormula formula)
{
	if(inConstants->fractal.juliaMode != lastFractal.juliaMode) recompileRequest = true;
	if(formula != lastFormula) recompileRequest = true;
	if(inConstants->params.DOFEnabled != lastParams.DOFEnabled) recompileRequest = true;
	if(inConstants->params.slowAmbientOcclusionEnabled != lastParams.slowAmbientOcclusionEnabled) recompileRequest = true;
	if(inConstants->params.auxLightNumber != lastParams.auxLightNumber) recompileRequest = true;
	if(inConstants->fractal.mandelbox.rotEnabled != lastFractal.mandelbox.rotEnabled) recompileRequest = true;
	if(inConstants->params.fakeLightsEnabled != lastParams.fakeLightsEnabled) recompileRequest = true;
	if(inConstants->fractal.customOCLFormulaDEMode != lastFractal.customOCLFormulaDEMode) recompileRequest = true;
	if(inConstants->fractal.limitsEnabled != lastFractal.limitsEnabled) recompileRequest = true;
	if(inConstants->fractal.primitives.planeEnable != lastFractal.primitives.planeEnable) recompileRequest = true;
	if(inConstants->fractal.primitives.boxEnable != lastFractal.primitives.boxEnable) recompileRequest = true;
	if(inConstants->fractal.primitives.invertedBoxEnable != lastFractal.primitives.invertedBoxEnable) recompileRequest = true;
	if(inConstants->fractal.primitives.sphereEnable != lastFractal.primitives.sphereEnable) recompileRequest = true;
	if(inConstants->fractal.primitives.invertedSphereEnable != lastFractal.primitives.invertedSphereEnable) recompileRequest = true;
	if(inConstants->fractal.primitives.waterEnable != lastFractal.primitives.waterEnable) recompileRequest = true;
	if(inConstants->params.texturedBackground != lastParams.texturedBackground) recompileRequest = true;
	if(inConstants->params.DOFmethod != lastParams.DOFmethod) recompileRequest = true;

	int engineNumber = gtk_combo_box_get_active(GTK_COMBO_BOX(Interface.comboOpenCLEngine));
	if(engineNumber != lastEngineNumber) recompileRequest = true;
	lastEngineNumber = engineNumber;

	lastParams = inConstants->params;
	lastFractal = inConstants->fractal;
	lastFormula = formula;
}

void CclSupport::Recompile(void)
{
	Disable();
	Enable();
	InitDevice();
	InitFractal();
	if(ready)
	{
		SSAOPrepare();
		DOFPrepare();
	}
	recompileRequest = false;
}

void CclSupport::Render(cImage *image, GtkWidget *outputDarea)
{
	cl_int err;

	if (recompileRequest)
	{
		Recompile();
	}

	if (ready)
	{
		stepSize = workGroupSize;
		int workGroupSizeMultiplier = 1;
		double startTime = real_clock();
		double lastTime = startTime;
		double lastTimeProcessing = startTime;
		double lastProcessingTime = 1.0;

		if(backgroundImage2D) delete backgroundImage2D; backgroundImage2D = NULL;
		if(lastParams.texturedBackground) PrepareBackgroundTexture(backgroundTextureSource);
		backgroundImage2D = new cl::Image2D(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, cl::ImageFormat(CL_RGBA, CL_UNORM_INT8), backgroundImage2DWidth, backgroundImage2DHeight,
				backgroundImage2DWidth * sizeof(cl_uchar4), backgroungImageBuffer, &err);
		if(!checkErr(err, "cl::Image2D(...backgroundImage...)")) return;

		int nDof = 1;
		if (lastParams.DOFEnabled && lastParams.DOFmethod == 1) nDof = 256;

		size_t sizeOfPixel = sizeof(sClPixel) + sizeof(sClReflect) * 10; //10 because max nuber of reflections is 10
		size_t jobSizeLimit;
		if (maxAllocMemSize > 0)
		{
			if (maxAllocMemSize * 0.75 < memoryLimitByUser)
			{
				jobSizeLimit = maxAllocMemSize / sizeOfPixel * 0.75;
			}
			else
			{
				jobSizeLimit = memoryLimitByUser / sizeOfPixel;
			}
		}
		else
		{
			jobSizeLimit = memoryLimitByUser / sizeOfPixel;
		}

		srand((unsigned int) ((double) clock() * 1000.0 / CLOCKS_PER_SEC));

		for (int dofLoop = 1; dofLoop <= nDof; dofLoop++)
		{
			for (int pixelIndex = 0; pixelIndex < width * height; pixelIndex += stepSize)
			{

				if(outCL) delete outCL; outCL = NULL;
				if(rgbbuff) delete[] rgbbuff; rgbbuff = NULL;
				if(auxReflectBuffer) delete auxReflectBuffer; auxReflectBuffer = NULL;
				if(reflectBuffer) delete[] reflectBuffer; reflectBuffer = NULL;

				double processingCycleTime = atof(gtk_entry_get_text(GTK_ENTRY(Interface.edit_OpenCLProcessingCycleTime)));
				if (processingCycleTime < 0.02) processingCycleTime = 0.02;

				workGroupSizeMultiplier *= processingCycleTime / lastProcessingTime;

				//printf("job size limit: %ld\n", jobSizeLimit);
				int pixelsLeft = width * height - pixelIndex;
				int maxWorkGroupSizeMultiplier = pixelsLeft / workGroupSize;
				if (workGroupSizeMultiplier > maxWorkGroupSizeMultiplier) workGroupSizeMultiplier = maxWorkGroupSizeMultiplier;
				if (workGroupSizeMultiplier * workGroupSize > jobSizeLimit) workGroupSizeMultiplier = jobSizeLimit / workGroupSize;
				if (workGroupSizeMultiplier < numberOfComputeUnits) workGroupSizeMultiplier = numberOfComputeUnits;

				constantsBuffer1->params.randomSeed = rand();

				stepSize = workGroupSize * workGroupSizeMultiplier;
				buffSize = stepSize * sizeof(sClPixel);
				rgbbuff = new sClPixel[buffSize];
				outCL = new cl::Buffer(*context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, buffSize, rgbbuff, &err);

				char errorText[1000];
				sprintf(errorText, "Buffer::Buffer(*context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, buffSize, rgbbuff, &err), buffSize = %ld", buffSize);
				if (!checkErr(err, errorText)) return;

				reflectBufferSize = sizeof(sClReflect) * 10 * stepSize;
				//printf("reflectBuffer size = %d\n", reflectBufferSize);
				reflectBuffer = new sClReflect[10 * stepSize];
				auxReflectBuffer = new cl::Buffer(*context, CL_MEM_READ_WRITE, reflectBufferSize, NULL, &err);
				sprintf(errorText, "Buffer::Buffer(*context, CL_MEM_READ_WRITE, reflectBufferSize, NULL, &err), reflectBufferSize = %ld", reflectBufferSize);
				if (!checkErr(err, errorText)) return;

				size_t usedGPUdMem = sizeOfPixel * stepSize;

				err = kernel->setArg(0, *outCL);
				err = kernel->setArg(1, *inCLBuffer1);
				err = kernel->setArg(2, *inCLConstBuffer1);
				err = kernel->setArg(3, *auxReflectBuffer);
				err = kernel->setArg(4, *backgroundImage2D);
				err = kernel->setArg(5, pixelIndex);

				//printf("size of inputs: %ld\n", sizeof(lastParams) + sizeof(lastFractal));

				err = queue->enqueueWriteBuffer(*inCLBuffer1, CL_TRUE, 0, sizeof(sClInBuff), inBuffer1);
				sprintf(errorText, "ComamndQueue::enqueueWriteBuffer(inCLBuffer1), used GPU mem = %ld", usedGPUdMem);
				if (!checkErr(err, errorText)) return;

				err = queue->finish();
				if (!checkErr(err, "ComamndQueue::finish() - CLBuffer")) return;

				err = queue->enqueueWriteBuffer(*inCLConstBuffer1, CL_TRUE, 0, sizeof(sClInConstants), constantsBuffer1);
				if (!checkErr(err, "ComamndQueue::enqueueWriteBuffer(inCLConstBuffer1)")) return;
				err = queue->finish();
				if (!checkErr(err, "ComamndQueue::finish() - ConstBuffer")) return;

				err = queue->enqueueNDRangeKernel(*kernel, cl::NullRange, cl::NDRange(stepSize), cl::NDRange(workGroupSize));
				sprintf(errorText, "ComamndQueue::nqueueNDRangeKernel(jobSize), jobSize = %d", stepSize);
				if (!checkErr(err, errorText)) return;

				err = queue->finish();
				if (!checkErr(err, "ComamndQueue::finish() - Kernel")) return;

				err = queue->enqueueReadBuffer(*outCL, CL_TRUE, 0, buffSize, rgbbuff);
				if (!checkErr(err, "ComamndQueue::enqueueReadBuffer()")) return;
				err = queue->finish();
				if (!checkErr(err, "ComamndQueue::finish() - ReadBuffer")) return;

				unsigned int offset = pixelIndex;

				for (unsigned int i = 0; i < stepSize; i++)
				{
					unsigned int a = offset + i;
					sClPixel pixelCl = rgbbuff[i];
					sRGBfloat pixel = { pixelCl.R, pixelCl.G, pixelCl.B };
					sRGB8 color = {pixelCl.colR, pixelCl.colG, pixelCl.colB};
					unsigned short opacity = pixelCl.opacity;
					unsigned short alpha = pixelCl.alpha;
					int x = a % width;
					int y = a / width;

					if (lastParams.DOFEnabled && lastParams.DOFmethod == 1)
					{
						sRGBfloat oldPixel = image->GetPixelImage(x, y);
						sRGBfloat newPixel;
						newPixel.R = oldPixel.R * (1.0 - 1.0 / dofLoop) + pixel.R * (1.0 / dofLoop);
						newPixel.G = oldPixel.G * (1.0 - 1.0 / dofLoop) + pixel.G * (1.0 / dofLoop);
						newPixel.B = oldPixel.B * (1.0 - 1.0 / dofLoop) + pixel.B * (1.0 / dofLoop);
						image->PutPixelImage(x, y, newPixel);
						image->PutPixelZBuffer(x, y, rgbbuff[i].zBuffer);
						unsigned short oldAlpha = image->GetPixelAlpha(x, y);
						unsigned short newAlpha = (double)oldAlpha * (1.0 - 1.0 / dofLoop) + alpha * (1.0 / dofLoop);
						image->PutPixelAlpha(x, y, newAlpha);
					}
					else
					{
						image->PutPixelImage(x, y, pixel);
						image->PutPixelZBuffer(x, y, rgbbuff[i].zBuffer);
						image->PutPixelColour(x, y, color);
						image->PutPixelOpacity(x, y, opacity);
						image->PutPixelAlpha(x, y, alpha);
					}
				}

				char progressText[1000];
				double percent;
				if (lastParams.DOFEnabled && lastParams.DOFmethod == 1)
				{
					percent = (double) (dofLoop - 1.0) / nDof + (double) (pixelIndex + stepSize) / (width * height) / nDof;
				}
				else
				{
					percent = (double) (pixelIndex + stepSize) / (width * height);
				}
				if (percent > 1.0) percent = 1.0;
				double time = real_clock() - startTime;
				double time_to_go = (1.0 - percent) * time / percent;
				int togo_time_s = (int) time_to_go % 60;
				int togo_time_min = (int) (time_to_go / 60) % 60;
				int togo_time_h = time_to_go / 3600;
				int time_s = (int) time % 60;
				int time_min = (int) (time / 60) % 60;
				int time_h = time / 3600;
				sprintf(progressText, "OpenCL - rendering. Done %.1f%%, to go %dh%dm%ds, elapsed %dh%dm%ds, used GPU mem %ld MB", percent * 100.0, togo_time_h, togo_time_min, togo_time_s,
						time_h, time_min, time_s, usedGPUdMem / 1024 / 1024);
				gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), progressText);
				gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(Interface.progressBar), percent);

				lastProcessingTime = time - lastTimeProcessing;
				lastTimeProcessing = time;

				if (real_clock() - lastTime > 30.0)
				{
					if (image->IsPreview())
					{
						image->CompileImage();
						image->ConvertTo8bit();
						image->UpdatePreview();
						image->RedrawInWidget(outputDarea);
						while (gtk_events_pending())
							gtk_main_iteration();
					}
					lastTime = real_clock();
				}

				if (programClosed) break;

				while (gtk_events_pending())
					gtk_main_iteration();
			}
			if (programClosed) break;
		}
	}

	if(outCL) delete outCL; outCL = NULL;
	if(rgbbuff) delete[] rgbbuff; rgbbuff = NULL;
	if(auxReflectBuffer) delete auxReflectBuffer; auxReflectBuffer = NULL;
	if(reflectBuffer) delete[] reflectBuffer; reflectBuffer = NULL;
	if(backgroundImage2D) delete backgroundImage2D; backgroundImage2D = NULL;
	//gdk_draw_rgb_image(outputDarea->window, renderWindow.drawingArea->style->fg_gc[GTK_STATE_NORMAL], 0, 0, clSupport->GetWidth(), clSupport->GetHeight(), GDK_RGB_DITHER_NONE,
	//		clSupport->GetRgbBuff(), clSupport->GetWidth() * 3);
}

void CclSupport::Enable(void)
{
	if(ready)
	{
		enabled = true;
	}
}

void CclSupport::Disable(void)
{
	if(context) delete context;
	context = NULL;
	if(rgbbuff) delete[] rgbbuff;
	rgbbuff = NULL;
	if(outCL) delete outCL;
	outCL = NULL;
	if(program) delete program;
	program = NULL;
	if(kernel) delete kernel;
	kernel = NULL;
	if(queue)	delete queue;
	queue = NULL;
	if (inCLBuffer1)delete inCLBuffer1;
	inCLBuffer1 = NULL;
	if(inCLConstBuffer1) delete inCLConstBuffer1;
	inCLConstBuffer1 = NULL;
	if(reflectBuffer) delete[] reflectBuffer;
	reflectBuffer = NULL;
	if(backgroungImageBuffer) delete[] backgroungImageBuffer;
	backgroungImageBuffer = NULL;
	if(backgroundImage2D) delete backgroundImage2D;
	backgroundImage2D = NULL;
	backgroundImage2DHeight = 10;
	backgroundImage2DWidth = 10;
	backgroungImageBuffer = new cl_uchar4[10*10];
	if(programSSAO) delete programSSAO;
	programSSAO = NULL;
	if(kernelSSAO) delete kernelSSAO;
	kernelSSAO = NULL;
	if(queueSSAO) delete queueSSAO;
	queueSSAO = NULL;
	SSAOprepared = false;
	if(programDOF) delete programDOF;
	programDOF = NULL;
	if(kernelDOF) delete kernelDOF;
	kernelDOF = NULL;
	if(queueDOF) delete queueDOF;
	queueDOF = NULL;
	DOFprepared = false;
	enabled = false;
	ready = false;
}

void CclSupport::SetSize(int w, int h)
{
	//if(width != w || height != h) recompileRequest = true;
	width = w;
	height = h;
}

void CclSupport::PrepareBackgroundTexture(cTexture *texture)
{
	int width = texture->Width();
	int height = texture->Height();

	if(backgroungImageBuffer) delete[] backgroungImageBuffer;
	backgroungImageBuffer = new cl_uchar4[width * height];
	backgroundImage2DWidth = width;
	backgroundImage2DHeight = height;

	for(int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			sRGB8 pixel = texture->FastPixel(x,y);
			backgroungImageBuffer[x + y * width].s0 = pixel.R;
			backgroungImageBuffer[x + y * width].s1 = pixel.G;
			backgroungImageBuffer[x + y * width].s2 = pixel.B;
			backgroungImageBuffer[x + y * width].s3 = CL_UCHAR_MAX;
		}
	}
}

void CclSupport::SSAOPrepare(void)
{
	if(!SSAOprepared)
	{
		cl_int err;
		std::string strFileSSAO = clDir + "cl_SSAO.cl";
		std::ifstream fileSSAO(strFileSSAO.c_str());
		if(!checkErr(fileSSAO.is_open() ? CL_SUCCESS : -1, ("Can't open file:" + strFileSSAO).c_str())) return;
		std::string progSSAO(std::istreambuf_iterator<char>(fileSSAO), (std::istreambuf_iterator<char>()));
		cl::Program::Sources sources;
		sources.push_back(std::make_pair(progSSAO.c_str(), progSSAO.length()));

		programSSAO = new cl::Program(*context, sources, &err);
		if(!checkErr(err, "ProgramSSAO()")) return;

		std::string buildParams = "-w -cl-single-precision-constant -cl-denorms-are-zero ";
		err = programSSAO->build(devices, buildParams.c_str());

		std::stringstream errorMessageStream;
		errorMessageStream << "OpenCL Build log:\t" << programSSAO->getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[deviceIndex]) << std::endl;
		std::string buildLogText;
		buildLogText = errorMessageStream.str();
		std::cout << buildLogText;

		if(!checkErr(err, "ProgramSSAO::build()")) return;
		printf("OpenCL SSAO program built done\n");

		kernelSSAO = new cl::Kernel(*programSSAO, "SSAO", &err);
		if(!checkErr(err, "Kernel::Kernel()")) return;
		printf("OpenCL SSAO kernel opened\n");

		queueSSAO = new cl::CommandQueue(*context, devices[deviceIndex], 0, &err);
		if(!checkErr(err, "CommandQueueSSAO::CommandQueue()"))return;
		printf("OpenCL SSAO command queue prepared\n");
	}
}

void CclSupport::DOFPrepare(void)
{
	if(!DOFprepared)
	{
		cl_int err;
		std::string strFileDOF = clDir + "cl_DOF.cl";
		std::ifstream fileDOF(strFileDOF.c_str());
		if(!checkErr(fileDOF.is_open() ? CL_SUCCESS : -1, ("Can't open file:" + strFileDOF).c_str())) return;
		std::string progDOF(std::istreambuf_iterator<char>(fileDOF), (std::istreambuf_iterator<char>()));
		cl::Program::Sources sources;
		sources.push_back(std::make_pair(progDOF.c_str(), progDOF.length()));

		programDOF = new cl::Program(*context, sources, &err);
		if(!checkErr(err, "ProgramDOF()")) return;

		std::string buildParams = "-w -cl-single-precision-constant -cl-denorms-are-zero ";
		err = programDOF->build(devices, buildParams.c_str());

		std::stringstream errorMessageStream;
		errorMessageStream << "OpenCL Build log:\t" << programDOF->getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[deviceIndex]) << std::endl;
		std::string buildLogText;
		buildLogText = errorMessageStream.str();
		std::cout << buildLogText;

		if(!checkErr(err, "ProgramDOF::build()")) return;
		printf("OpenCL DOF program built done\n");

		kernelDOF = new cl::Kernel(*programDOF, "DOF", &err);
		if(!checkErr(err, "Kernel::Kernel()")) return;
		printf("OpenCL DOF kernel opened\n");

		queueDOF = new cl::CommandQueue(*context, devices[deviceIndex], 0, &err);
		if(!checkErr(err, "CommandQueueDOF::CommandQueue()"))return;
		printf("OpenCL DOF command queue prepared\n");
	}
}

void CclSupport::SSAORender(cImage *image, GtkWidget *outputDarea)
{
	cl_int err;
	char errorText[1000];

	char progressText[1000];
	sprintf(progressText, "Rendering Screen Space Ambient Occlussion with OpenCL");
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), progressText);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(Interface.progressBar), 1.0);
	while (gtk_events_pending())
		gtk_main_iteration();

	float *zBuffer = image->GetZBufferPtr();
	size_t zBufferSize = image->GetZBufferSize();
	size_t size = width * height;

	size_t usedMemForSSAO = (sizeof(cl_ushort3)*size + sizeof(cl_uchar3)*size + sizeof(cl_ushort)*size + zBufferSize) / 1024 / 1024;
	printf("GPU memory for SSAO effect: %ld MB\n\n", usedMemForSSAO);

	cl::Buffer *zBufferCl = new cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, zBufferSize, zBuffer, &err);
	sprintf(errorText, "zBufferCl = new cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, zBufferSize, zBuffer, &err)");
	if (!checkErr(err, errorText)) return;

	cl_ushort3 *imageBuffer = new cl_ushort3[size];
	cl_uchar3 *colorBuffer = new cl_uchar3[size];
	cl_ushort *opacityBuffer = new cl_ushort[size];

	sRGB16 *image16 = image->GetImage16Ptr();
	sRGB8 *imageColour = image->GetColorPtr();
	unsigned short *imageOpacity = image->GetOpacityPtr();
	for (size_t i=0; i < size; i++)
	{
		sRGB16 pixel = image16[i];
		cl_ushort3 pixelCl;
		pixelCl.s0 = pixel.R;
		pixelCl.s1 = pixel.G;
		pixelCl.s2 = pixel.B;
		imageBuffer[i] = pixelCl;

		sRGB8 color = imageColour[i];
		cl_uchar3 colorCl;
		colorCl.s0 = color.R;
		colorCl.s1 = color.G;
		colorCl.s2 = color.B;
		colorBuffer[i] = colorCl;

		opacityBuffer[i] = imageOpacity[i];
	}
	cl::Buffer *imageBufferCl = new cl::Buffer(*context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_ushort3)*size, imageBuffer, &err);
	sprintf(errorText, "imageBufferCl = new cl::Buffer(*context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_ushort3)*size, imageBuffer, &err)");
	if (!checkErr(err, errorText)) return;

	cl::Buffer *colorBufferCl = new cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_uchar3)*size, colorBuffer, &err);
	sprintf(errorText, "imageBufferCl = new cl::Buffer(*context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_ushort3)*size, imageBuffer, &err)");
	if (!checkErr(err, errorText)) return;

	cl::Buffer *opacityBufferCl = new cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_ushort)*size, opacityBuffer, &err);
	sprintf(errorText, "imageBufferCl = new cl::Buffer(*context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(cl_ushort3)*size, imageBuffer, &err)");
	if (!checkErr(err, errorText)) return;

	err = queueSSAO->enqueueWriteBuffer(*zBufferCl, CL_TRUE, 0, zBufferSize, zBuffer);
	sprintf(errorText, "ComamndQueueSSAO(zBuffer)");
	if (!checkErr(err, errorText)) return;

	err = queueSSAO->enqueueWriteBuffer(*imageBufferCl, CL_TRUE, 0, sizeof(cl_ushort3)*size, imageBuffer);
	sprintf(errorText, "ComamndQueueSSAO(imageBuffer)");
	if (!checkErr(err, errorText)) return;

	err = queueSSAO->enqueueWriteBuffer(*opacityBufferCl, CL_TRUE, 0, sizeof(cl_ushort)*size, opacityBuffer);
	sprintf(errorText, "ComamndQueueSSAO(opacityBuffer)");
	if (!checkErr(err, errorText)) return;

	sClParamsSSAO paramsSSAO;
	paramsSSAO.width = width;
	paramsSSAO.height = height;
	paramsSSAO.fov = lastParams.persp;
	paramsSSAO.quality = lastParams.SSAOquality;
	paramsSSAO.intensity = lastParams.ambientOcclusionIntensity;

	err = kernelSSAO->setArg(0, *imageBufferCl);
	err = kernelSSAO->setArg(1, *zBufferCl);
	err = kernelSSAO->setArg(2, *colorBufferCl);
	err = kernelSSAO->setArg(3, *opacityBufferCl);
	err = kernelSSAO->setArg(4, paramsSSAO);

	stepSize = workGroupSize;
	int workGroupSizeMultiplier = 1;
	double startTime = real_clock();
	double lastTimeProcessing = startTime;
	double lastProcessingTime = 1.0;

	for (int pixelIndex = 0; pixelIndex < width * height; pixelIndex += stepSize)
	{
		double processingCycleTime = atof(gtk_entry_get_text(GTK_ENTRY(Interface.edit_OpenCLProcessingCycleTime)));
		if (processingCycleTime < 0.02) processingCycleTime = 0.02;

		double factor = processingCycleTime / lastProcessingTime;
		if (factor > 2.0) factor = 2.0;
		workGroupSizeMultiplier *= factor;

		//printf("job size limit: %ld\n", jobSizeLimit);
		int pixelsLeft = width * height - pixelIndex;
		int maxWorkGroupSizeMultiplier = pixelsLeft / workGroupSize;
		if (workGroupSizeMultiplier > maxWorkGroupSizeMultiplier) workGroupSizeMultiplier = maxWorkGroupSizeMultiplier;
		if (workGroupSizeMultiplier > width * height / workGroupSize / 8) workGroupSizeMultiplier = width * height / workGroupSize / 8;
		if (workGroupSizeMultiplier < numberOfComputeUnits) workGroupSizeMultiplier = numberOfComputeUnits;
		stepSize = workGroupSize * workGroupSizeMultiplier;

		//calculation of stepSize to be multiplied workGroupSize
		size_t limitedWorkgroupSize = workGroupSize;
		if (stepSize > pixelsLeft)
		{
			int mul = pixelsLeft / workGroupSize;
			if (mul > 0)
			{
				stepSize = mul * workGroupSize;
			}
			else
			{
				//in this case will be limited workGroupSize
				stepSize = pixelsLeft;
				limitedWorkgroupSize = pixelsLeft;
			}
		}
		err = queueSSAO->enqueueNDRangeKernel(*kernelSSAO, cl::NDRange(pixelIndex), cl::NDRange(stepSize), cl::NDRange(limitedWorkgroupSize));
		sprintf(errorText, "ComamndQueueSSAO::nqueueNDRangeKernel(size)");
		if (!checkErr(err, errorText)) return;

		err = queueSSAO->finish();
		if (!checkErr(err, "ComamndQueueSSAO::finish() - Kernel")) return;

		double time = real_clock() - startTime;
		char progressText[1000];
		double percent;
		percent = (double) (pixelIndex + stepSize) / (width * height);

		double time_to_go = (1.0 - percent) * time / percent;
		int togo_time_s = (int) time_to_go % 60;
		int togo_time_min = (int) (time_to_go / 60) % 60;
		int togo_time_h = time_to_go / 3600;
		int time_s = (int) time % 60;
		int time_min = (int) (time / 60) % 60;
		int time_h = time / 3600;
		sprintf(progressText, "SSAO OpenCL - rendering. Done %.1f%%, to go %dh%dm%ds, elapsed %dh%dm%ds", percent * 100.0, togo_time_h, togo_time_min, togo_time_s, time_h, time_min,
				time_s);
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), progressText);
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(Interface.progressBar), percent);

		if (programClosed) break;

		while (gtk_events_pending())
			gtk_main_iteration();

		lastProcessingTime = time - lastTimeProcessing;
		lastTimeProcessing = time;
	}
	err = queueSSAO->enqueueReadBuffer(*imageBufferCl, CL_TRUE, 0, sizeof(cl_ushort3)*size, imageBuffer);
	if (!checkErr(err, "ComamndQueueSSAO::enqueueReadBuffer()")) return;
	err = queueSSAO->finish();

	for (size_t i=0; i < size; i++)
	{
		sRGB16 pixel;
		cl_ushort3 pixelCl = imageBuffer[i];
		pixel.R = pixelCl.s0;
		pixel.G = pixelCl.s1;
		pixel.B = pixelCl.s2;
		image16[i] = pixel;
	}

	sprintf(progressText, "Rendering Screen Space Ambient Occlussion done");
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), progressText);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(Interface.progressBar), 1.0);
	while (gtk_events_pending())
		gtk_main_iteration();

	delete zBufferCl;
	delete[] imageBuffer;
	delete[] colorBuffer;
	delete[] opacityBuffer;
	delete imageBufferCl;
	delete colorBufferCl;
	delete opacityBufferCl;
}

void CclSupport::DOFRender(cImage *image, GtkWidget *outputDarea)
{
	cl_int err;
	char errorText[1000];

	cl_ushort4 *in_image = new cl_ushort4[width * height];
	cl_ushort4 *out_image = new cl_ushort4[width * height];
	size_t imageSize = sizeof(cl_ushort4) * width * height;

	sSortZ<cl_float> *temp_sort = new sSortZ<cl_float> [width * height];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int ptr = x + y * width;
			sRGB16 pixel16 = image->GetPixelImage16(x, y);
			in_image[ptr].s0 = pixel16.R;
			in_image[ptr].s1 = pixel16.G;
			in_image[ptr].s2 = pixel16.B;
			in_image[ptr].s3 = image->GetPixelAlpha(x, y);
			temp_sort[ptr].z = image->GetPixelZBuffer(x, y);
			temp_sort[ptr].i = ptr;
		}
	}
	memcpy(out_image, in_image, imageSize);

	if (!noGUI && image->IsPreview())
	{
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), "Rendering Depth Of Field effect. Sorting zBuffer");
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(Interface.progressBar), 0.0);
		while (gtk_events_pending())
			gtk_main_iteration();
	}

	//------------ CPU part --------------

	QuickSortZBuffer(temp_sort, 1, height * width - 1);

	if (!noGUI && image->IsPreview())
	{
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), "Rendering Depth Of Field effect. Randomizing zBuffer");
		while (gtk_events_pending())
			gtk_main_iteration();
	}

	//Randomize Z-buffer
	int imgSize = height * width;
	double neutral = lastParams.DOFFocus;
	double deep = lastParams.DOFRadius;
	for (int i = imgSize - 1; i >= 0; i--)
	{
		sSortZ<cl_float> temp;
		temp = temp_sort[i];
		double z1 = temp.z;
		double size1 = (z1 - neutral) / z1 * deep;

		int randomStep = i;

		bool done = false;
		int ii;
		do
		{
			ii = i - Random(randomStep);
			if (ii <= 0) ii = 0;
			sSortZ<cl_float> temp2 = temp_sort[ii];
			double z2 = temp2.z;
			double size2 = (z2 - neutral) / z2 * deep;

			if (size1 == 0 && size2 == 0) done = true;

			if (size1 * size2 > 0)
			{
				double sizeCompare;
				if (size1 > 0)
				{
					sizeCompare = size2 / size1;
				}
				else
				{
					sizeCompare = size1 / size2;
				}

				if (sizeCompare > 0.7)
				{
					done = true;
				}
				else
				{
					done = false;
				}
			}
			else
			{
				done = false;
			}
			randomStep = randomStep * 0.7 - 1;

			if (randomStep <= 0) done = true;
		} while (!done);
		temp_sort[i] = temp_sort[ii];
		temp_sort[ii] = temp;
	}

	//------------------ GPU part ------------------

	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), "Rendering Depth Of Field effect with OpenCL");
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(Interface.progressBar), 0.0);
	while (gtk_events_pending())
		gtk_main_iteration();

	size_t usedMemForDOF = (imageSize + imageSize + sizeof(sSortZ<cl_float>) * width * height) / 1024 / 1024;
	printf("GPU memory for DOF effect: %ld MB\n", usedMemForDOF);

	//cl::Image2D *in_imageBufferCl = new cl::Image2D(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, cl::ImageFormat(CL_RGBA, CL_UNORM_INT16), width, height,
	//		width * sizeof(cl_ushort4), in_image, &err);
	//if(!checkErr(err, "cl::Image2D(...backgroundImage...)")) return;

	//cl::Image2D *out_imageBufferCl = new cl::Image2D(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, cl::ImageFormat(CL_RGBA, CL_UNORM_INT16), width, height,
	//		width * sizeof(cl_ushort4), out_image, &err);
	//if(!checkErr(err, "cl::Image2D(...backgroundImage...)")) return;

	cl::Buffer *in_imageBufferCl = new cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imageSize, in_image, &err);
	sprintf(errorText, "in_imageBufferCl = new cl::Buffer(*context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, imageSize, in_image, &err)");
	if (!checkErr(err, errorText)) return;

	cl::Buffer *out_imageBufferCl = new cl::Buffer(*context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, imageSize, out_image, &err);
	sprintf(errorText, "out_imageBufferCl = new cl::Buffer(*context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, imageSize, out_image, &err)");
	if (!checkErr(err, errorText)) return;

	cl::Buffer *zBufferCl = new cl::Buffer(*context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(sSortZ<cl_float> ) * width * height, temp_sort, &err);
	sprintf(errorText, "zBufferCl = new cl::Buffer(*context, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR, sizeof(sSortZ<cl_float>)*width*height, temp_sort, &err)");
	if (!checkErr(err, errorText)) return;

	err = queueDOF->enqueueWriteBuffer(*in_imageBufferCl, CL_TRUE, 0, imageSize, in_image);
	sprintf(errorText, "ComamndQueueDOF(in_imageBufferCl)");
	if (!checkErr(err, errorText)) return;

	err = queueDOF->enqueueWriteBuffer(*out_imageBufferCl, CL_TRUE, 0, imageSize, out_image);
	sprintf(errorText, "ComamndQueueDOF(out_imageBufferCl)");
	if (!checkErr(err, errorText)) return;

	err = queueDOF->enqueueWriteBuffer(*zBufferCl, CL_TRUE, 0, sizeof(sSortZ<cl_float> ) * width * height, temp_sort);
	sprintf(errorText, "ComamndQueueDOF(in_imageBufferCl)");
	if (!checkErr(err, errorText)) return;

	sClParamsDOF paramsDOF;
	paramsDOF.width = width;
	paramsDOF.height = height;
	paramsDOF.focus = lastParams.DOFFocus;
	paramsDOF.radius = lastParams.DOFRadius * width / 1000.0;

	err = kernelDOF->setArg(0, *in_imageBufferCl);
	err = kernelDOF->setArg(1, *out_imageBufferCl);
	err = kernelDOF->setArg(2, *zBufferCl);
	err = kernelDOF->setArg(3, paramsDOF);

	stepSize = workGroupSize;
	int workGroupSizeMultiplier = 1;
	double startTime = real_clock();
	double lastTime = real_clock();
	double lastTimeProcessing = startTime;
	double lastProcessingTime = 1.0;

	for (int pixelIndex = 0; pixelIndex < width * height; pixelIndex += stepSize)
	{
		double processingCycleTime = atof(gtk_entry_get_text(GTK_ENTRY(Interface.edit_OpenCLProcessingCycleTime)));
		if (processingCycleTime < 0.02) processingCycleTime = 0.02;

		double factor = processingCycleTime / lastProcessingTime;
		if (factor > 2.0) factor = 2.0;
		workGroupSizeMultiplier *= factor;

		//printf("job size limit: %ld\n", jobSizeLimit);
		int pixelsLeft = width * height - pixelIndex;
		int maxWorkGroupSizeMultiplier = pixelsLeft / workGroupSize;
		if (workGroupSizeMultiplier > maxWorkGroupSizeMultiplier) workGroupSizeMultiplier = maxWorkGroupSizeMultiplier;
		if (workGroupSizeMultiplier > width * height / workGroupSize / 8) workGroupSizeMultiplier = width * height / workGroupSize / 8;
		if (workGroupSizeMultiplier < numberOfComputeUnits) workGroupSizeMultiplier = numberOfComputeUnits;
		stepSize = workGroupSize * workGroupSizeMultiplier;

		//calculation of stepSize to be multiplied workGroupSize
		size_t limitedWorkgroupSize = workGroupSize;
		if (stepSize > pixelsLeft)
		{
			int mul = pixelsLeft / workGroupSize;
			if (mul > 0)
			{
				stepSize = mul * workGroupSize;
			}
			else
			{
				//in this case will be limited workGroupSize
				stepSize = pixelsLeft;
				limitedWorkgroupSize = pixelsLeft;
			}
		}

		err = queueDOF->enqueueNDRangeKernel(*kernelDOF, cl::NDRange(pixelIndex), cl::NDRange(stepSize), cl::NDRange(limitedWorkgroupSize));
		sprintf(errorText, "ComamndQueueSSAO::nqueueNDRangeKernel(size)");
		if (!checkErr(err, errorText)) return;

		err = queueDOF->finish();
		if (!checkErr(err, "ComamndQueueSSAO::finish() - Kernel")) return;

		double time = real_clock() - startTime;
		char progressText[1000];
		double percent;
		percent = (double) (pixelIndex + stepSize) / (width * height);

		double time_to_go = (1.0 - percent) * time / percent;
		int togo_time_s = (int) time_to_go % 60;
		int togo_time_min = (int) (time_to_go / 60) % 60;
		int togo_time_h = time_to_go / 3600;
		int time_s = (int) time % 60;
		int time_min = (int) (time / 60) % 60;
		int time_h = time / 3600;
		sprintf(progressText, "DOF OpenCL - rendering. Done %.1f%%, to go %dh%dm%ds, elapsed %dh%dm%ds", percent * 100.0, togo_time_h, togo_time_min, togo_time_s, time_h, time_min,
				time_s);
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), progressText);
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(Interface.progressBar), percent);

		if (programClosed) break;

		if (real_clock() - lastTime > 60.0)
		{
			if (image->IsPreview())
			{
				err = queueDOF->enqueueReadBuffer(*out_imageBufferCl, CL_TRUE, 0, imageSize, out_image);
				sRGB16 *image16 = image->GetImage16Ptr();
				size_t size = width * height;
				for (size_t i = 0; i < size; i++)
				{
					sRGB16 pixel;
					cl_ushort3 pixelCl = out_image[i];
					pixel.R = pixelCl.s0;
					pixel.G = pixelCl.s1;
					pixel.B = pixelCl.s2;
					image16[i] = pixel;
				}
				image->ConvertTo8bit();
				image->UpdatePreview();
				image->RedrawInWidget(outputDarea);
				while (gtk_events_pending())
					gtk_main_iteration();
			}
			lastTime = real_clock();
		}

		while (gtk_events_pending())
			gtk_main_iteration();

		lastProcessingTime = time - lastTimeProcessing;
		lastTimeProcessing = time;
	}

	err = queueDOF->enqueueReadBuffer(*out_imageBufferCl, CL_TRUE, 0, imageSize, out_image);

	//cl::size_t<3> origin;
	//cl::size_t<3> region;
	//origin[0] = 0, origin[1] = 0; origin[1] = 0;
	//region[0] = width, region[1] = height, origin[2] = 1;
	//err = queueDOF->enqueueReadImage(*out_imageBufferCl, true, origin, region, width * sizeof(cl_ushort4), 0, out_image);
	//if (!checkErr(err, "ComamndQueueSSAO::enqueueReadBuffer()")) return;
	//err = queueDOF->finish();

	sRGB16 *image16 = image->GetImage16Ptr();
	size_t size = width * height;
	for (size_t i = 0; i < size; i++)
	{
		sRGB16 pixel;
		cl_ushort3 pixelCl = out_image[i];
		pixel.R = pixelCl.s0;
		pixel.G = pixelCl.s1;
		pixel.B = pixelCl.s2;
		image16[i] = pixel;
	}

	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(Interface.progressBar), "Rendering Depth Of Field effect done");
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(Interface.progressBar), 1.0);

	delete[] in_image;
	delete[] out_image;
	delete[] temp_sort;
	delete in_imageBufferCl;
	delete out_imageBufferCl;
	delete zBufferCl;
}

//------------------------------------ custom formulas -----------------------------
CCustomFormulas::CCustomFormulas(std::string dataDir)
{
	customFormulasPath = dataDir + "/custom_ocl_formulas";
	actualIndex = 0;
	count = 1;
	listOfFiles.clear();
	listOfNames.clear();

	printf("Custom formulas path: %s\n", customFormulasPath.c_str());

	//create directory for custom formulas if not exists
	DIR *dir;
	dir = opendir(customFormulasPath.c_str());
	if (dir != NULL) (void) closedir(dir);
#ifdef WIN32
	else mkdir(customFormulasPath.c_str());
#else
	else mkdir(customFormulasPath.c_str(), (S_IRUSR | S_IWUSR | S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH));
#endif

	RefreshList();
}

void CCustomFormulas::RefreshList(void)
{
	listOfFiles.clear();
	listOfNames.clear();

	GError *error = NULL;
	GDir *dir = g_dir_open(customFormulasPath.c_str(), 0, &error);

	gtk_list_store_clear(GTK_LIST_STORE(gtk_combo_box_get_model (GTK_COMBO_BOX(Interface.comboOpenCLCustomFormulas))));

	/*
	if(count > 0)
	{
		for(int i = count -1; count >=0; count--)
		{
			gtk_combo_box_remove_text(GTK_COMBO_BOX(Interface.comboOpenCLCustomFormulas), i);
		}
	}
*/
	count = 0;

	int indexTemp = actualIndex;

	if(!error)
	{
		gchar *filename;
		do
		{
			filename = (gchar*)g_dir_read_name(dir);
			if(filename)
			{
				std::string sFilename(filename);
				if(sFilename.find("Init.c") == std::string::npos)
				{
					listOfFiles.push_back(sFilename);
					std::string name;
					name = sFilename.substr(3, sFilename.length()-3-2);
					listOfNames.push_back(name);
					gtk_combo_box_append_text(GTK_COMBO_BOX(Interface.comboOpenCLCustomFormulas), name.c_str());
					count++;
				}
			}
		}
		while(filename);
	}

	if(count > 0)
	{
		gtk_combo_box_set_active(GTK_COMBO_BOX(Interface.comboOpenCLCustomFormulas), indexTemp);
		actualIndex = indexTemp;
	}

	if(dir) g_dir_close(dir);
}

void CCustomFormulas::GetActual(std::string *name, std::string *formulaFile, std::string *iniFile)
{
	if(name) *name = listOfNames[actualIndex];
	std::string filename = listOfFiles[actualIndex];
	if(formulaFile) *formulaFile = customFormulasPath + "/" + filename;
	if(iniFile) *iniFile = customFormulasPath + "/cl_" + listOfNames[actualIndex] + "Init.c";
}

void CCustomFormulas::NewFormula(std::string newName)
{
	std::string formulaFile = customFormulasPath + "/cl_" + newName + ".c";
	std::string formulaInitFile = customFormulasPath + "/cl_" + newName + "Init.c";

	std::string actualName, actualFormula, actualIni;
	GetActual(&actualName, &actualFormula, &actualIni);

	int result = fcopy(actualFormula.c_str(), formulaFile.c_str());
	if(result) printf("Cannot create new formula\n");

	result = fcopy(actualIni.c_str(), formulaInitFile.c_str());
	if(result) printf("Cannot create new formula\n");

	RefreshList();

	SetActualByName(newName);

	const char *editor = gtk_entry_get_text(GTK_ENTRY(Interface.edit_OpenCLTextEditor));
#ifdef WIN32
	spawnlp(P_NOWAIT, editor, editor, formulaFile.c_str(), NULL);
#else
	if(!fork())
	{
		execlp(editor, editor, formulaFile.c_str(), NULL);
		_exit(0);
	}
#endif

#ifdef WIN32
	spawnlp(P_NOWAIT, editor, editor, formulaInitFile.c_str(), NULL);
#else
	if(!fork())
	{
		execlp(editor, editor, formulaInitFile.c_str(), NULL);
		_exit(0);
	}
#endif
}

bool CCustomFormulas::SetActualByName(std::string name)
{
	bool result = false;
	for (int i = 0; i < count; i++)
	{
		if (name == listOfNames[i])
		{
			gtk_combo_box_set_active(GTK_COMBO_BOX(Interface.comboOpenCLCustomFormulas), i);
			actualIndex = i;
			result = true;
		}
	}
	return result;
}

void CCustomFormulas::DeleteFormula(void)
{
	std::string actualName, actualFormula, actualIni;
	GetActual(&actualName, &actualFormula, &actualIni);
	remove(actualFormula.c_str());
	remove(actualIni.c_str());
	RefreshList();
	actualIndex = 0;
	gtk_combo_box_set_active(GTK_COMBO_BOX(Interface.comboOpenCLCustomFormulas), actualIndex);
}
#endif
