/*********************************************************
 /                   MANDELBULBER
 / class similar to std::vector, but much simplier
 /
 /
 / author: Krzysztof Marczak
 / contact: buddhi1980@gmail.com
 / licence: GNU GPL v3.0
 /
 ********************************************************/

#include "database.hpp"

cDatabase::cDatabase(int size)
{
	records = new char*[size];
	status = new sRecordStatus[size];

	//wartosci poczštkowe
	for (int i = 0; i < size; i++)
	{
		status[i].used = false;
		status[i].index = i;
	}
	recordCount = size;
	indexCount = size - 1;
}

void cDatabase::ClearDatabase()
{
	for (int i = 0; i < recordCount; i++)
	{
		if (status[i].used)
		{
			delete[] records[i];
		}
	}
	delete[] records;
	delete[] status;
}

//put data into database
errorCode cDatabase::SetRecord(int index, const char *record, int recordSize)
{
	errorCode err;
	if (index < recordCount)
	{
		//delete old data
		if (status[index].used) delete[] records[index];

		//przepisanie danych do rekordu
		char *data = new char[recordSize];
		const char *dataInput = record;
		for (int i = 0; i < recordSize; i++)
		{
			data[i] = dataInput[i];
		}
		records[index] = data;

		status[index].size = recordSize;
		status[index].used = true;
		err = err_ok;
	}
	else
	{
		err = err_badIndex;
	}
	return err;
}

//get data from database
errorCode cDatabase::GetRecord(int index, char *record)
{
	errorCode err;
	if (index < recordCount)
	{
		if (status[index].used)
		{
			char *data = (char*) records[index];
			char *dataOut = (char*) record;
			for (int i = 0; i < status[index].size; i++)
			{
				dataOut[i] = data[i];
			}
			err = err_ok;
		}
		else
		{
			err = err_missedRecord;
		}
	}
	else
	{
		err = err_badIndex;
	}
	return err;
}

//add new record into the end of database
int cDatabase::AddRecord(const char *record, int recordSize)
{
	recordCount++;
	//database resize
	char **records_new = new char*[recordCount];
	sRecordStatus *status_new = new sRecordStatus[recordCount];
	for (int i = 0; i < recordCount - 1; i++)
	{
		records_new[i] = records[i];
		status_new[i] = status[i];
	}
	delete[] records;
	delete[] status;
	records = records_new;
	status = status_new;

	//adding new record
	indexCount++;
	status[recordCount - 1].index = indexCount;
	status[recordCount - 1].used = false;
	SetRecord(recordCount - 1, record, recordSize);
	return indexCount;
}

//inserting new record
int cDatabase::InsertRecord(int index, const char *record, int recordSize)
{
	recordCount++;
	//database resize
	char **records_new = new char*[recordCount];
	sRecordStatus *status_new = new sRecordStatus[recordCount];

	//copying data into new array and prepare place for new record
	for (int i = 0; i < recordCount - 1; i++)
	{
		if (i < index)
		{
			records_new[i] = records[i];
			status_new[i] = status[i];
		}
		else
		{
			records_new[i + 1] = records[i];
			status_new[i + 1] = status[i];
		}
	}

	delete[] records;
	delete[] status;
	records = records_new;
	status = status_new;

	//adding new record
	indexCount++;
	status[index].index = indexCount;
	status[index].used = false;
	SetRecord(index, record, recordSize);
	return indexCount;
}

//delete record and decrease size of database
errorCode cDatabase::DeleteRecord(int index)
{
	errorCode err;
	if (index < recordCount)
	{
		recordCount--;

		//delete record data
		if (status[index].used) delete[] records[index];

		//database resize
		char **records_new = new char*[recordCount];
		sRecordStatus *status_new = new sRecordStatus[recordCount];

		//copying data into new array
		for (int i = 0; i < recordCount; i++)
		{
			if (i < index)
			{
				records_new[i] = records[i];
				status_new[i] = status[i];
			}
			else
			{
				records_new[i] = records[i + 1];
				status_new[i] = status[i + 1];
			}
		}

		delete[] records;
		delete[] status;
		records = records_new;
		status = status_new;

		err = err_ok;
	}
	else
	{
		err = err_badIndex;
	}
	return err;
}

int cDatabase::Count()
{
	return recordCount;
}

bool cDatabase::IsFilled(int index)
{
	if (index < recordCount)
	{
		if (status[index].used)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

int cDatabase::GetInternalIndex(int index)
{
	if (index < recordCount)
	{
		return status[index].index;
	}
	else
	{
		return -1;
	}
}
