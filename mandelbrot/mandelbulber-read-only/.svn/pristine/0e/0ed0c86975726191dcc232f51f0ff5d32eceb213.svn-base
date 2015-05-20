/*********************************************************
 /                   MANDELBULBER
 / class for animation timeline
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

#ifndef TIMELINE_HPP_
#define TIMELINE_HPP_

#include "database.hpp"
#include "cimage.hpp"
#include "smartptr.h"
#include "files.h"
#include "interface.h"
#include "settings.h"
#include "callbacks.h"

gboolean thumbnail_expose(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);
void PressedKeyframeThumbnail(GtkWidget *widget, GdkEventButton *event);


struct sTimelineRecord
{
	int index;
	sRGB8 thumbnail[128*128];
};

class cTimeline
{
public:
	cTimeline();
	~cTimeline();
	bool IsCreated(void) {return isCreated;}
	int Initialize(const char *keyframesPath); //returns number of keyframes
	bool GetImage(int index, sRGB8 *image);
	void DisplayInDrawingArea(int index, GtkWidget *darea);
	int CheckNumberOfKeyframes(const char *keyframesPath);
	void CreateInterface(int numberOfKeyframes);
	void RebulidTimelineWindow(void);
	void RecordKeyframe(int index, const char *keyframeFile, bool modeInsert);
	void DeleteKeyframe(int index, const char *keyframesPath);
	void Resize(int newsize);
	void Reset(void);
	void Refresh(void);
	void UpdateGlobalMorph(void);
	void GetFrameParamsInterpolated(int index, int framesPerKey, sParamRenderD *params);
	int GetKeyframeCount(void) {return keyframeCount;}
	bool isOpened;

private:
	smart_ptr<cDatabase> database;
	int keyframeCount;
	bool isCreated;
	CMorph *morphParamRenderGlobal;
};

extern smart_ptr<cTimeline> timeline;



#endif /* TIMELINE_HPP_ */
