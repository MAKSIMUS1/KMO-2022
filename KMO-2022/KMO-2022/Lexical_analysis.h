#pragma once
#include "IT.h"
#include "LT.h"
#include "In.h"
#include "Parm.h"
#include "Log.h"

#define STANDART1 "concatstr"
#define STANDART2 "copystr"

namespace Lex_analysis
{
	enum operations_priority
	{
		PRIORITY_LEFTTHESIS = 0,	// (
		PRIORITY_RIGHTTHESIS = 0,	// )
		PRIORITY_COMMA = 1,			// ,
		PRIORITY_PLUS = 2,			// +
		PRIORITY_MINUS = 2,			// -
		PRIORITY_STAR = 3,			// *
		PRIORITY_DIRSLASH = 3,		// /
		PRIORITY_PERCENT = 3,		// %
	};
	enum TYPEOFFST
	{
		_TYPEINT = 1, 
		_TYPESTRING, 
		_TYPEFUNCTION,
		_STRCREATE, 
		_STRRETURN, 
		_STRMAIN,
		_STRPRINT, 
		_LEFTBRACE,
		_RIGHTBRACE, 
		_LEFTTHESIS,
		_RIGHTTHESIS, 
		_SEMICOLON, 
		_COMMA,
		_PLUS, 
		_MINUS, 
		_DEL,
		_UMN,
		_PERCENT,
		_RAVNO, 
		_LITERAL_INT_HEX,
		_LITERAL_INT_DEC,
		_LITERAL_INT_OCT,
		_LITERAL_INT_BIN,
		_LITERAL_STR,
		_FSTIDENF,
		_FSTGO,
		_FSTCONDITION,
		_STANDART_LIBRARY_COPYSTR,
		_STANDART_LIBRARY_CONCATSTR,

		_NO_FST
	};
	struct LEX_FLAGS
	{
		bool __flag_int = false;				// (типы данных идентификаторов ) флаг integer
		bool __flag_string = false;				// (типы данных идентификаторов ) флаг string
		bool __flag_function = false;			// ( типы идентификаторов ) флаг функции
		bool __flag_parameter = false;			// ( типы идентификаторов ) флаг параметр
		bool __flag_standard_library = false;	// функции стандартой библиотеки
		bool __flag_function_external = false;	// костыль
		bool __flag_declare = false;			// флаг declare
		bool __flag_operator = false;			// флаг оператора
		bool __flag_return = false;				// флаг return
		bool __flag_comment = false;			// флаг комметария
	};

	struct LEX {
		IT::IdTable idtable;
		LT::LexTable lextable;
	};

	TYPEOFFST Proverka_FST(char* c);
	void Add_To_LT(int line, char c);
	LEX Lexical_analysis(In::IN in);
	short Get_Priority(char c);
	void LTLog(LT::LexTable lt, Log::LOG log);
	void ITLog(IT::IdTable it, Log::LOG log);
	void LTLogPolska(LT::LexTable lt, Log::LOG log);
};