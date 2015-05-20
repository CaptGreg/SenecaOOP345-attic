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

#ifndef DATABASE_HPP_
#define DATABASE_HPP_

enum errorCode
{
  err_ok = 0, //operacja ok
  err_badIndex, //zły numer rekordu
  err_missedRecord //odwołanie do pustego rekordu
};

//status pojedynczego rekordu
struct sRecordStatus
{
  bool used;
  int index;
  int size;
};

//************ definicja klasy opisujšcej rekordy
class cDatabase
{
  public:
	cDatabase(int size = 1);
	~cDatabase() {ClearDatabase();};
	errorCode SetRecord(int index, const char *record, int recordSize);
	errorCode GetRecord(int index, char *record);
	int AddRecord(const char *record, int size);
	int InsertRecord(int index, const char *record, int recordSize);
	errorCode DeleteRecord(int index);
	int Count();
	bool IsFilled(int index);
	int GetInternalIndex(int index);
	int NextInternalIndex(void) {return indexCount + 1;}

  private:
	char **records;
	sRecordStatus *status;
	int recordCount;
	int indexCount;
	void ClearDatabase();
};

#endif /* DATABASE_HPP_ */
