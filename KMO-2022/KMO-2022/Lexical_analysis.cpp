#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include "stdafx.h"
#include "Parm.h"
#include "IT.h"
#include "LT.h"
#include "Lexical_analysis.h"
#include "In.h"
#include "Error.h"
#include "FST.h"

LT::LexTable __LexTable = LT::Create(LT_MAXSIZE - 1);
IT::IdTable __IdTable = IT::Create(TI_MAXSIZE - 1);
std::fstream file;

namespace Lex_analysis
{
	void ITLog(IT::IdTable it, Log::LOG log)
	{
		int i, numberOP = 0;
		*log.stream << "\nТаблица идентификаторов:\n\n";
		*log.stream << "№   " << " | " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в ТЛ" << " | " << " Значение   " << endl;
		*log.stream << setw(87) << endl;
		for (i = 0; i < it.size; i++)
		{
			*log.stream << setfill('0') << setw(4) << right << i << "    ";
			*log.stream << setfill(' ') << setw(13) << left << it.table[i].id;
			switch (it.table[i].iddatatype)
			{
			case IT::INT: *log.stream << setw(10) << left;
				if (it.table[i].idtype == IT::OP)*log.stream << "-";
				else *log.stream << "integer   "; break;
			case IT::STR: *log.stream << setw(10) << left;
				if (it.table[i].idtype == IT::OP) *log.stream << "-";
				else *log.stream << "string    "; break;
			default: *log.stream << setw(10) << std::left << "unknown"; break;
			}
			switch (it.table[i].idtype)
			{
			case IT::V: *log.stream << setw(23) << left << "\t\tпеременная"; break;
			case IT::F: *log.stream << setw(23) << left << "\t\tфункция"; break;
			case IT::P: *log.stream << setw(23) << left << "\t\tпараметр"; break;
			case IT::L: *log.stream << setw(23) << left << "\t\tлитерал"; break;
			case IT::OP: *log.stream << setw(23) << left << "\t\tоператор"; break;
				break;
			default: *log.stream << setw(23) << left << "\t\tunknown"; break;
			}
			*log.stream << setw(15) << left << it.table[i].idxfirstLE;
			if (it.table[i].iddatatype == IT::INT && (it.table[i].idtype == IT::V || it.table[i].idtype == IT::L))
				*log.stream << setw(18) << left << it.table[i].value.vint;
			else if (it.table[i].iddatatype == IT::STR && (it.table[i].idtype == IT::V || it.table[i].idtype == IT::L))
				*log.stream << "[" << it.table[i].value.vstr->len << "]" << " \"" << it.table[i].value.vstr->str << "\" " << "";
			else if (it.table[i].idtype == IT::OP)
				*log.stream << setw(18) << left << it.table[i].value.op;
			else
				*log.stream << "       -       ";
			*log.stream << endl;
		}

		*log.stream << "\nКоличество идентификаторов:\n " << i << endl;

	}


	void LTLog(LT::LexTable lt, Log::LOG log)
	{
		*log.stream << "\n";
		*log.stream << "Таблица лексем:\n\n";
		for (int i = 0; i < lt.size; i++)
		{
			*log.stream << i+1 << "\t" << lt.table[i].lexema[0] << "\t\t" << "idxTI" << lt.table[i].idxTI << endl;
		}
	}
	void LTLogPolska(LT::LexTable lt, Log::LOG log)
	{

		int line = 2;
		*log.stream << "\n\t\tПромежуточное представление\n";
		*log.stream << "0001| ";
		for (int i = 0; i < lt.size; i++) {
			if (lt.table[i].lexema[0] == ';' || lt.table[i].lexema[0] == '<') {
				*log.stream << lt.table[i].lexema << std::endl;
				if (i + 1 == lt.size) break;
				line > 999 ? *log.stream << line << "| " : line > 99 ? *log.stream << "0" << line << "| " : line > 9 ? *log.stream << "00" << line <<
					"| " : *log.stream << "000" << line << "| ";
				line++;
			}
			else *log.stream << lt.table[i].lexema;
		}
	}

	int Hex_to_dec(char* s)
	{
		int res = 0;
		if (s[0] == '~')
		{
			for (int i = 3; s[i] != NULL; ++i)
			{
				res *= 16;
				switch (s[i])
				{
				case 'A':
					res += 10;
					break;
				case 'B':
					res += 11;
					break;
				case 'C':
					res += 12;
					break;
				case 'D':
					res += 13;
					break;
				case 'E':
					res += 14;
					break;
				case 'F':
					res += 15;
					break;
				default:
					res += (s[i] - '0');
					break;
				}
			}
			res = 0 - res;
		}
		else
		{
			for (int i = 2; s[i] != NULL; ++i)
			{
				res *= 16;
				switch (s[i])
				{
				case 'A':
					res += 10;
					break;
				case 'B':
					res += 11;
					break;
				case 'C':
					res += 12;
					break;
				case 'D':
					res += 13;
					break;
				case 'E':
					res += 14;
					break;
				case 'F':
					res += 15;
					break;
				default:
					res += (s[i] - '0');
					break;
				}
			}
		}
		return res;
	}
	int Dec_to_dec(char* s)
	{
		int res = 0;
		if (s[0] == '~')
		{
			s[0] = '-';
			res = atoi(s);
		}
		else
			res = atoi(s);
		return res;
	}
	int Oct_to_dec(char* s)
	{
		int res = 0;

		if (s[0] == '~')
		{
			for (int i = 3; s[i] != NULL; ++i)
			{
				res *= 8;
				res += (s[i] - '0');
			}
			res = 0 - res;
		}
		else
		{
			for (int i = 2; s[i] != NULL; ++i)
			{
				res *= 8;
				res += (s[i] - '0');
			}
		}
		return res;
	}
	int Bin_to_dec(char* s)
	{
		int res = 0;
		if (s[0] == '~')
		{
			for (int i = 3; s[i] != NULL; ++i)
			{
				res *= 2;
				res += (s[i] - '0');
			}
			res = 0 - res;
		}
		else
		{
			for (int i = 2; s[i] != NULL; ++i)
			{
				res *= 2;
				res += (s[i] - '0');
			}
		}
		return res;
	}

	void Add_Operator_To_LT(int line, char c, char _operatorName)
	{
		LT::Entry entry;
		entry.sn = line;
		entry.idxTI = __IdTable.size;
		memset(entry.lexema, '\0', strlen(entry.lexema) - 1);
		entry.lexema[0] = c;
		entry.priority = Get_Priority(_operatorName);
		LT::Add(__LexTable, entry);
	}

	void Add_To_LT(int line, char c)
	{
		LT::Entry entry;
		entry.sn = line;
		if (c != 'i')
			entry.idxTI = LT_TI_NULLIDX;
		else
			entry.idxTI = __IdTable.size;
		memset(entry.lexema, '\0', strlen(entry.lexema) - 1);
		entry.lexema[0] = c;
		LT::Add(__LexTable, entry);
	}

	void Add_To_LT_INDEF(int line, char c, int idx)
	{
		LT::Entry entry;
		entry.sn = line;
		entry.idxTI = idx;
		memset(entry.lexema, '\0', strlen(entry.lexema) - 1);
		entry.lexema[0] = c;
		LT::Add(__LexTable, entry);
	}

	void Add_To_LT_LITERAL(int line, char c, int idx)
	{
		LT::Entry entry;
		entry.sn = line;
		entry.idxTI = idx;
		memset(entry.lexema, '\0', strlen(entry.lexema) - 1);
		entry.lexema[0] = c;
		LT::Add(__LexTable, entry);
	}



	LEX Lexical_analysis(In::IN in)
	{
		LEX lex;
		LEX_FLAGS _LEX_FLAGS;

		IT::Entry _IT_entry;
		char tmp_char;
		char tmp_word[1024] = "";
		int line = 0;
		short k = 0;
		char tmp_func[ID_MAXSIZE * 10] = "";
		char tmp_func_external[ID_MAXSIZE * 20 + 1] = "";
		char tmp_func_standart_library[ID_MAXSIZE * 20 + 1] = "";
		unsigned short literal_count = 1;
		int idx;
		bool findSameLiteral = false;
		int litValue;
		bool findLibraryFunc_copystr = false;
		bool findLibraryFunc_concatstr = false;
		TYPEOFFST FST_type;
		for (int i = 0; i <= in.size; i++)
		{
			tmp_char = in.text[i];
			if (tmp_char == IN_CODE_ENDL)
			{
				if (_LEX_FLAGS.__flag_comment)
				{
					_LEX_FLAGS.__flag_comment = false;
					k = 0;
					goto next;
				}
				line++;
				continue;
			}

			if (tmp_char != ' ' && tmp_char != '\0')
			{
				tmp_word[k] = tmp_char;
				if (k == 0 && tmp_word[k] == '\'' && !_LEX_FLAGS.__flag_comment)
				{
					i++; k++;
					tmp_char = in.text[i];
					while (tmp_char != '\'')
					{
						tmp_word[k] = tmp_char;
						i++; k++;
						tmp_char = in.text[i];
					}
					tmp_word[k] = tmp_char;
				}
				k++;
				continue;
			}
			else
			{
				tmp_word[k] = '\0';
				if ((tmp_word[0] == '\0') || (tmp_word[0] == ' ' && tmp_word[1] == '\0'))
					continue;
				k = 0;
				//для перевода через бубну
				std::string lit_value_str_ = "", lit_value_str2_ = "";
				char lit_value_str_buf_[TI_STR_MAXSIZE - 1] = "";
				if (tmp_word[k] == '$' && tmp_word[k + 1] == '\0')
				{
					_LEX_FLAGS.__flag_comment = true;
				}
				if (_LEX_FLAGS.__flag_comment)
					goto next;
				FST_type = Proverka_FST(tmp_word);
				switch (FST_type)
				{
				case _TYPEINT:
					Add_To_LT(line, LEX_INTEGER);
					_LEX_FLAGS.__flag_int = true;
					break;
				case _TYPESTRING:
					Add_To_LT(line, LEX_STRING);
					_LEX_FLAGS.__flag_string = true;
					break;
				case _TYPEFUNCTION:
					Add_To_LT(line, LEX_FUNCTION);
					if ((_LEX_FLAGS.__flag_function && _LEX_FLAGS.__flag_declare) && (_LEX_FLAGS.__flag_int || _LEX_FLAGS.__flag_string))
						_LEX_FLAGS.__flag_function_external = true;
					_LEX_FLAGS.__flag_function = true;
					break;
				case _STRCREATE:
					Add_To_LT(line, LEX_CREATE);
					_LEX_FLAGS.__flag_declare = true;
					break;
				case _STRRETURN:
					Add_To_LT(line, LEX_RETURN);
					_LEX_FLAGS.__flag_return = true;
					break;
				case _STRMAIN:
					Add_To_LT(line, LEX_MAINFUNC);
					strcpy_s(tmp_func, tmp_word);
					_LEX_FLAGS.__flag_function = true;
					break;
				case _STRPRINT:
					Add_To_LT(line, LEX_PRINT);
					break;
				case _LEFTBRACE:
					Add_To_LT(line, LEX_LEFTBRACE);
					break;
				case _RIGHTBRACE:
					Add_To_LT(line, LEX_BRACELET);
					if (_LEX_FLAGS.__flag_return)
					{
						_LEX_FLAGS.__flag_function = false;
						_LEX_FLAGS.__flag_return = false;
					}
					break;
				case _LEFTTHESIS:
					Add_To_LT(line, LEX_LEFTHESIS);
					_LEX_FLAGS.__flag_int = false;
					_LEX_FLAGS.__flag_string = false;
					if (_LEX_FLAGS.__flag_function || _LEX_FLAGS.__flag_function_external || _LEX_FLAGS.__flag_standard_library)
						_LEX_FLAGS.__flag_parameter = true;
					break;
				case _RIGHTTHESIS:
					Add_To_LT(line, LEX_RIGHTHESIS);
					_LEX_FLAGS.__flag_int = false;
					_LEX_FLAGS.__flag_string = false;
					_LEX_FLAGS.__flag_parameter = false;
					break;
				case _SEMICOLON:
					Add_To_LT(line, LEX_SEMICOLON);
					_LEX_FLAGS.__flag_int = false;
					_LEX_FLAGS.__flag_string = false;
					_LEX_FLAGS.__flag_parameter = false;
					_LEX_FLAGS.__flag_declare = false;
					_LEX_FLAGS.__flag_function_external = false;
					_LEX_FLAGS.__flag_standard_library = false;
					break;
				case _COMMA:
					Add_To_LT(line, LEX_COMMA);
					_LEX_FLAGS.__flag_int = false;
					_LEX_FLAGS.__flag_string = false;
					break;
				case _PLUS:
					Add_Operator_To_LT(line, LEX_PLUS, '+');
					_LEX_FLAGS.__flag_operator = true;
					break;
				case _MINUS:
					Add_Operator_To_LT(line, LEX_MINUS, '-');
					_LEX_FLAGS.__flag_operator = true;
					break;
				case _DEL:
					Add_Operator_To_LT(line, LEX_DIRSLASH, '/');
					_LEX_FLAGS.__flag_operator = true;
					break;
				case _UMN:
					Add_Operator_To_LT(line, LEX_STAR, '*');
					_LEX_FLAGS.__flag_operator = true;
					break;
				case _PERCENT:
					Add_Operator_To_LT(line, LEX_PERCENT, '%');
					_LEX_FLAGS.__flag_operator = true;
					break;
				case _RAVNO:
					Add_To_LT(line, LEX_RAV);
					_LEX_FLAGS.__flag_declare = false;
					_LEX_FLAGS.__flag_int = false;
					_LEX_FLAGS.__flag_string = false;
					break;
					// Литералы int
				case _LITERAL_INT_HEX:
					litValue = Hex_to_dec(tmp_word);
					for (int k = 0; k < __IdTable.size; k++)
					{
						if (__IdTable.table[k].value.vint == litValue &&
							__IdTable.table[k].idtype == IT::L &&
							__IdTable.table[k].iddatatype == IT::INT)
						{
							Add_To_LT_LITERAL(line, LEX_LITERAL, k);
							findSameLiteral = true;
							break;
						}
					}
					if (!findSameLiteral)
						Add_To_LT_LITERAL(line, LEX_LITERAL, __IdTable.size);
					break;

				case _LITERAL_INT_DEC:
					litValue = Dec_to_dec(tmp_word);
					for (int k = 0; k < __IdTable.size; k++)
					{
						if (__IdTable.table[k].value.vint == litValue &&
							__IdTable.table[k].idtype == IT::L &&
							__IdTable.table[k].iddatatype == IT::INT)
						{
							Add_To_LT_LITERAL(line, LEX_LITERAL, k);
							findSameLiteral = true;
							break;
						}
					}
					if (!findSameLiteral)
						Add_To_LT_LITERAL(line, LEX_LITERAL, __IdTable.size);
					break;
				case _LITERAL_INT_OCT:
					litValue = Oct_to_dec(tmp_word);
					for (int k = 0; k < __IdTable.size; k++)
					{
						if (__IdTable.table[k].value.vint == litValue &&
							__IdTable.table[k].idtype == IT::L &&
							__IdTable.table[k].iddatatype == IT::INT)
						{
							Add_To_LT_LITERAL(line, LEX_LITERAL, k);
							findSameLiteral = true;
							break;
						}
					}
					if (!findSameLiteral)
						Add_To_LT_LITERAL(line, LEX_LITERAL, __IdTable.size);
					break;
				case _LITERAL_INT_BIN:
					litValue = Bin_to_dec(tmp_word);
					for (int k = 0; k < __IdTable.size; k++)
					{
						if (__IdTable.table[k].value.vint == litValue &&
							__IdTable.table[k].idtype == IT::L &&
							__IdTable.table[k].iddatatype == IT::INT)
						{
							Add_To_LT_LITERAL(line, LEX_LITERAL, k);
							findSameLiteral = true;
							break;
						}
					}
					if (!findSameLiteral)
						Add_To_LT_LITERAL(line, LEX_LITERAL, __IdTable.size);
					break;
					// цикл
				case _FSTGO:
					Add_To_LT(line, LEX_GO);
					break;
				case _FSTCONDITION:
					Add_To_LT(line, LEX_CONDITION);
					break;
					// литерал string
				case _LITERAL_STR:
					// перевод через бубну
					memset(lit_value_str_buf_, '\0', strlen(lit_value_str_buf_));
					lit_value_str_ = tmp_word;
					std::remove_copy(lit_value_str_.begin(), lit_value_str_.end(), std::back_inserter(lit_value_str2_), '\'');
					strcpy(lit_value_str_buf_, lit_value_str2_.c_str());
					if (strlen(lit_value_str_buf_) == 0)
						throw ERROR_THROW(314);
					for (int k = 0; k < __IdTable.size; k++)
					{
						if ((strcmp(__IdTable.table[k].value.vstr->str, lit_value_str_buf_) == 0) &&
							__IdTable.table[k].idtype == IT::L &&
							__IdTable.table[k].iddatatype == IT::STR)
						{
							Add_To_LT_LITERAL(line, LEX_LITERAL, k);
							findSameLiteral = true;
							break;
						}
					}
					if (!findSameLiteral)
						Add_To_LT_LITERAL(line, LEX_LITERAL, __IdTable.size);
					break;
				case _FSTIDENF:

					if (strlen(tmp_word) > ID_MAXSIZE)
					{
						throw ERROR_THROW(118);
					}
					if (IT::IsId(__IdTable, tmp_word) != (int)TI_NULLIDX)
					{
						if (__IdTable.table[IT::IsId(__IdTable, tmp_word)].idtype == IT::F)
						{
							Add_To_LT_INDEF(line, LEX_ID, IT::IsId(__IdTable, tmp_word));
						}
					}
					else
					{
						if (_LEX_FLAGS.__flag_function)
						{
							char buffer_parm2[ID_MAXSIZE * 3 + 1] = "";
							strcat(buffer_parm2, tmp_func);
							strcat(buffer_parm2, "_");
							strcat(buffer_parm2, tmp_word);

							if (IT::IsId(__IdTable, buffer_parm2) == (int)TI_NULLIDX)
							{
								if (!_LEX_FLAGS.__flag_declare && (!_LEX_FLAGS.__flag_string && !_LEX_FLAGS.__flag_int))
									throw ERROR_THROW(309);
								Add_To_LT(line, LEX_ID);
							}
							else
								Add_To_LT_INDEF(line, LEX_ID, IT::IsId(__IdTable, buffer_parm2));
						}
					}
					break;
				case _STANDART_LIBRARY_COPYSTR:
					if (findLibraryFunc_copystr)
						Add_To_LT_INDEF(line, LEX_ID, IT::IsId(__IdTable, tmp_word));
					else
					{
						Add_To_LT(line, LEX_ID);
					}
					break;
				case _STANDART_LIBRARY_CONCATSTR:
					if (findLibraryFunc_concatstr)
						Add_To_LT_INDEF(line, LEX_ID, IT::IsId(__IdTable, tmp_word));
					else
					{
						Add_To_LT(line, LEX_ID);
					}
					break;
				default:
					throw ERROR_THROW(117);
				}
			
			// copystr ( F )
			if (FST_type == _STANDART_LIBRARY_COPYSTR && !findLibraryFunc_copystr)
			{
				if (__LexTable.table[__LexTable.size - 2].lexema[0] == LEX_FUNCTION)
					throw ERROR_THROW(308);
				memset(_IT_entry.id, '\0', ID_MAXSIZE * 3 + 1);
				strcpy_s(_IT_entry.id, tmp_word);
				_IT_entry.iddatatype = IT::STR;
				strcpy_s(_IT_entry.value.vstr->str, "");
				_IT_entry.idtype = IT::F;
				_IT_entry.idxfirstLE = __LexTable.size;
				IT::Add(__IdTable, _IT_entry);
				findLibraryFunc_copystr = true;
				goto next;
			}
			// concatstr ( F )
			if (FST_type == _STANDART_LIBRARY_CONCATSTR && !findLibraryFunc_concatstr)
			{
				if (__LexTable.table[__LexTable.size-2].lexema[0] == LEX_FUNCTION)
					throw ERROR_THROW(308);
				memset(_IT_entry.id, '\0', ID_MAXSIZE * 3 + 1);
				strcpy_s(_IT_entry.id, tmp_word);
				_IT_entry.iddatatype = IT::STR;
				strcpy_s(_IT_entry.value.vstr->str, "");
				_IT_entry.idtype = IT::F;
				_IT_entry.idxfirstLE = __LexTable.size;
				IT::Add(__IdTable, _IT_entry);
				findLibraryFunc_concatstr = true;
			}
			// переменная ( V )
			if ((_LEX_FLAGS.__flag_int || _LEX_FLAGS.__flag_string) && (!_LEX_FLAGS.__flag_parameter && FST_type == _FSTIDENF)
				&& (_LEX_FLAGS.__flag_declare) && (!_LEX_FLAGS.__flag_function_external))
			{
				memset(_IT_entry.id, '\0', ID_MAXSIZE * 3 + 1);
				if (strlen(tmp_word) > ID_MAXSIZE)
				{
					throw ERROR_THROW(118);
				}
				if (_LEX_FLAGS.__flag_function)
				{
					char buffer_parm[ID_MAXSIZE * 3 + 1] = "";
					strcat(buffer_parm, tmp_func);
					strcat(buffer_parm, "_");
					strcat(buffer_parm, tmp_word);
					if (IT::IsId(__IdTable, buffer_parm) == (int)TI_NULLIDX)
					{
						strcpy_s(_IT_entry.id, buffer_parm);
					}
					else
					{
						throw ERROR_THROW(310);
					}
				}
				if (_LEX_FLAGS.__flag_int)
				{
					_IT_entry.iddatatype = IT::INT;
					_IT_entry.value.vint = 0;
				}
				if (_LEX_FLAGS.__flag_string)
				{
					_IT_entry.iddatatype = IT::STR;
					strcpy_s(_IT_entry.value.vstr->str, "");
				}
				_IT_entry.idtype = IT::V;
				_IT_entry.idxfirstLE = __LexTable.size;

				IT::Add(__IdTable, _IT_entry);
				goto next;
			}
			// функция ( F ) 
			if ((_LEX_FLAGS.__flag_int || _LEX_FLAGS.__flag_string) && (_LEX_FLAGS.__flag_function && FST_type == _FSTIDENF)
				&& (!_LEX_FLAGS.__flag_parameter) && (!_LEX_FLAGS.__flag_declare))
			{
				memset(_IT_entry.id, '\0', ID_MAXSIZE * 3 + 1);
				if (strlen(tmp_word) > ID_MAXSIZE)
				{
					throw ERROR_THROW(118);
				}
				else
				{
					if (IT::IsId(__IdTable, tmp_word) == (int)TI_NULLIDX)
					{
						strcpy_s(_IT_entry.id, tmp_word);
						strcpy_s(tmp_func, tmp_word);
					}
					else
					{
						throw ERROR_THROW(310);
					}
				}
				if (_LEX_FLAGS.__flag_int)
				{
					_IT_entry.iddatatype = IT::INT;
					_IT_entry.value.vint = 0;
				}
				if (_LEX_FLAGS.__flag_string)
				{
					_IT_entry.iddatatype = IT::STR;
					strcpy_s(_IT_entry.value.vstr->str, "");
				}
				_IT_entry.idtype = IT::F;
				_IT_entry.idxfirstLE = __LexTable.size;

				IT::Add(__IdTable, _IT_entry);
				goto next;
			}
			// функция (ВНЕШНЯЯ) ( F ) 
			if (_LEX_FLAGS.__flag_function_external && FST_type == _FSTIDENF && !_LEX_FLAGS.__flag_parameter)
			{
				memset(_IT_entry.id, '\0', ID_MAXSIZE * 3 + 1);
				strcpy_s(_IT_entry.id, tmp_word);
				strcpy_s(tmp_func_external, tmp_word);
				if (_LEX_FLAGS.__flag_int)
				{
					_IT_entry.iddatatype = IT::INT;
					_IT_entry.value.vint = 0;
				}
				if (_LEX_FLAGS.__flag_string)
				{
					_IT_entry.iddatatype = IT::STR;
					strcpy_s(_IT_entry.value.vstr->str, "");
				}
				_IT_entry.idtype = IT::F;
				_IT_entry.idxfirstLE = __LexTable.size;

				IT::Add(__IdTable, _IT_entry);
				goto next;
			}
			// параметр ( P )
			if ((_LEX_FLAGS.__flag_int || _LEX_FLAGS.__flag_string) && (_LEX_FLAGS.__flag_function && FST_type == _FSTIDENF) && (_LEX_FLAGS.__flag_parameter))
			{
				memset(_IT_entry.id, '\0', ID_MAXSIZE * 3 + 1);
				//если функция внешняя
				if (_LEX_FLAGS.__flag_function_external)
				{
					if (strlen(tmp_word) > ID_MAXSIZE)
					{
						throw ERROR_THROW(118);
					}
					else
					{
						char buffer_parm[ID_MAXSIZE * 3 + 1] = "";
						strcat(buffer_parm, tmp_func_external);
						strcat(buffer_parm, "_");
						strcat(buffer_parm, tmp_word);
						strcpy_s(_IT_entry.id, buffer_parm);
					}
					if (_LEX_FLAGS.__flag_int)
					{
						_IT_entry.iddatatype = IT::INT;
						_IT_entry.value.vint = 0;
					}
					if (_LEX_FLAGS.__flag_string)
					{
						_IT_entry.iddatatype = IT::STR;
						strcpy_s(_IT_entry.value.vstr->str, "");
					}
					_IT_entry.idtype = IT::P;
					_IT_entry.idxfirstLE = __LexTable.size;

					IT::Add(__IdTable, _IT_entry);
					goto next;
				}
				//если функция не внешняя
				if (strlen(tmp_word) > ID_MAXSIZE)
				{
					throw ERROR_THROW(118);
				}
				else
				{
					char buffer_parm[ID_MAXSIZE * 3 + 1] = "";
					strcat(buffer_parm, tmp_func);
					strcat(buffer_parm, "_");
					strcat(buffer_parm, tmp_word);
					strcpy_s(_IT_entry.id, buffer_parm);
				}
				if (_LEX_FLAGS.__flag_int)
				{
					_IT_entry.iddatatype = IT::INT;
					_IT_entry.value.vint = 0;
				}
				if (_LEX_FLAGS.__flag_string)
				{
					_IT_entry.iddatatype = IT::STR;
					strcpy_s(_IT_entry.value.vstr->str, "");
				}
				_IT_entry.idtype = IT::P;
				_IT_entry.idxfirstLE = __LexTable.size;

				IT::Add(__IdTable, _IT_entry);
				goto next;
			}
			// литерал ( L )
			if ((FST_type == _LITERAL_INT_HEX || FST_type == _LITERAL_INT_DEC || FST_type == _LITERAL_INT_OCT || FST_type == _LITERAL_INT_BIN || FST_type == _LITERAL_STR) && !findSameLiteral)
			{
				memset(_IT_entry.id, '\0', ID_MAXSIZE * 3 + 1);
				char lit_numb[4];
				if (FST_type == _LITERAL_INT_HEX)
				{
					_IT_entry.iddatatype = IT::INT;
					_IT_entry.value.vint = Hex_to_dec(tmp_word);
				}
				if (FST_type == _LITERAL_INT_DEC)
				{
					_IT_entry.iddatatype = IT::INT;
					_IT_entry.value.vint = Dec_to_dec(tmp_word);
				}
				if (FST_type == _LITERAL_INT_OCT)
				{
					_IT_entry.iddatatype = IT::INT;
					_IT_entry.value.vint = Oct_to_dec(tmp_word);
				}
				if (FST_type == _LITERAL_INT_BIN)
				{
					_IT_entry.iddatatype = IT::INT;
					_IT_entry.value.vint = Bin_to_dec(tmp_word);
				}
				if (FST_type == _LITERAL_STR)
				{
					_IT_entry.iddatatype = IT::STR;

					// перевод через бубну
					std::string lit_value_str = tmp_word, lit_value_str2;
					std::remove_copy(lit_value_str.begin(), lit_value_str.end(), std::back_inserter(lit_value_str2), '\'');
					if (lit_value_str2.length() > TI_STR_MAXSIZE - 1)
					{
						throw ERROR_THROW(119);
					}
					char lit_value_str_buf[TI_STR_MAXSIZE - 1];
					strcpy(lit_value_str_buf, lit_value_str2.c_str());
					strcpy_s(_IT_entry.value.vstr->str, lit_value_str_buf);
					_IT_entry.value.vstr->len = strlen(_IT_entry.value.vstr->str);
				}
				itoa(literal_count, lit_numb, 10);
				char buffer_parm[ID_MAXSIZE * 3 + 1] = "";
				strcat(buffer_parm, "L");
				strcat(buffer_parm, lit_numb);
				strcpy_s(_IT_entry.id, buffer_parm);

				_IT_entry.idtype = IT::L;
				_IT_entry.idxfirstLE = __LexTable.size;

				IT::Add(__IdTable, _IT_entry);
				literal_count++;
				goto next;
			}
			// оператор ( OP )
			if (_LEX_FLAGS.__flag_operator)
			{
				_LEX_FLAGS.__flag_operator = false;
				memset(_IT_entry.id, '\0', ID_MAXSIZE * 3 + 1);
				switch (FST_type)
				{
				case _PLUS:
					_IT_entry.value.op = '+';
					break;
				case _MINUS:
					_IT_entry.value.op = '-';
					break;
				case _DEL:
					_IT_entry.value.op = '/';
					break;
				case _UMN:
					_IT_entry.value.op = '*';
					break;
				case _PERCENT:
					_IT_entry.value.op = '%';
					break;
				}
				_IT_entry.iddatatype = IT::INT;
				_IT_entry.idtype = IT::OP;
				_IT_entry.idxfirstLE = __LexTable.size;

				IT::Add(__IdTable, _IT_entry);
				goto next;
			}
		}
		next:

			findSameLiteral = false;
			if (!_LEX_FLAGS.__flag_standard_library)
				memset(tmp_func_standart_library, '\0', strlen(tmp_func_standart_library));
			if (!_LEX_FLAGS.__flag_function_external)
				memset(tmp_func_external, '\0', strlen(tmp_func_external));
			if (!_LEX_FLAGS.__flag_function)
				memset(tmp_func, '\0', strlen(tmp_func));
			memset(tmp_word, '\0', strlen(tmp_word));
		}

		lex.idtable = __IdTable;
		lex.lextable = __LexTable;
		return lex;
	}
	TYPEOFFST Proverka_FST(char* c)
	{
		FST::FST typeint(c,
			8,
			FST::NODE(1, FST::RELATION('i', 1)),
			FST::NODE(1, FST::RELATION('n', 2)),
			FST::NODE(1, FST::RELATION('t', 3)),
			FST::NODE(1, FST::RELATION('e', 4)),
			FST::NODE(1, FST::RELATION('g', 5)),
			FST::NODE(1, FST::RELATION('e', 6)),
			FST::NODE(1, FST::RELATION('r', 7)),
			FST::NODE()
		);

		FST::FST typestring(c,
			7,
			FST::NODE(1, FST::RELATION('s', 1)),
			FST::NODE(1, FST::RELATION('t', 2)),
			FST::NODE(1, FST::RELATION('r', 3)),
			FST::NODE(1, FST::RELATION('i', 4)),
			FST::NODE(1, FST::RELATION('n', 5)),
			FST::NODE(1, FST::RELATION('g', 6)),
			FST::NODE()
		);

		FST::FST typefunction(c,
			9,
			FST::NODE(1, FST::RELATION('f', 1)),
			FST::NODE(1, FST::RELATION('u', 2)),
			FST::NODE(1, FST::RELATION('n', 3)),
			FST::NODE(1, FST::RELATION('c', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
			FST::NODE(1, FST::RELATION('i', 6)),
			FST::NODE(1, FST::RELATION('o', 7)),
			FST::NODE(1, FST::RELATION('n', 8)),
			FST::NODE()
		);

		FST::FST strcreate(c,
			7,
			FST::NODE(1, FST::RELATION('c', 1)),
			FST::NODE(1, FST::RELATION('r', 2)),
			FST::NODE(1, FST::RELATION('e', 3)),
			FST::NODE(1, FST::RELATION('a', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
			FST::NODE(1, FST::RELATION('e', 6)),
			FST::NODE()
		);

		FST::FST streturn(c,
			7,
			FST::NODE(1, FST::RELATION('r', 1)),
			FST::NODE(1, FST::RELATION('e', 2)),
			FST::NODE(1, FST::RELATION('t', 3)),
			FST::NODE(1, FST::RELATION('u', 4)),
			FST::NODE(1, FST::RELATION('r', 5)),
			FST::NODE(1, FST::RELATION('n', 6)),
			FST::NODE()
		);

		FST::FST strmain(c,
			5,
			FST::NODE(1, FST::RELATION('m', 1)),
			FST::NODE(1, FST::RELATION('a', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE()
		);

		FST::FST strprint(c,
			6,
			FST::NODE(1, FST::RELATION('p', 1)),
			FST::NODE(1, FST::RELATION('r', 2)),
			FST::NODE(1, FST::RELATION('i', 3)),
			FST::NODE(1, FST::RELATION('n', 4)),
			FST::NODE(1, FST::RELATION('t', 5)),
			FST::NODE()
		);

		FST::FST leftbrace(c,
			2,
			FST::NODE(1, FST::RELATION('<', 1)),
			FST::NODE()
		);

		FST::FST rightbrace(c,
			2,
			FST::NODE(1, FST::RELATION('>', 1)),
			FST::NODE()
		);

		FST::FST leftthesis(c,
			2,
			FST::NODE(1, FST::RELATION('(', 1)),
			FST::NODE()
		);

		FST::FST rightthesis(c,
			2,
			FST::NODE(1, FST::RELATION(')', 1)),
			FST::NODE()
		);

		FST::FST semicolon(c,
			2,
			FST::NODE(1, FST::RELATION(';', 1)),
			FST::NODE()
		);

		FST::FST comma(c,
			2,
			FST::NODE(1, FST::RELATION(',', 1)),
			FST::NODE()
		);

		FST::FST _plus(c,
			2,
			FST::NODE(1, FST::RELATION('+', 1)),
			FST::NODE());

		FST::FST _minus(c,
			2,
			FST::NODE(1, FST::RELATION('-', 1)),
			FST::NODE());

		FST::FST divider(c,
			2,
			FST::NODE(1, FST::RELATION('/', 1)),
			FST::NODE());

		FST::FST umn(c,
			2,
			FST::NODE(1, FST::RELATION('*', 1)),
			FST::NODE());

		FST::FST FSTpercent(c,
			2,
			FST::NODE(1, FST::RELATION('%', 1)),
			FST::NODE());

		FST::FST ravno(c,
			2,
			FST::NODE(1, FST::RELATION('=', 1)),
			FST::NODE());

		FST::FST Literal_str(c,
			4,
			FST::NODE(2, FST::RELATION('\'', 1), FST::RELATION('\'', 2)),
			FST::NODE(158,
				FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1),
				FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1),
				FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1), FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1),
				FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1),
				FST::RELATION('y', 1), FST::RELATION('z', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
				FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), FST::RELATION('0', 1),
				
				FST::RELATION('а', 1), FST::RELATION('б', 1), FST::RELATION('в', 1), FST::RELATION('г', 1), FST::RELATION('д', 1), FST::RELATION('е', 1),
				FST::RELATION('ё', 1), FST::RELATION('ж', 1), FST::RELATION('з', 1), FST::RELATION('и', 1), FST::RELATION('й', 1), FST::RELATION('к', 1),
				FST::RELATION('л', 1), FST::RELATION('м', 1), FST::RELATION('н', 1), FST::RELATION('о', 1), FST::RELATION('п', 1), FST::RELATION('р', 1),
				FST::RELATION('с', 1), FST::RELATION('т', 1), FST::RELATION('у', 1), FST::RELATION('ф', 1), FST::RELATION('х', 1), FST::RELATION('ц', 1),
				FST::RELATION('ч', 1), FST::RELATION('ш', 1), FST::RELATION('щ', 1), FST::RELATION('ъ', 1), FST::RELATION('ы', 1), FST::RELATION('ь', 1),
				FST::RELATION('э', 1), FST::RELATION('ю', 1), FST::RELATION('я', 1), FST::RELATION(' ', 1), FST::RELATION('.', 1), FST::RELATION(',', 1),
				FST::RELATION('?', 1), FST::RELATION('!', 1), FST::RELATION(';', 1), FST::RELATION(':', 1), FST::RELATION('-', 1), FST::RELATION(')', 1),
				FST::RELATION('(', 1),
				
				FST::RELATION('a', 2), FST::RELATION('b', 2), FST::RELATION('c', 2), FST::RELATION('d', 2), FST::RELATION('e', 2), FST::RELATION('f', 2),
				FST::RELATION('g', 2), FST::RELATION('h', 2), FST::RELATION('i', 2), FST::RELATION('j', 2), FST::RELATION('k', 2), FST::RELATION('l', 2),
				FST::RELATION('m', 2), FST::RELATION('n', 2), FST::RELATION('o', 2), FST::RELATION('p', 2), FST::RELATION('q', 2), FST::RELATION('r', 2),
				FST::RELATION('s', 2), FST::RELATION('t', 2), FST::RELATION('u', 2), FST::RELATION('v', 2), FST::RELATION('w', 2), FST::RELATION('x', 2),
				FST::RELATION('y', 2), FST::RELATION('z', 2), FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2),
				FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2), FST::RELATION('0', 2),
				
				FST::RELATION('а', 2), FST::RELATION('б', 2), FST::RELATION('в', 2), FST::RELATION('г', 2), FST::RELATION('д', 2), FST::RELATION('е', 2),
				FST::RELATION('ё', 2), FST::RELATION('ж', 2), FST::RELATION('з', 2), FST::RELATION('и', 2), FST::RELATION('й', 2), FST::RELATION('к', 2),
				FST::RELATION('л', 2), FST::RELATION('м', 2), FST::RELATION('н', 2), FST::RELATION('о', 2), FST::RELATION('п', 2), FST::RELATION('р', 2),
				FST::RELATION('с', 2), FST::RELATION('т', 2), FST::RELATION('у', 2), FST::RELATION('ф', 2), FST::RELATION('х', 2), FST::RELATION('ц', 2),
				FST::RELATION('ч', 2), FST::RELATION('ш', 2), FST::RELATION('щ', 2), FST::RELATION('ъ', 2), FST::RELATION('ы', 2), FST::RELATION('ь', 2),
				FST::RELATION('э', 2), FST::RELATION('ю', 2), FST::RELATION('я', 2), FST::RELATION(' ', 2), FST::RELATION('.', 2), FST::RELATION(',', 2),
				FST::RELATION('?', 2), FST::RELATION('!', 2), FST::RELATION(';', 2), FST::RELATION(':', 2), FST::RELATION('-', 2), FST::RELATION(')', 2),
				FST::RELATION('(', 2)),
			FST::NODE(1, FST::RELATION('\'', 3)),
			FST::NODE()
		);

		FST::FST fstidenf(c,
			2,
			FST::NODE(62,
				FST::RELATION('a', 0), FST::RELATION('b', 0), FST::RELATION('c', 0), FST::RELATION('d', 0), FST::RELATION('e', 0), FST::RELATION('f', 0),
				FST::RELATION('g', 0), FST::RELATION('h', 0), FST::RELATION('i', 0), FST::RELATION('j', 0), FST::RELATION('k', 0), FST::RELATION('l', 0),
				FST::RELATION('m', 0), FST::RELATION('n', 0), FST::RELATION('o', 0), FST::RELATION('p', 0), FST::RELATION('q', 0), FST::RELATION('r', 0),
				FST::RELATION('s', 0), FST::RELATION('t', 0), FST::RELATION('u', 0), FST::RELATION('v', 0), FST::RELATION('w', 0), FST::RELATION('x', 0),
				FST::RELATION('y', 0), FST::RELATION('z', 0),
				
				FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1),
				FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1),
				FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1), FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1),
				FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1),
				FST::RELATION('y', 1), FST::RELATION('z', 1), FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1),
				FST::RELATION('4', 1), FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1)),
			FST::NODE()
		);

		FST::FST copystr_func(c,
			8,
			FST::NODE(1, FST::RELATION('c', 1)),
			FST::NODE(1, FST::RELATION('o', 2)),
			FST::NODE(1, FST::RELATION('p', 3)),
			FST::NODE(1, FST::RELATION('y', 4)),
			FST::NODE(1, FST::RELATION('s', 5)),
			FST::NODE(1, FST::RELATION('t', 6)),
			FST::NODE(1, FST::RELATION('r', 7)),
			FST::NODE()
		);

		FST::FST concatstr_func(c,
			10,
			FST::NODE(1, FST::RELATION('c', 1)),
			FST::NODE(1, FST::RELATION('o', 2)),
			FST::NODE(1, FST::RELATION('n', 3)),
			FST::NODE(1, FST::RELATION('c', 4)),
			FST::NODE(1, FST::RELATION('a', 5)),
			FST::NODE(1, FST::RELATION('t', 6)),
			FST::NODE(1, FST::RELATION('s', 7)),
			FST::NODE(1, FST::RELATION('t', 8)),
			FST::NODE(1, FST::RELATION('r', 9)),
			FST::NODE()
		);


		FST::FST fstgo(c,
			3,
			FST::NODE(1, FST::RELATION('g', 1)),
			FST::NODE(1, FST::RELATION('o', 2)),
			FST::NODE()
		);

		FST::FST fstcondition(c,
			3,
			FST::NODE(1, FST::RELATION('-', 1)),
			FST::NODE(1, FST::RELATION('>', 2)),
			FST::NODE()
		);

		
		FST::FST Literal_int_hex(c,
			4,
			FST::NODE(1, FST::RELATION('H', 1)),
			FST::NODE(1, FST::RELATION('x', 2)),
			
			FST::NODE(32,
				FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2), FST::RELATION('5', 2), FST::RELATION('6', 2), 
				FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2),
				FST::RELATION('A', 2), FST::RELATION('B', 2), FST::RELATION('C', 2), FST::RELATION('D', 2), FST::RELATION('E', 2), FST::RELATION('F', 2),
				FST::RELATION('0', 3), FST::RELATION('1', 3), FST::RELATION('2', 3), FST::RELATION('3', 3), FST::RELATION('4', 3), FST::RELATION('5', 3), FST::RELATION('6', 3), 
				FST::RELATION('7', 3), FST::RELATION('8', 3), FST::RELATION('9', 3),
				FST::RELATION('A', 3), FST::RELATION('B', 3), FST::RELATION('C', 3), FST::RELATION('D', 3), FST::RELATION('E', 3), FST::RELATION('F', 3)),
			FST::NODE()
		);
		

		FST::FST Literal_int_dec(c,
			2,
			FST::NODE(20,
				FST::RELATION('0', 0), FST::RELATION('1', 0), FST::RELATION('2', 0),
				FST::RELATION('3', 0), FST::RELATION('4', 0), FST::RELATION('5', 0),
				FST::RELATION('6', 0), FST::RELATION('7', 0), FST::RELATION('8', 0),
				FST::RELATION('9', 0), FST::RELATION('0', 1), FST::RELATION('1', 1),
				FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
				FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1),
				FST::RELATION('8', 1), FST::RELATION('9', 1)),
			FST::NODE()
		);

		FST::FST Literal_int_oct(c,
			4,
			FST::NODE(1, FST::RELATION('Q', 1)),
			FST::NODE(1, FST::RELATION('x', 2)),
			FST::NODE(16, FST::RELATION('0', 2), FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2),
				FST::RELATION('4', 2), FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2),
				FST::RELATION('0', 3), FST::RELATION('1', 3), FST::RELATION('2', 3), FST::RELATION('3', 3),
				FST::RELATION('4', 3), FST::RELATION('5', 3), FST::RELATION('6', 3), FST::RELATION('7', 3)),
			FST::NODE()
		);

		FST::FST Literal_int_bin(c,
			4,
			FST::NODE(1, FST::RELATION('B', 1)),
			FST::NODE(1, FST::RELATION('x', 2)),
			FST::NODE(4, FST::RELATION('0', 2), FST::RELATION('1', 2),
				FST::RELATION('0', 3), FST::RELATION('1', 3)),
			FST::NODE()
		);


		FST::FST Neg_literal_int_hex(c,
			5,
			FST::NODE(1, FST::RELATION('~', 1)),
			FST::NODE(1, FST::RELATION('H', 2)),
			FST::NODE(1, FST::RELATION('x', 3)),
			FST::NODE(32,
				FST::RELATION('0', 3), FST::RELATION('1', 3), FST::RELATION('2', 3), FST::RELATION('3', 3), FST::RELATION('4', 3), FST::RELATION('5', 3), FST::RELATION('6', 3),
				FST::RELATION('7', 3), FST::RELATION('8', 3), FST::RELATION('9', 3),
				FST::RELATION('A', 3), FST::RELATION('B', 3), FST::RELATION('C', 3), FST::RELATION('D', 3), FST::RELATION('E', 3), FST::RELATION('F', 3),
				FST::RELATION('0', 4), FST::RELATION('1', 4), FST::RELATION('2', 4), FST::RELATION('3', 4), FST::RELATION('4', 4), FST::RELATION('5', 4), FST::RELATION('6', 4),
				FST::RELATION('7', 4), FST::RELATION('8', 4), FST::RELATION('9', 4),
				FST::RELATION('A', 4), FST::RELATION('B', 4), FST::RELATION('C', 4), FST::RELATION('D', 4), FST::RELATION('E', 4), FST::RELATION('F', 4)),
			FST::NODE()
		);

		FST::FST Neg_literal_int_dec(c,
			3,
			FST::NODE(1, FST::RELATION('~', 1)),
			FST::NODE(20,
				FST::RELATION('0', 1), FST::RELATION('1', 1), FST::RELATION('2', 1),
				FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1),
				FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1),
				FST::RELATION('9', 1), FST::RELATION('0', 2), FST::RELATION('1', 2),
				FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2),
				FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2),
				FST::RELATION('8', 2), FST::RELATION('9', 2)),
			FST::NODE()
		);

		FST::FST Neg_literal_int_oct(c,
			5,
			FST::NODE(1, FST::RELATION('~', 1)),
			FST::NODE(1, FST::RELATION('Q', 2)),
			FST::NODE(1, FST::RELATION('x', 3)),
			FST::NODE(16, FST::RELATION('0', 3), FST::RELATION('1', 3), FST::RELATION('2', 3), FST::RELATION('3', 3),
				FST::RELATION('4', 3), FST::RELATION('5', 3), FST::RELATION('6', 3), FST::RELATION('7', 3),
				FST::RELATION('0', 4), FST::RELATION('1', 4), FST::RELATION('2', 4), FST::RELATION('3', 4),
				FST::RELATION('4', 4), FST::RELATION('5', 4), FST::RELATION('6', 4), FST::RELATION('7', 4)),
			FST::NODE()
		);

		FST::FST Neg_literal_int_bin(c,
			5,
			FST::NODE(1, FST::RELATION('~', 1)),
			FST::NODE(1, FST::RELATION('B', 2)),
			FST::NODE(1, FST::RELATION('x', 3)),
			FST::NODE(4, FST::RELATION('0', 3), FST::RELATION('1', 3),
				FST::RELATION('0', 4), FST::RELATION('1', 4)),
			FST::NODE()
		);


		typeint.string = c;
		if (FST::execute(typeint))
			return _TYPEINT;
		typestring.string = c;
		if (FST::execute(typestring))
			return _TYPESTRING;
		typefunction.string = c;
		if (FST::execute(typefunction))
			return _TYPEFUNCTION;
		strcreate.string = c;
		if (FST::execute(strcreate))
			return _STRCREATE;
		streturn.string = c;
		if (FST::execute(streturn))
			return _STRRETURN;
		strmain.string = c;
		if (FST::execute(strmain))
			return _STRMAIN;
		strprint.string = c;
		if (FST::execute(strprint))
			return _STRPRINT;

		leftbrace.string = c;
		if (FST::execute(leftbrace))
			return _LEFTBRACE;
		rightbrace.string = c;
		if (FST::execute(rightbrace))
			return _RIGHTBRACE;
		leftthesis.string = c;
		if (FST::execute(leftthesis))
			return _LEFTTHESIS;
		rightthesis.string = c;
		if (FST::execute(rightthesis))
			return _RIGHTTHESIS;
		rightthesis.string = c;
		if (FST::execute(rightthesis))
			return _RIGHTTHESIS;
		semicolon.string = c;
		if (FST::execute(semicolon))
			return _SEMICOLON;
		comma.string = c;
		if (FST::execute(comma))
			return _COMMA;
		_plus.string = c;
		if (FST::execute(_plus))
			return _PLUS;
		_minus.string = c;
		if (FST::execute(_minus))
			return _MINUS;
		FSTpercent.string = c;
		if (FST::execute(FSTpercent))
			return _PERCENT;
		divider.string = c;
		if (FST::execute(divider))
			return _DEL;
		umn.string = c;
		if (FST::execute(umn))
			return _UMN;
		ravno.string = c;
		if (FST::execute(ravno))
			return _RAVNO;
		ravno.string = c;
		if (FST::execute(ravno))
			return _RAVNO;
		//для цикла

		fstgo.string = c;
		if (FST::execute(fstgo))
			return _FSTGO;
		fstcondition.string = c;
		if (FST::execute(fstcondition))
			return _FSTCONDITION;
		// функции стандартной библиотеки
		copystr_func.string = c;
		if (FST::execute(copystr_func))
			return _STANDART_LIBRARY_COPYSTR;
		concatstr_func.string = c;
		if (FST::execute(concatstr_func))
			return _STANDART_LIBRARY_CONCATSTR;
		// литералы int
		Literal_int_hex.string = c;
		if (FST::execute(Literal_int_hex))
			return _LITERAL_INT_HEX;
		Literal_int_dec.string = c;
		if (FST::execute(Literal_int_dec))
			return _LITERAL_INT_DEC;
		Literal_int_oct.string = c;
		if (FST::execute(Literal_int_oct))
			return _LITERAL_INT_OCT;
		Literal_int_bin.string = c;
		if (FST::execute(Literal_int_bin))
			return _LITERAL_INT_BIN;


		Neg_literal_int_hex.string = c;
		if (FST::execute(Neg_literal_int_hex))
			return _LITERAL_INT_HEX;
		Neg_literal_int_dec.string = c;
		if (FST::execute(Neg_literal_int_dec))
			return _LITERAL_INT_DEC;
		Neg_literal_int_oct.string = c;
		if (FST::execute(Neg_literal_int_oct))
			return _LITERAL_INT_OCT;
		Neg_literal_int_bin.string = c;
		if (FST::execute(Neg_literal_int_bin))
			return _LITERAL_INT_BIN;
		// литерал string
		Literal_str.string = c;
		if (FST::execute(Literal_str))
			return _LITERAL_STR;

		fstidenf.string = c;
		if (FST::execute(fstidenf))
			return _FSTIDENF;

		return _NO_FST;
	}

	short Get_Priority(char c)
	{
		switch (c)
		{
		case LEX_LEFTHESIS:
			return PRIORITY_LEFTTHESIS;
		case LEX_RIGHTHESIS:
			return PRIORITY_RIGHTTHESIS;
		case LEX_COMMA:
			return PRIORITY_COMMA;
		case '+':
			return PRIORITY_PLUS;
		case '-':
			return PRIORITY_MINUS;
		case '*':
			return PRIORITY_STAR;
		case '/':
			return PRIORITY_DIRSLASH;
		case '%':
			return PRIORITY_PERCENT;
		}
	}
}