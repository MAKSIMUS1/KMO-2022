#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"
#include "Log.h"

namespace Semantic_analysis
{
	void CheckParm(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);			// Проверка параметров функции на совпадение по типу и количеству
	void CheckReturn(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);			// Преверка совпадения типа функции и типа возвращаемого значения
	void CheckAssignValue(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);	// Проверка соответствия типа переменной и присваиваемого значения
	void CheckAllowedValue(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);	// Проверка присваиваемых числовых значений на допустимость
	void CheckFunc(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);			// Проверка функций
	void CheckRecursion(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);		// Проверка функци на рекурсию
	void CheckPrint(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);			// Проверка на print
	void CheckStaticParm(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);		// Проверка функци на параметры статических функций
	void Semantic_analysis(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);	// Семантический анализ
}