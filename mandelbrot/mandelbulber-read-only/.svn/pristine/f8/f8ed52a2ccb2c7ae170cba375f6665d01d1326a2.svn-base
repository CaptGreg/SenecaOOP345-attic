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

#include <cstdlib>
#include <cstring>
#include "timeline.hpp"


using namespace std;

smart_ptr<cTimeline> timeline;

cTimeline::cTimeline()
{
	database.reset(new cDatabase(1));
	keyframeCount = 0;
	isCreated = false;
	isOpened = false;
	morphParamRenderGlobal = NULL;
}

cTimeline::~cTimeline()
{
}

int cTimeline::Initialize(const char *keyframesPath)
{
	int numberOfKeyframes = CheckNumberOfKeyframes(keyframesPath);
	printf("Found %d keyframes\n", numberOfKeyframes);

	smart_ptr<cImage> thumbnail;

	CreateInterface(numberOfKeyframes);

	gtk_widget_set_sensitive(timelineInterface.buAnimationDeleteKeyframe, false);
	gtk_widget_set_sensitive(timelineInterface.buAnimationInsertKeyframe, false);
	gtk_widget_set_sensitive(timelineInterface.buAnimationRecordKey2, false);

	smart_ptr<sTimelineRecord> record(new sTimelineRecord);

	for (int i = 0; i < numberOfKeyframes; i++)
	{
		if (!database->IsFilled(i))
		{
			thumbnail.reset(new cImage(128, 128));
			string filename2=IndexFilename(keyframesPath, "fract", i);
			ThumbnailRender(filename2.c_str(), thumbnail.ptr(), 1);
			thumbnail->CreatePreview(1.0);
			thumbnail->ConvertTo8bit();
			thumbnail->UpdatePreview();
			memcpy(record->thumbnail, thumbnail->GetPreviewPtr(), sizeof(sRGB8) * 128 * 128);
			record->index = i; //only for testing database

			if(isOpened)
			{
				if (i == 0) database->SetRecord(0, (char*) record.ptr(), sizeof(sTimelineRecord));
				else database->AddRecord((char*) record.ptr(), sizeof(sTimelineRecord));
			}
		}
		if (isOpened)
		{
			DisplayInDrawingArea(i, timelineInterface.darea[i]);
			while (gtk_events_pending())
				gtk_main_iteration();
			gtk_signal_connect(GTK_OBJECT(timelineInterface.darea[i]), "expose-event", GTK_SIGNAL_FUNC(thumbnail_expose), NULL);
			gtk_signal_connect(GTK_OBJECT(timelineInterface.darea[i]), "button_press_event", GTK_SIGNAL_FUNC(PressedKeyframeThumbnail), NULL);
			gtk_widget_add_events(timelineInterface.darea[i], GDK_BUTTON_PRESS_MASK);
		}
		else
		{
			isCreated = false;
			keyframeCount = numberOfKeyframes;
			return numberOfKeyframes;
		}
		gtk_widget_queue_draw(timelineInterface.table);
	}

	gtk_widget_set_sensitive(timelineInterface.buAnimationDeleteKeyframe, true);
	gtk_widget_set_sensitive(timelineInterface.buAnimationInsertKeyframe, true);
	gtk_widget_set_sensitive(timelineInterface.buAnimationRecordKey2, true);

	isCreated = true;
	keyframeCount = numberOfKeyframes;
	return numberOfKeyframes;
}

int cTimeline::CheckNumberOfKeyframes(const char *keyframesPath)
{
	string filename2;
	int maxKeyNumber = 0;
	do
	{
		filename2=IndexFilename(keyframesPath, "fract", maxKeyNumber);
		maxKeyNumber++;
	} while (FileIfExists(filename2.c_str()));
	maxKeyNumber--;
	return maxKeyNumber;
}

bool cTimeline::GetImage(int index, sRGB8 *image)
{
	errorCode err;
	smart_ptr<sTimelineRecord> record(new sTimelineRecord);
	err = database->GetRecord(index, (char*) record.ptr());
	if (!err)
	{
		memcpy(image, record->thumbnail, sizeof(sRGB8) * 128 * 128);
		return true;
	}
	return false;
}

void cTimeline::DisplayInDrawingArea(int index, GtkWidget *darea)
{
	smart_array<sRGB8> image(new sRGB8[128 * 128]);
	if (timeline->GetImage(index, image.ptr()))
	{
		gdk_draw_rgb_image(darea->window, darea->style->fg_gc[GTK_STATE_NORMAL], 0, 0, 128, 128, GDK_RGB_DITHER_MAX, (unsigned char*) image.ptr(), 128 * 3);
	}

	int selectedIndex = atoi(gtk_entry_get_text(GTK_ENTRY(timelineInterface.editAnimationKeyNumber)));

	GdkColor color_black = { 0, 0, 0, 0 };
	GdkColor color_white = { 0, 65535, 65535, 65535 };
	GdkGC *GC = gdk_gc_new(darea->window);
	if (selectedIndex == index)
	{
		gdk_gc_set_rgb_fg_color(GC, &color_black);
		gdk_draw_line(darea->window, GC, 0, 0, 127, 0);
		gdk_draw_line(darea->window, GC, 0, 0, 0, 127);
		gdk_draw_line(darea->window, GC, 1, 1, 126, 1);
		gdk_draw_line(darea->window, GC, 1, 1, 1, 126);
		gdk_gc_set_rgb_fg_color(GC, &color_white);
		gdk_draw_line(darea->window, GC, 127, 127, 127, 0);
		gdk_draw_line(darea->window, GC, 127, 127, 0, 127);
		gdk_draw_line(darea->window, GC, 126, 126, 126, 0);
		gdk_draw_line(darea->window, GC, 126, 126, 0, 126);
	}
	else
	{
		gdk_gc_set_rgb_fg_color(GC, &color_white);
		gdk_draw_line(darea->window, GC, 0, 0, 127, 0);
		gdk_draw_line(darea->window, GC, 0, 0, 0, 127);
		gdk_gc_set_rgb_fg_color(GC, &color_black);
		gdk_draw_line(darea->window, GC, 127, 127, 127, 0);
		gdk_draw_line(darea->window, GC, 127, 127, 0, 127);
	}
}

void cTimeline::CreateInterface(int numberOfKeyframes)
{
	timelineInterface.boxMain = gtk_vbox_new(FALSE, 1);
	timelineInterface.boxButtons = gtk_hbox_new(FALSE, 1);

	timelineInterface.buAnimationRecordKey2 = gtk_button_new_with_label("Record");
	timelineInterface.buAnimationInsertKeyframe = gtk_button_new_with_label("Insert after");
	timelineInterface.buAnimationDeleteKeyframe = gtk_button_new_with_label("Delete");
	timelineInterface.buNextKeyframe = gtk_button_new_with_label("Next");
	timelineInterface.buPreviousKeyframe = gtk_button_new_with_label("Previous");
	timelineInterface.buRefresh = gtk_button_new_with_label("Refresh");
	timelineInterface.editAnimationKeyNumber = gtk_entry_new();

	gtk_box_pack_start(GTK_BOX(timelineInterface.boxButtons), timelineInterface.buAnimationRecordKey2, true, true, 1);
	gtk_box_pack_start(GTK_BOX(timelineInterface.boxButtons), timelineInterface.buAnimationInsertKeyframe, true, true, 1);
	gtk_box_pack_start(GTK_BOX(timelineInterface.boxButtons), timelineInterface.buAnimationDeleteKeyframe, true, true, 1);
	gtk_box_pack_start(GTK_BOX(timelineInterface.boxButtons), timelineInterface.buPreviousKeyframe, true, true, 1);
	gtk_box_pack_start(GTK_BOX(timelineInterface.boxButtons), timelineInterface.buNextKeyframe, true, true, 1);
	gtk_box_pack_start(GTK_BOX(timelineInterface.boxButtons), timelineInterface.buRefresh, true, true, 1);
	gtk_box_pack_start(GTK_BOX(timelineInterface.boxButtons), CreateEdit("0", "Key no.:", 5, timelineInterface.editAnimationKeyNumber), true, true, 1);

	gtk_box_pack_start(GTK_BOX(timelineInterface.boxMain), timelineInterface.boxButtons, false, false, 1);

	timelineInterface.scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
	timelineInterface.windowHadjustment = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(timelineInterface.scrolledWindow));
	timelineInterface.windowVadjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(timelineInterface.scrolledWindow));
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(timelineInterface.scrolledWindow), GTK_POLICY_ALWAYS, GTK_POLICY_NEVER);
	timelineInterface.boxTable = gtk_hbox_new(false, 1);
	timelineInterface.table = gtk_table_new(2, numberOfKeyframes + 1, false);

	gtk_box_pack_start(GTK_BOX(timelineInterface.boxTable), timelineInterface.table, false, false, 1);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(timelineInterface.scrolledWindow), timelineInterface.boxTable);

	gtk_box_pack_start(GTK_BOX(timelineInterface.boxMain), timelineInterface.scrolledWindow, true, true, 1);
	gtk_container_add(GTK_CONTAINER(timeLineWindow), timelineInterface.boxMain);

	gtk_widget_show_all(timeLineWindow);

	timelineInterface.darea = new GtkWidget*[numberOfKeyframes];
	timelineInterface.label = new GtkWidget*[numberOfKeyframes];

	char widgetName[7];
	char labelText[20];

	for (int i = 0; i < numberOfKeyframes; i++)
	{
		timelineInterface.darea[i] = gtk_drawing_area_new();
		sprintf(widgetName, "da%d", i);
		sprintf(labelText, "keyframe %d", i);
		timelineInterface.label[i] = gtk_label_new(labelText);
		gtk_widget_set_size_request(timelineInterface.darea[i], 128, 128);
		gtk_widget_set_name(timelineInterface.darea[i], widgetName);

		gtk_table_attach(GTK_TABLE(timelineInterface.table), timelineInterface.label[i], i, i + 1, 0, 1, GTK_EXPAND, GTK_EXPAND, 1, 0);
		gtk_table_attach(GTK_TABLE(timelineInterface.table), timelineInterface.darea[i], i, i + 1, 1, 2, GTK_EXPAND, GTK_EXPAND, 1, 0);
		gtk_widget_show(timelineInterface.darea[i]);
		gtk_widget_show(timelineInterface.label[i]);
	}

	g_signal_connect(G_OBJECT(timelineInterface.buNextKeyframe), "clicked", G_CALLBACK(PressedNextKeyframe), NULL);
	g_signal_connect(G_OBJECT(timelineInterface.buPreviousKeyframe), "clicked", G_CALLBACK(PressedPreviousKeyframe), NULL);
	g_signal_connect(G_OBJECT(timelineInterface.buAnimationRecordKey2), "clicked", G_CALLBACK(PressedRecordKeyframe), NULL);
	g_signal_connect(G_OBJECT(timelineInterface.buAnimationInsertKeyframe), "clicked", G_CALLBACK(PressedInsertKeyframe), NULL);
	g_signal_connect(G_OBJECT(timelineInterface.buAnimationDeleteKeyframe), "clicked", G_CALLBACK(PressedDeleteKeyframe), NULL);
	g_signal_connect(G_OBJECT(timelineInterface.buRefresh), "clicked", G_CALLBACK(PressedTimelineRefresh), NULL);

	isOpened = true;
}

void cTimeline::RebulidTimelineWindow(void)
{
	CreateInterface(keyframeCount);
	for (int i = 0; i < keyframeCount; i++)
	{
		DisplayInDrawingArea(i, timelineInterface.darea[i]);
		gtk_signal_connect(GTK_OBJECT(timelineInterface.darea[i]), "expose-event", GTK_SIGNAL_FUNC(thumbnail_expose), NULL);
		gtk_signal_connect(GTK_OBJECT(timelineInterface.darea[i]), "button_press_event", GTK_SIGNAL_FUNC(PressedKeyframeThumbnail), NULL);
		gtk_widget_add_events(timelineInterface.darea[i], GDK_BUTTON_PRESS_MASK);
		gtk_widget_queue_draw(timelineInterface.table);
	}
}

void cTimeline::RecordKeyframe(int index, const char *keyframeFile, bool modeInsert)
{

	smart_ptr<sTimelineRecord> record(new sTimelineRecord);
	smart_ptr<cImage> thumbnail(new cImage(128, 128));
	ThumbnailRender(keyframeFile, thumbnail.ptr(), 1);
	thumbnail->CreatePreview(1.0);
	thumbnail->ConvertTo8bit();
	thumbnail->UpdatePreview();
	memcpy(record->thumbnail, thumbnail->GetPreviewPtr(), sizeof(sRGB8) * 128 * 128);
	record->index = index;
	if (index < keyframeCount)
	{
		if (modeInsert)
		{
			database->InsertRecord(index, (char*) record.ptr(), sizeof(sTimelineRecord));
			Resize(keyframeCount + 1);
			keyframeCount++;
		}
		else
		{
			database->SetRecord(index, (char*) record.ptr(), sizeof(sTimelineRecord));
		}
	}
	else
	{
		database->AddRecord((char*) record.ptr(), sizeof(sTimelineRecord));
		Resize(keyframeCount + 1);
		keyframeCount++;
	}
	DisplayInDrawingArea(index, timelineInterface.darea[index]);
	gtk_widget_queue_draw(timelineInterface.table);
}

void cTimeline::DeleteKeyframe(int index, const char *keyframesPath)
{
	string filename, filename2;
	filename=IndexFilename(keyframesPath, "fract", index);
	if (remove(filename.c_str()) != 0)
	{
		fprintf(stderr, "Error! Cannot delete keyframe file\n");
	}
	else
	{
		for (int i = index; i < keyframeCount - 1; i++)
		{
			filename=IndexFilename(keyframesPath, "fract", i);
			filename2=IndexFilename(keyframesPath, "fract", i + 1);
			rename(filename2.c_str(), filename.c_str());
		}
		database->DeleteRecord(index);
		Resize(keyframeCount - 1);
		keyframeCount--;
		gtk_widget_queue_draw(timelineInterface.table);
	}
}

void cTimeline::Resize(int newsize)
{
	gtk_table_resize(GTK_TABLE(timelineInterface.table), 2, newsize + 1);

	if (newsize < keyframeCount)
	{
		for (int i = newsize; i < keyframeCount; i++)
		{
			gtk_widget_destroy(timelineInterface.darea[i]);
			gtk_widget_destroy(timelineInterface.label[i]);
		}
	}

	//resize array with drawing area widgets
	GtkWidget **darea_new = new GtkWidget*[newsize];
	int max;
	if (newsize < keyframeCount) max = newsize;
	else max = keyframeCount;
	for (int i = 0; i < max; i++)
	{
		darea_new[i] = timelineInterface.darea[i];
	}
	delete[] timelineInterface.darea;
	timelineInterface.darea = darea_new;

	char widgetName[7];
	char labelText[20];
	if (newsize > keyframeCount)
	{
		for (int i = keyframeCount; i < newsize; i++)
		{
			timelineInterface.darea[i] = gtk_drawing_area_new();
			sprintf(widgetName, "da%d", i);
			gtk_widget_set_size_request(timelineInterface.darea[i], 128, 128);
			gtk_widget_set_name(timelineInterface.darea[i], widgetName);

			sprintf(labelText, "keyframe %d", i);
			timelineInterface.label[i] = gtk_label_new(labelText);

			gtk_table_attach(GTK_TABLE(timelineInterface.table), timelineInterface.label[i], i, i + 1, 0, 1, GTK_EXPAND, GTK_EXPAND, 1, 0);
			gtk_table_attach(GTK_TABLE(timelineInterface.table), timelineInterface.darea[i], i, i + 1, 1, 2, GTK_EXPAND, GTK_EXPAND, 1, 0);
			gtk_widget_show(timelineInterface.darea[i]);
			gtk_widget_show(timelineInterface.label[i]);
			gtk_signal_connect(GTK_OBJECT(timelineInterface.darea[i]), "expose-event", GTK_SIGNAL_FUNC(thumbnail_expose), NULL);
			gtk_signal_connect(GTK_OBJECT(timelineInterface.darea[i]), "button_press_event", GTK_SIGNAL_FUNC(PressedKeyframeThumbnail), NULL);
			gtk_widget_add_events(timelineInterface.darea[i], GDK_BUTTON_PRESS_MASK);
		}
	}
	gtk_widget_queue_draw(timelineInterface.table);
}

void cTimeline::Reset(void)
{
	database.reset(new cDatabase(1));
	keyframeCount = 0;
	isCreated = false;
	isOpened = false;
	if (timeLineWindow)
	{
		gtk_widget_destroy(timeLineWindow);
	}
}

void cTimeline::Refresh(void)
{
	gtk_widget_destroy(timelineInterface.boxMain);
	database.reset(new cDatabase(1));
	keyframeCount = 0;
	isCreated = false;
	isOpened = false;
	Initialize(Interface_data.file_keyframes);
}

void cTimeline::UpdateGlobalMorph(void)
{
	if(morphParamRenderGlobal) delete morphParamRenderGlobal;
	morphParamRenderGlobal = new CMorph (keyframeCount, sizeof(sParamRenderD) / sizeof(double));

	sParamRender fractParam;
	ReadInterface(&fractParam);

	morphParamRenderGlobal->SetFramesPerKey(fractParam.framesPerKeyframe);

	for (int keyNumber = 0; keyNumber < keyframeCount; keyNumber++)
	{
		string filename;
		filename = IndexFilename(fractParam.file_keyframes, "fract", keyNumber);
		sParamRender fractParamLoaded;
		LoadSettings(filename.c_str(), fractParamLoaded, true);
		morphParamRenderGlobal->AddData(keyNumber, (double*) &fractParamLoaded.doubles);
	}
}

void cTimeline::GetFrameParamsInterpolated(int index, int framesPerKey, sParamRenderD *params)
{
	if(index > 0 && index < framesPerKey * keyframeCount)
	{
		morphParamRenderGlobal->SetFramesPerKey(framesPerKey);
		morphParamRenderGlobal->CatmullRom(index, (double*) params);
	}
	else
	{
		memset(params, 0, sizeof(sParamRenderD));
	}
}

gboolean thumbnail_expose(GtkWidget *widget, GdkEventExpose *event, gpointer user_data)
{
	const char* widgetName = gtk_widget_get_name(widget);
	int number = 0;
	sscanf(widgetName, "da%d", &number);
	timeline->DisplayInDrawingArea(number, widget);
	while (gtk_events_pending())
		gtk_main_iteration();
	return true;
}

void PressedKeyframeThumbnail(GtkWidget *widget, GdkEventButton *event)
{
	const char* widgetName = gtk_widget_get_name(widget);
	int index = 0;
	sscanf(widgetName, "da%d", &index);

	gtk_entry_set_text(GTK_ENTRY(timelineInterface.editAnimationKeyNumber), IntToString(index));
	gtk_widget_queue_draw(timelineInterface.table);

	if (event->type == GDK_2BUTTON_PRESS)
	{
		string filename2 = IndexFilename(Interface_data.file_keyframes, "fract", index);
		if (FileIfExists(filename2.c_str()))
		{
			sParamRender fractParamLoaded;
			LoadSettings(filename2.c_str(), fractParamLoaded, true);
			KeepOtherSettings(&fractParamLoaded);
			WriteInterface(&fractParamLoaded);
			last_keyframe_position = fractParamLoaded.doubles.vp;

			Interface_data.animMode = false;
			Interface_data.playMode = false;
			Interface_data.recordMode = false;
			Interface_data.continueRecord = false;
			Interface_data.keyframeMode = false;

			programClosed = true;
			isPostRendering = false;
			renderRequest = true;
		}
		else
		{
			GtkWidget *dialog =
					gtk_message_dialog_new(GTK_WINDOW(window_interface), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CANCEL, "Error! Keyframe does not exist: %s", filename2.c_str());
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			gtk_entry_set_text(GTK_ENTRY(timelineInterface.editAnimationKeyNumber), IntToString(index));
		}

		sParamRender params;
		ReadInterface(&params);
		char distanceString[1000];
		double actDistance = CalculateDistance(params.doubles.vp, params.fractal);
		double key_distance = 0.0;
		if (timeline->isOpened)
		{
			sParamRenderD paramsD;
			int keyframe = atoi(gtk_entry_get_text(GTK_ENTRY(timelineInterface.editAnimationKeyNumber)));
			timeline->GetFrameParamsInterpolated((keyframe - 1) * 100, 100, &paramsD);
			key_distance = (params.doubles.vp - paramsD.vp).Length();
		}
		sprintf(distanceString, "Estimated viewpoint distance to the surface: %g\nDistance from last keyframe: %g", actDistance, key_distance);
		gtk_label_set_text(GTK_LABEL(Interface.label_NavigatorEstimatedDistance), distanceString);
	}
}


