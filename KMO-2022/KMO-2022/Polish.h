#pragma once
#include "Lexical_analysis.h"
#include "LT.h"

namespace Polish {
	bool PolishNotation(int i, LT::LexTable& lextable, IT::IdTable& idtable);
	void PolishStart(Lex_analysis::LEX lex);
}