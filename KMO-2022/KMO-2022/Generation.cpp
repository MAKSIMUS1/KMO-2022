#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include "Generation.h"
#include "In.h"
#include "Polish.h"
#include "Log.h"
#include <fstream>
#pragma once


namespace CG
{
	int cycle_schet = 1;
	void add(IT::IdTable& idtable, ConstSegment& cnst)
	{
		std::string str;
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::STR)
			{
				str += "\n\t"; str += idtable.table[i].id; str += " BYTE ";
				str += "\""; str += idtable.table[i].value.vstr->str; str += "\"";
				str += ", 0";
				strcpy(cnst.Data[cnst.size++], str.c_str());
				str.clear();
			}
			else if (idtable.table[i].idtype == IT::L && idtable.table[i].iddatatype == IT::INT)
			{
				str += "\n\t"; str += idtable.table[i].id; str += " SDWORD ";
				if (idtable.table[i].value.vint < 0)
					str += "-";
				str += std::to_string(std::abs(idtable.table[i].value.vint));
				strcpy(cnst.Data[cnst.size++], str.c_str());
				str.clear();
			}
		}
	}

	void add(IT::IdTable& idtable, DataSegment& dataseg)
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].idtype == IT::V)
				switch (idtable.table[i].iddatatype)
				{
				case IT::INT:
					strcpy(dataseg.Data[dataseg.size], "\n\t");

					strcat(dataseg.Data[dataseg.size], idtable.table[i].id);
					strcat(dataseg.Data[dataseg.size++], " SDWORD 0");
					break;
				case IT::STR:
					strcpy(dataseg.Data[dataseg.size], "\n\t");

					strcat(dataseg.Data[dataseg.size], idtable.table[i].id);
					strcat(dataseg.Data[dataseg.size++], " BYTE 255 DUP(0)");
					break;
				}
		}
	}
	void CodeSegment::add(std::string str)
	{
		std::strcpy(Data[size++], str.c_str());
	}

	void	CodeSegmentGeneration(CodeSegment& CS, LT::LexTable l, IT::IdTable& iT)
	{

		bool ifmain = false, error = false, go = false, func = false;
		std::string str;
		int expression_type = IT::INT;
		std::vector<LT::Entry> tempEntries;
		std::string func_name;
		int idxTI, goParm;
		char separators[10] = "()<>,;=u\n";
		for (int i = 0; i < l.size; i++)
		{
			str.clear();
			while (!strchr(separators, l.table[i].lexema[0]))
			{
				str += l.table[i++].lexema[0];
				if (l.table[i - 1].lexema[0] == 'i')
				{
					idxTI = l.table[i - 1].idxTI;
				}
				else if (l.table[i - 1].lexema[0] == 'p' || l.table[i - 1].lexema[0] == 'r')
					break;
			}
			if (str == "gi")
			{
				while (l.table[i - 1].lexema[0] != 'l')
					i++;
				goParm = l.table[i - 1].idxTI;
				expression_type = iT.table[idxTI].iddatatype;
				go = true;
				CS.add(generateInstructions(GO1, idxTI, iT, expression_type));
			}
			else if (str == "tfi")
			{
				func = true;
				func_name = iT.table[idxTI].id;

				func_name += "_proc";
				CS.add(generateInstructions(FUNC, idxTI, iT, expression_type, func_name));
			}
			else if (str == "ti" && iT.table[idxTI].iddatatype == IT::INT) { // параметр int

				expression_type = IT::INT;
				CS.add(generateInstructions(PARM, idxTI, iT, expression_type));
			}
			else if (str == "ti" && iT.table[idxTI].iddatatype == IT::STR) { // параметр str

				expression_type = IT::STR;
				CS.add(generateInstructions(PARM, idxTI, iT, expression_type));
			}
			else if (str == "m") {
				ifmain = true;
				func_name = "main";
				CS.add(generateInstructions(FUNC, l.table[i].idxTI, iT, expression_type, func_name));
			}
			else if (str == "r") {
				if (iT.table[l.table[i].idxTI].iddatatype == IT::STR)
				{
					expression_type = IT::STR;
					error = Polish::PolishNotation(i, l, iT);
				}
				else
				{
					expression_type = IT::INT;
					error = Polish::PolishNotation(i, l, iT);
				}
				int j = i;
				while (l.table[j].lexema[0] != ';')
				{
					tempEntries.push_back(l.table[j]);
					j++;
				}
				generateExpression(tempEntries, CS, NULL, iT, expression_type);
				endOfFucntion(CS);
				CS.add(generateInstructions(RET, NULL, iT));

			}
			else if (str == "p") {
				error = Polish::PolishNotation(i, l, iT);

				if (iT.table[l.table[i].idxTI].iddatatype == IT::STR) expression_type = IT::STR;
				else expression_type = IT::INT;
				int j = i;
				while (l.table[j].lexema[0] != ';')
				{
					tempEntries.push_back(l.table[j]);
					j++;
				}
				generateExpression(tempEntries, CS, -1, iT, expression_type);
				CS.add(generateInstructions(PRNT, -1, iT, expression_type, func_name));
			}
			if (l.table[i].lexema[0] == '>')
			{
				if (go)
				{
					go = false;
					CS.add(generateInstructions(GO2, goParm, iT, expression_type));
				}
				else if (func)
				{
					func = false;
					CS.add(generateInstructions(RET, 1, iT));
					CS.add(generateInstructions(ENDP, NULL, iT, expression_type, func_name));
				}
				else if (ifmain && !go)
				{
					endOfFucntion(CS);
					CS.add(generateInstructions(RET, 1, iT));
					CS.add(generateInstructions(ENDP, NULL, iT, expression_type, func_name));
				}

			}
			else if (l.table[i].lexema[0] == '=') {

				i++;
				int j = i;

				if (iT.table[l.table[i - 2].idxTI].iddatatype == IT::INT)
					expression_type = IT::INT;
				else { expression_type = IT::STR; }
				error = Polish::PolishNotation(i, l, iT);
				while (l.table[j - 1].lexema[0] != ';')
				{
					tempEntries.push_back(l.table[j]);
					j++;
				}
				generateExpression(tempEntries, CS, idxTI, iT, expression_type);

			}
		}
	}

	void endOfFucntion(CodeSegment& CS)
	{
		CS.add("\n\n\tjmp EXIT\n\tEXIT_DIV_ON_NULL:\n\tpush offset null_division\n\tcall outstr\n\tpush - 1\n\tcall ExitProcess");
		CS.add("\n\n\tEXIT_OVERFLOW:\n\tpush offset overflow\n\tcall outstr\n\tpush - 2\n\tcall ExitProcess\n\n\tEXIT:");
	}

	void generateExpression(std::vector<LT::Entry>& tempEntries, CodeSegment& CS, int idxTI, IT::IdTable iT, int type)
	{
		int begin = 0; int end = 0; int count = 0; bool findF = false;
		int numofparms = 0;
		for (size_t j = 0; j < tempEntries.size(); j++)
		{
			if (tempEntries[j].lexema[0] == '@')
			{

				count = tempEntries[j + 1].lexema[0] - '0';

				begin = j - count;

			}
			if (tempEntries[j].lexema[0] == '#')
			{
				end = begin + count - 1;
			}
			if (begin && end && count != 1 && tempEntries[begin - 1].idxTI != (int)TI_NULLIDX && strcmp(iT.table[tempEntries[begin - 1].idxTI].id, "concatstr") != 0)
			{

				while (end >= begin)
				{
					std::swap(tempEntries[begin++], tempEntries[end--]);
				}
				end = 0;
				begin = 0;
			}

		}
		int j = 0;
		for (size_t i = 0; i < tempEntries.size(); i++)
		{
			switch (tempEntries[i].lexema[0])
			{

			case LEX_LITERAL:
			case LEX_ID:		CS.add(generateInstructions(PUSH, tempEntries[i].idxTI, iT));	break;
			case LEX_SEPARATOR: CS.add(generateInstructions(SEP, idxTI, iT));	break;
			case LEX_OPERATOR:
			{
				if (iT.table[tempEntries[i].idxTI].value.op == '+')
				{
					CS.add(generateInstructions(ADD, tempEntries[i].idxTI, iT));
				}
				if (iT.table[tempEntries[i].idxTI].value.op == '*')
				{
					CS.add(generateInstructions(MUL, tempEntries[i].idxTI, iT));
				}
				if (iT.table[tempEntries[i].idxTI].value.op == '-')
				{
					CS.add(generateInstructions(DIFF, tempEntries[i].idxTI, iT));
				}
				if (iT.table[tempEntries[i].idxTI].value.op == '/')
				{
					CS.add(generateInstructions(DIV, tempEntries[i].idxTI, iT));
				}
				if (iT.table[tempEntries[i].idxTI].value.op == '%')
				{
					CS.add(generateInstructions(MOD, tempEntries[i].idxTI, iT));
				}
				break;
			}
			case '@':
				for (j; j < tempEntries.size(); j++)
				{
					if (tempEntries[j].idxTI != (int)TI_NULLIDX)
					{
						if (iT.table[tempEntries[j].idxTI].idtype == IT::F)
						{
							CS.add(generateInstructions(CALL, tempEntries[j].idxTI, iT));
							j++;
							break;
						}
					}
					else
						continue;
				}
				break;
			}
		};
		tempEntries.clear();
	}



	std::string	generateInstructions(INSTRUCTIONTYPE t, int idxTI, IT::IdTable iT, int type, std::string fucn_name)
	{
		std::string str; static int ret = 0;
		switch (t)
		{
		case FUNC:
			str += "\n\n"; str += fucn_name; str += " PROC";
			return str;
			break;
		case PARM:
			str += ", "; str += iT.table[idxTI].id; str += " : ";
			switch (type)
			{
			case IT::STR: str += "DWORD"; ret += 4;  return str; break;
			case IT::INT: str += "SDWORD"; ret += 4;   return str; break;
			}
			break;
		case PUSH:	if ((iT.table[idxTI].iddatatype == IT::INT || iT.table[idxTI].idtype == IT::P) && iT.table[idxTI].idtype != IT::F)
		{
			str += "\n\tpush "; str += iT.table[idxTI].id;
		}
				 else if (iT.table[idxTI].idtype != IT::F)
		{
			str += "\n\tpush offset ";	str += iT.table[idxTI].id;
		}
				 return str;
				 break;
		case RET:	if (idxTI)
			return "\n\tpush 0\n\tcall ExitProcess\n";
				else
		{
			char str[30]; char buf[4];
			std::strcpy(str, "\n\tpop eax\n\tret ");
			_itoa(ret, buf, 10);
			std::strcat(str, buf);
			std::strcat(str, "\n");
			ret = 0;
			return str;  break;
		}
		case GO1:
			str += "\n\tCYCLE"; str += std::to_string(cycle_schet); str += ": ";
			return str; break;
		case GO2:
		{
			str += "\n\tcmp eax,"; str += iT.table[idxTI].id; str += " \n\tjge NEXT"; str += std::to_string(cycle_schet);
			str += "\n\tloop CYCLE"; str += std::to_string(cycle_schet); str += "\n\tNEXT"; str += std::to_string(cycle_schet); str += ":";
			cycle_schet++;
			return str; break;
		}
		case ADD:		return "\n\tpop eax\n\tpop ebx\n\tadd eax, ebx\n\tjo EXIT_OVERFLOW\n\tpush eax"; break;
		case DIFF:		return "\n\tpop eax\n\tneg eax\n\tpop ebx\n\tadd eax, ebx\n\tjo EXIT_OVERFLOW\n\tpush eax"; break;
		case MUL:		return "\n\tpop eax\n\tpop ebx\n\timul eax, ebx\n\tjo EXIT_OVERFLOW\n\tpush eax"; break;
		case DIV:		return "\n\tpop ebx\n\tpop eax\n\ttest ebx,ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tidiv ebx\n\tpush eax"; break;
		case MOD:		return "\n\tpop ebx\n\tpop eax\n\ttest ebx,ebx\n\tjz EXIT_DIV_ON_NULL\n\tcdq\n\tidiv ebx\n\tpush edx"; break;
		case ENDP:	str += "\n"; str += fucn_name; str += " ENDP";
			switch ((strcmp(fucn_name.c_str(), "main") == 0) ? true : false)
			{
			case true: str += "\nend main"; return str;
			case false:						return str;
			}
			break;
		case PRNT:	if (type == IT::STR)
			return "\n\tcall outstr\n";
				 else
			return "\n\tcall outint\n"; break;
		case SEP:	if (idxTI != -1 && iT.table[idxTI].iddatatype == IT::INT && iT.table[idxTI].idtype != IT::F)
		{
			str += "\n\tpop "; str += iT.table[idxTI].id;
		}
				else if (idxTI != -1 && iT.table[idxTI].iddatatype == IT::STR)
		{
			str += "\n\tpush offset "; str += iT.table[idxTI].id;
			str += "\n\tcall copystr\n";
		}	return str; break;

		case CALL:
			if (strcmp(iT.table[idxTI].id, "copystr") == 0 || strcmp(iT.table[idxTI].id, "concatstr") == 0)
			{
				str += "\n\tcall "; str += iT.table[idxTI].id;
			}
			else { str += "\n\tcall "; str += iT.table[idxTI].id; str += "_proc"; }
			str += "\n\tpush eax";
			return str; break;


		}
		return "error";
	}

	void print(Log::LOG log, ConstSegment& cnst, DataSegment& dataseg, CodeSegment& codeseg)
	{
		for (int i = 0; i < cnst.size; i++)
			*log.stream << cnst.Data[i];
		for (int i = 0; i < dataseg.size; i++)
			*log.stream << dataseg.Data[i];
		for (int i = 0; i < codeseg.size; i++)
			*log.stream << codeseg.Data[i];
	}
	void DataOut(std::ofstream& _file, ConstSegment& cnst, DataSegment& dataseg, CodeSegment& codeseg)
	{
		for (int i = 0; i < cnst.size; i++)
			_file << cnst.Data[i];
		for (int i = 0; i < dataseg.size; i++)
			_file << dataseg.Data[i];
		for (int i = 0; i < codeseg.size; i++)
			_file << codeseg.Data[i];
	}
	void startGeneration(const char* file, ConstSegment& CnstS, DataSegment& DS, CodeSegment& CS)
	{ 
		std::ofstream _file; _file.open(file, std::ios_base::out | std::ios_base::trunc);
		if (_file.is_open())
		{
			remove(file);
		}
		DataOut(_file, CnstS, DS, CS);
		_file.close();
	}

	void startGeneration(wchar_t* file, ConstSegment& CnstS, DataSegment& DS, CodeSegment& CS)
	{
		std::ofstream _file; _file.open(file, std::ios_base::out | std::ios_base::trunc);
		if (_file.is_open())
		{
			DataOut(_file, CnstS, DS, CS);
		}
		_file.close();
	}
}