#include <fstream>
#include <string>
#include <iostream>
#include "In.h"
#include "Error.h"

using namespace std;

namespace In
{
	IN getin(wchar_t* infile)
	{
		IN inner_text;

		ifstream file;
		file.open(infile);

		if (!file)
			throw ERROR_THROW(110);

		unsigned char tempChar;
		int position = 0;
		inner_text.size = 0;
		inner_text.lines = 1;
		inner_text.ignor = 0;
		inner_text.text = new unsigned char[IN_MAX_LEN_TEXT];
		bool flagEmptyFile = true;

		while ((tempChar = file.get()) && !file.eof())
		{
			flagEmptyFile = false;

			switch (inner_text.code[tempChar])
			{
			case IN::T:																//  разрещающий символ
			{
				if (tempChar == IN_CODE_ENDL)										// конец строки
				{
					inner_text.text[inner_text.size] = '\n';
					inner_text.size++;
					inner_text.lines++;
					position = 0;
					break;
				}
				inner_text.text[inner_text.size++] = tempChar;
				position++;
				break;
			}

			case IN::I:																// игнорирование
			{
				inner_text.ignor++;
				position++;
				break;
			}

			case IN::F:																// запрещающий символ
			{
				throw ERROR_THROW_IN(111, inner_text.lines, position + 1);
				break;
			}
			default:
			{
				inner_text.text[inner_text.size++] = inner_text.code[tempChar];		//замена символа
				position++;
				break;
			}
			}
		}

		if (flagEmptyFile)
			throw ERROR_THROW(113);

		if (inner_text.size == 0)
			inner_text.lines = 0;

		inner_text.text[inner_text.size] = '\0';
		file.close();

		setlocale(LC_ALL, "rus");
		inner_text.text = BetterText(inner_text.text, strlen((const char*)inner_text.text));
		inner_text.size = strlen((const char*)inner_text.text);
		return inner_text;
	}

	unsigned char* BetterText(unsigned char* str, int size)
	{
		unsigned char* text_out = new unsigned char[size * 3];
		unsigned char* text_out_SpaceDelete = new unsigned char[size * 3];
		int j = 0;
		int i = 1;
		bool findKov = false;
		bool findComment = false;
		for (int i = 0; i < size * 3; i++, j++)
		{
			if (str[i] == '$')
			{
				while (true)
				{
					i++;
					if (str[i] == '\n')
						break;
					if (str[i] == '\0')
						break;
				}
			}
			if (str[i] == '\'' && !findKov)
			{
				findKov = true;
				text_out[j] = str[i];
				continue;
			}
			if (findKov)
			{
				text_out[j] = str[i];
				if (str[i] == '\'')
					findKov = false;
				if (str[i] == '\0')	break;
				continue;
			}
			if (str[i] == '-' && str[i + 1] == '>')
			{
				text_out[j] = ' ';
				text_out[j + 1] = str[i];
				text_out[j + 2] = str[i + 1];
				text_out[j + 3] = ' ';
				j += 3;
				i++;
			}
			else
			{
				if (str[i] == ';' || str[i] == ',' || str[i] == '<' || str[i] == '>'
					|| str[i] == '(' || str[i] == ')' || str[i] == '=' || str[i] == '+' || str[i] == '-' || str[i] == '/' || str[i] == '*' || str[i] == '%')
				{
					text_out[j] = ' ';
					text_out[j + 1] = str[i];
					text_out[j + 2] = ' ';
					j += 2;
				}
				else
					text_out[j] = str[i];
			}
			if (str[i] == '\0')	break;
		}
		text_out[j + 1] = '\0';

		bool b = false;
		int g = 0;
		for (int k = 0; k < size * 3; k++)
		{
			if (text_out[k] == ' ')
			{
				if (b)
					continue;
				b = true;
			}
			else
				b = false;
			text_out_SpaceDelete[g] = text_out[k];
			g++;
		}
		text_out_SpaceDelete[j] = '\0';
		return text_out_SpaceDelete;
	}
}