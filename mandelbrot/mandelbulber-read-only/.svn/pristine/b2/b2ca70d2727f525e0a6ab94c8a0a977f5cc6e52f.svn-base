/*********************************************************
 /                   MANDELBULBER
 / Functions called by UI
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

#ifndef CALLBACKS_H_
#define CALLBACKS_H_

#include "Render3D.h"

extern double last_navigator_step;
extern CVector3 last_keyframe_position;
extern bool renderRequest;

gboolean CallerTimerLoop(void);
gboolean CallerTimerLoopWindowRefresh(GtkWidget *widget);
gboolean motion_notify_event(GtkWidget *widget, GdkEventMotion *event);
gboolean pressed_button_on_image(GtkWidget *widget, GdkEventButton *event);
gboolean pressed_button_on_palette(GtkWidget *widget, GdkEventButton *event);
gboolean StopRenderingAndQuit(GtkWidget *widget, GdkEvent *event, gpointer data);
gboolean WindowReconfigured(GtkWindow *window, GdkEvent *event, gpointer data);
gboolean on_darea_expose(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);
gboolean on_dareaPalette_expose(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);
void StartRendering(GtkWidget *widget, gpointer data);
void StopRendering(GtkWidget *widget, gpointer data);
void PressedApplyBrigtness(GtkWidget *widget, gpointer data);
void PressedLoadSettings(GtkWidget *widget, gpointer data);
void PressedSaveSettings(GtkWidget *widget, gpointer data);
void PressedSaveImage(GtkWidget *widget, gpointer data);
void PressedSaveImagePNG(GtkWidget *widget, gpointer data);
void PressedSaveImagePNG16(GtkWidget *widget, gpointer data);
void PressedSaveImagePNG16Alpha(GtkWidget *widget, gpointer data);
void PressedOkDialogFiles(GtkWidget *widget, gpointer data);
void PressedCancelDialogFiles(GtkWidget *widget, gpointer data);
void PressedNavigatorUp(GtkWidget *widget, gpointer data);
void PressedNavigatorDown(GtkWidget *widget, gpointer data);
void PressedNavigatorLeft(GtkWidget *widget, gpointer data);
void PressedNavigatorRight(GtkWidget *widget, gpointer data);
void PressedNavigatorRotateLeft(GtkWidget *widget, gpointer data);
void PressedNavigatorRotateRight(GtkWidget *widget, gpointer data);
void PressedNavigatorMoveUp(GtkWidget *widget, gpointer data);
void PressedNavigatorMoveDown(GtkWidget *widget, gpointer data);
void PressedNavigatorMoveLeft(GtkWidget *widget, gpointer data);
void PressedNavigatorMoveRight(GtkWidget *widget, gpointer data);
void PressedNavigatorForward(GtkWidget *widget, gpointer data);
void PressedNavigatorBackward(GtkWidget *widget, gpointer data);
void PressedNavigatorInit(GtkWidget *widget, gpointer data);
void PressedAnimationRecord(GtkWidget *widget, gpointer data);
void PressedAnimationContinueRecording(GtkWidget *widget, gpointer data);
void PressedAnimationRender(GtkWidget *widget, gpointer data);
void ChangedComboScale(GtkWidget *widget, gpointer data);
void ChangedComboFormula(GtkWidget *widget, gpointer data);
void ChangedTgladFoldingMode(GtkWidget *widget, gpointer data);
void ChangedJulia(GtkWidget *widget, gpointer data);
void ChangedSphericalFoldingMode(GtkWidget *widget, gpointer data);
void ChangedLimits(GtkWidget *widget, gpointer data);
void ChangedAmbientOcclusion(GtkWidget *widget, gpointer data);
void ChangedFastAmbientOcclusion(GtkWidget *widget, gpointer data);
void ChangedMandelboxRotations(GtkWidget *widget, gpointer data);
void ChangedIFSFoldingMode(GtkWidget *widget, gpointer data);
void ChangedSliderFog(GtkWidget *widget, gpointer data);
void ChangedSliderDOF(GtkWidget *widget, gpointer data);
void PressedSSAOUpdate(GtkWidget *widget, gpointer data);
void PressedDOFUpdate(GtkWidget *widget, gpointer data);
void PressedDistributeLights(GtkWidget *widget, gpointer data);
void RecalculateIFSParams(sFractal& fractal);
void PressedIFSNormalizeOffset(GtkWidget *widget, gpointer data);
void PressedIFSNormalizeVectors(GtkWidget *widget, gpointer data);
void PressedRecordKeyframe(GtkWidget *widget, gpointer data);
void PressedInsertKeyframe(GtkWidget *widget, gpointer data);
void PressedKeyframeAnimationRender(GtkWidget *widget, gpointer data);
void PressedPreviousKeyframe(GtkWidget *widget, gpointer data);
void PressedNextKeyframe(GtkWidget *widget, gpointer data);
void PressedTimelineRefresh(GtkWidget *widget, gpointer data);
void PressedUndo(GtkWidget *widget, gpointer data);
void PressedRedo(GtkWidget *widget, gpointer data);
void PressedBuddhabrot(GtkWidget *widget, gpointer data);
void PressedSelectDestination(GtkWidget *widget, gpointer data);
void PressedSelectBackground(GtkWidget *widget, gpointer data);
void PressedSelectEnvmap(GtkWidget *widget, gpointer data);
void PressedSelectLightmap(GtkWidget *widget, gpointer data);
void PressedSelectFlightPath(GtkWidget *widget, gpointer data);
void PressedSelectKeyframes(GtkWidget *widget, gpointer data);
void ChangedSliderPaletteOffset(GtkWidget *widget, gpointer data);
void PressedRandomPalette(GtkWidget *widget, gpointer data);
void CreateFormulaSequence(sFractal &fractal);
void PressedGetPaletteFromImage(GtkWidget *widget, gpointer data);
void PressedTimeline(GtkWidget *widget, gpointer data);
void DeleteTimelineWindow(GtkWidget *widget, gpointer data);
void PressedDeleteKeyframe(GtkWidget *widget, gpointer widget_pointer);
void UpdatePreviewSettingsDialog(GtkFileChooser *file_chooser, gpointer data);
void UpdatePreviewImageDialog(GtkFileChooser *file_chooser, gpointer data);
void PressedIFSDefaultDodeca(GtkWidget *widget, gpointer widget_pointer);
void PressedIFSDefaultIcosa(GtkWidget *widget, gpointer widget_pointer);
void PressedIFSDefaultOcta(GtkWidget *widget, gpointer widget_pointer);
void PressedIFSDefaultMengerSponge(GtkWidget *widget, gpointer widget_pointer);
void PressedIFSReset(GtkWidget *widget, gpointer data);
void PressedAutoDEStep(GtkWidget *widget, gpointer widget_pointer);
void PressedAutoDEStepHQ(GtkWidget *widget, gpointer widget_pointer);
void ChangedConstantDEThreshold(GtkWidget *widget, gpointer data);
void ChangedImageProportion(GtkWidget *widget, gpointer data);
void PressedCopyToClipboard(GtkWidget *widget, gpointer data);
void PressedPasteFromClipboard(GtkWidget *widget, gpointer data);
void PressedLoadExample(GtkWidget *widget, gpointer data);
void PressedAutoFog(GtkWidget *widget, gpointer data);
void PressedMeasureActivation(GtkWidget *widget, gpointer data);
void ChangedOpenClEnabled(GtkWidget *widget, gpointer data);
void ChangedIterFogEnable(GtkWidget *widget, gpointer data);
void PressedSaveAllImageLayers(GtkWidget *widget, gpointer data);
void PressedServerEnable(GtkWidget *widget, gpointer data);
void PressedClientEnable(GtkWidget *widget, gpointer data);
void PressedServerScan(GtkWidget *widget, gpointer data);
void ChangedOpenClCustomEnable(GtkWidget *widget, gpointer data);
void ChangedComboOpenCLCustomFormulas(GtkWidget *widget, gpointer data);
void PressedOpenCLEditFormula(GtkWidget *widget, gpointer data);
void PressedOpenCLEditFormulaInit(GtkWidget *widget, gpointer data);
void PressedOpenCLNewFormula(GtkWidget *widget, gpointer data);
void PressedOpenCLDeleteFormula(GtkWidget *widget, gpointer data);
void PressedRecompile(GtkWidget *widget, gpointer data);
void PressedPath2Keyframes(GtkWidget *widget, gpointer data);

double ScanFractal(sParamRender *params, CVector3 direction);
double ScanSizeOfFractal(sParamRender *params);
void AutoDEStep(bool highQuality);
bool SendSettingsToClients(sParamRender fractParamToSave);
bool GetSettingsfromServer(char *data, size_t size);
void NoGUIClientEnable(void);

#endif /* CALLBACKS_H_ */
