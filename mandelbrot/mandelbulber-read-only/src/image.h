/*********************************************************
 /                   MANDELBULBER
 / functions for image post-processing
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

#ifndef IMAGE_H_
#define IMAGE_H_

#include "cimage.hpp"
#include "fractparams.h"

struct sSSAOparams
{
	int threadNo;
	cImage *image;
	double persp;
	int quality;
	int done;
	enumPerspectiveType perspectiveType;
	int progressive;
	bool quiet;
};


extern sRGB *buddhabrotImg;
extern bool isBuddhabrot;
extern double buddhabrotAutoBright;

extern guint64 histogram[256];
extern unsigned int histogram2[1000];

void NewPalette(sRGB *p, double nasycenie);
void DrawHistogram(void);
void DrawHistogram2(void);
void PostRendering_SSAO(cImage *image, double persp, int quality, enumPerspectiveType perspectiveType, bool quiet);
void PostRendering_DOF(cImage *image, double deep, double neutral);
void DrawPalette(sRGB *palette);
void MakeStereoImage(cImage *left, cImage *right, guchar *stereoImage);
void StereoPreview(cImage *temoraryImage, guchar *stereoImage);


#endif /* IMAGE_H_ */
