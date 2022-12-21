#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"
#include "Log.h"

namespace Semantic_analysis
{
	void CheckParm(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);			// �������� ���������� ������� �� ���������� �� ���� � ����������
	void CheckReturn(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);			// �������� ���������� ���� ������� � ���� ������������� ��������
	void CheckAssignValue(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);	// �������� ������������ ���� ���������� � �������������� ��������
	void CheckAllowedValue(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);	// �������� ������������� �������� �������� �� ������������
	void CheckFunc(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);			// �������� �������
	void CheckRecursion(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);		// �������� ������ �� ��������
	void CheckPrint(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);			// �������� �� print
	void CheckStaticParm(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);		// �������� ������ �� ��������� ����������� �������
	void Semantic_analysis(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable);	// ������������� ������
}