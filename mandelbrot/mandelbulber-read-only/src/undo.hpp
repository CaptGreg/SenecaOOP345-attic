/*********************************************************
 /                   MANDELBULBER
 / class for undo buffer
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

#ifndef UNDO_HPP_
#define UNDO_HPP_

#include "fractparams.h"

class CParamsUndo
{
public:
	CParamsUndo(void);
	void SaveUndo(sParamRender *params);
	bool GetUndo(sParamRender *params);
	bool GetRedo(sParamRender *params);
	void LoadStatus(void);
private:
	void SaveStatus(void);
	int count;
	int last;
	int level;
	int max;
};

extern CParamsUndo undoBuffer;

#endif /* UNDO_HPP_ */
