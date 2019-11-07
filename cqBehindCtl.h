#include <windows.h>
#include <stdio.h>
#define SIZE 100

static char buff[SIZE];

char * calc(const char *_string, int _flag, double _time)
{

	HMODULE hDLL = LoadLibrary(TEXT("cqBehindCtl.dll"));

	if(hDLL == NULL){
		sprintf_s(buff,"%s","the calc module is not loaded");
		return buff;
	}
	char * (*pCalculate)(const char *, int, double) = (char * (*) (const char*, int, double))GetProcAddress(hDLL,"cqBehindCtl");
	
	sprintf_s(buff,"%s",pCalculate(_string, _flag, _time));

	FreeLibrary(hDLL);

	return buff;
}