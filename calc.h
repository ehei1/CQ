#include <windows.h>
#include <stdio.h>
#define SIZE 100

char buff[SIZE];

char * calc(const char *_string, int _flag, double _hitratio)
{

	HMODULE hDLL = LoadLibrary(TEXT("cqBehindCtl.dll"));

	if(hDLL == NULL){
		sprintf_s(buff,"%s","the calc module is not loaded");
		return buff;
	}
	char * (*pCalculate)(const char *, int, double) = (char * (*) (const char*, int, double))GetProcAddress(hDLL,"calculate");
	
	sprintf_s(buff,"%s",pCalculate(_string, _flag, _hitratio));

	FreeLibrary(hDLL);

	return buff;
}