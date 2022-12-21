#include "pch.h"
#include "StandartLib.h"
#include <iostream>
#include <math.h>
#include <Windows.h>
#pragma warning(disable: 4996)


extern "C"
{
	char* _stdcall concatstr(char* str1, char* str2)
	{
		char buf[254];
		if (str1 == "" || str2 == "")
		{
			std::cout << "ERROR: Null string in concatstr function!!!" << std::endl;
			system("pause");
			ExitProcess(0);
		}
		strcpy(buf, str2);
		strcat(buf, str1);

		return  buf;
	}
	char* _stdcall copystr(char* str1, char* str2)
	{
		if (!str1 || !str2)
		{
			std::cout << "ERROR: Null string pointer in copystr function!!!" << std::endl;
			system("pause");
			ExitProcess(0);
		}
		else
			return strcpy(str1, str2);
	}


	void _stdcall outstr(char* s)
	{
		if (s != nullptr)
			std::cout << s << std::endl;
		else
			std::cout << "Empty line!";
	}
	void _stdcall outint(int i)
	{
		std::cout << i << std::endl;
	}
}