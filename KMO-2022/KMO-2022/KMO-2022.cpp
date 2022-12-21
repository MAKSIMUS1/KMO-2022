#include "stdafx.h"
#include <iostream>
#include <locale>
#include <cwchar>

#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "Lexical_analysis.h"
#include "MFST.h"
#include "Semantic_analysis.h"
#include "Polish.h"
#include "Generation.h"

#define FILE "D:\\BSTU\\Kurs_2_1\\KMO-2022\\KMO-2022\\AsmCode\\AsmCode.asm"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");

	Log::LOG	log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
		Lex_analysis::LEX lex = Lex_analysis::Lexical_analysis(in);
		// ------------- Lexical analysis -------------

		Lex_analysis::LTLog(lex.lextable, log);
		Lex_analysis::ITLog(lex.idtable, log);
		MFST::SyntaxCheck(lex, log, *log.stream);
		// ------------- Syntax analysis -------------

		Semantic_analysis::Semantic_analysis(log, lex.lextable, lex.idtable);
		// ------------- Semantic analysis -------------
		
		CG::ConstSegment CnstS(lex.idtable.size);
		CG::DataSegment Data(lex.idtable.size);
		CG::CodeSegment Code;
		CG::add(lex.idtable, CnstS);
		CG::add(lex.idtable, Data);
		CG::CodeSegmentGeneration(Code, lex.lextable, lex.idtable);
		CG::startGeneration(parm.out, CnstS, Data, Code);
		CG::startGeneration(FILE, CnstS, Data, Code);
		Lex_analysis::LTLogPolska(lex.lextable, log);
		// ------------- Вывод польской записи -------------
		std::cout << "Генерация кода выполнена успешно." << std::endl;
		
		Log::Close(log);
	}
	catch (Error::ERROR e) {
		std::cout << "Ошибка " << e.id << ": " << e.message << std::endl << std::endl;
	};
	return 0;
}

