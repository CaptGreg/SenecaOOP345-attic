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

#include "undo.hpp"
#include "settings.h"
#include "files.h"
#include "interface.h"

using namespace std;

CParamsUndo undoBuffer;

CParamsUndo::CParamsUndo(void)
{
	max = 100;
	count = 0;
	last = 0;
	level = 0;
}

void CParamsUndo::SaveUndo(sParamRender *params)
{
        string filename2=IndexFilename("undo/undo","fract",level%max);
	SaveSettings(filename2.c_str(), *params, true);
	printf("Undo information saved (index = %d)\n", level % max);
	level++;
	last = level;
	count++;
	if (count > max - 2) count = max - 2;
	SaveStatus();
}

bool CParamsUndo::GetUndo(sParamRender *params)
{
	bool result = false;
	if (count > 0)
	{
		ReadInterface(params);
                string filename2=IndexFilename("undo/undo","fract",level%max);
		SaveSettings(filename2.c_str(), *params, true);

		count--;
		level--;
		if (level < 0) level = 0;

                filename2=IndexFilename("undo/undo","fract",level%max);
		if (FileIfExists(filename2.c_str()))
		{
			LoadSettings(filename2.c_str(), *params);
			result = true;
		}
		else
		{
			printf("Undo file not found (index = %d)\n", level % max);
			result = false;
		}
	}
	else
	{
		printf("I'm sorry, no undo\n");
		result = false;
	}
	SaveStatus();
	return result;
}

bool CParamsUndo::GetRedo(sParamRender *params)
{
	bool result = false;
	if (level < last)
	{
		level++;
		count++;
		if (count > max - 2) count = max - 2;
		string filename2=IndexFilename("undo/undo", "fract", level % max);
		if (FileIfExists(filename2.c_str()))
		{
			LoadSettings(filename2.c_str(), *params);
			result = true;
		}
		else
		{
			printf("Undo file not found (index = %d)\n", level % max);
			result = false;
		}
	}
	else
	{
		printf("I'm sorry, no redo\n");
		result = false;
	}
	SaveStatus();
	return result;
}

void CParamsUndo::SaveStatus(void)
{
	FILE *file = fopen("undo/undo.state", "w");
	if (file)
	{
		fprintf(file,"%d %d %d",count,last,level);
		fclose(file);
	}
}

void CParamsUndo::LoadStatus(void)
{
	FILE *file = fopen("undo/undo.state", "r");
	if(file)
	{
		int result = fscanf(file,"%d %d %d",&count,&last,&level);
		(void)result;
		printf("Undo status: count=%d, last=%d, level=%d\n",count,last,level);
		fclose(file);
	}
}
