#pragma once

extern "C"
{
	char* _stdcall copystr(char*, char*);
	char* _stdcall concatstr(char*, char*);
	void _stdcall outstr(char*);
	void _stdcall outint(int);

}