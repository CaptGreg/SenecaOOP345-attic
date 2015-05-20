/*********************************************************
 /                   MANDELBULBER
 / Main program and CPU rendering engine
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 / Windows port by Makemeunsee, Knigty
 ********************************************************/

#ifndef RENDER3D_H_
#define RENDER3D_H_

#include "algebra.hpp"
#include "image.h"
#include "texture.hpp"
#include "fractparams.h"
#include "cl_support.hpp"

struct sParam
{
  int start;
  int z;
  int progressive;
  int progressiveStart;
  sParamRender param;
  cImage *image;
  int *done;
  int *thread_done;
  bool *linesToSend;
};

struct sStep
{
	double distance;
	double step;
	CVector3 point;
	int iters;
	double distThresh;
};

struct sReflect
{
	CVector3 start;
	CVector3 point;
	CVector3 viewVector;
	CVector3 distance;
	sStep *stepBuff;
	int buffCount;
	double depth;
	double lastDist;
	bool found;
	double distThresh;
	enumObjectType objectType;
	double reflect;
};

//global
extern guint64 N_counter;
extern guint64 Loop_counter;
extern guint64 DE_counter;
extern guint64 Pixel_counter;
extern int Missed_DE_counter;
extern double DEerror;
extern guint64 DE_counterForDEerror;

extern double start_time;
extern bool isRendering;
extern bool isPostRendering;
extern int NR_THREADS;
extern bool noGUI;

extern cImage mainImage;

double real_clock(void);
void MainRender(void);
void *MainThread(void *ptr);
void Render(sParamRender param, cImage *image, GtkWidget *outputDarea);
void InitMainParameters(sParamRender *fractParam);
bool InitMainImage(cImage *image, int width, int height, double previewScale, GtkWidget *drawingArea);
bool LoadTextures(sParamRender *params);
void ThumbnailRender(const char *settingsFile, cImage *miniImage, int mode);
void ThumbnailRender2(sParamRender fractParamLoaded, cImage *miniImage);
CVector3 RayMarching(sParamRender *param, sFractal *calcParam, CVector3 start, CVector3 direction, double maxScan, bool binaryEnable, sStep *stepBuff, int *buffCount,
		double *distThreshOut, double *lastDistOut, bool *foundOut, double *depthOut);
void DrawKeyframePaths(sParamRender *param, cImage *image, GtkWidget *outputDarea);
void DrawHud(sParamRender *param, cImage *image, GtkWidget *outputDarea);
CVector3 CalcPointPersp(CVector3 point, CRotationMatrix rot, double persp);
#endif /* RENDER3D_H_ */
