#include "stdafx.h"
#include <iostream>
#include "IT.h"
#include <stack>
#include "Semantic_analysis.h"
#include "Lexical_analysis.h"
#include "LT.h"
#include "Error.h"
using namespace std;

namespace Semantic_analysis
{
	void CheckParm(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		stack<LT::Entry> called;
		stack<LT::Entry> buf;
		stack<LT::Entry> call;
		char calledBuf[10];
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].idxTI != TI_NULLIDX && lextable.table[i].lexema[0] == LEX_ID)
			{
				if (idtable.table[lextable.table[i].idxTI].idtype == IT::F && lextable.table[i - 1].lexema[0] == LEX_FUNCTION)
				{
					while (!called.empty())
						called.pop();
					strcpy(calledBuf, idtable.table[lextable.table[i].idxTI].id);
					i++;
					while (lextable.table[i].lexema[0] != LEX_RIGHTHESIS)
					{
						if (lextable.table[i].lexema[0] == LEX_ID || lextable.table[i].lexema[0] == LEX_LITERAL)
						{
							called.push(lextable.table[i]);
						}
						i++;
					}

					for (int j = i; j < lextable.size; j++)
					{
						if (lextable.table[j].lexema[0] == LEX_ID && strcmp(idtable.table[lextable.table[j].idxTI].id, calledBuf) == 0)
						{
							j++;
							while (lextable.table[j].lexema[0] != LEX_RIGHTHESIS)
							{
								if (lextable.table[j].lexema[0] == LEX_ID || lextable.table[j].lexema[0] == LEX_LITERAL)
									call.push(lextable.table[j]);
								j++;
							}

							while (!called.empty() && !call.empty())
							{
								if (idtable.table[called.top().idxTI].iddatatype != idtable.table[call.top().idxTI].iddatatype)
								{
									throw ERROR_THROW_IN(301, lextable.table[j].sn, -1);
								}
								buf.push(called.top());
								called.pop();
								call.pop();
							}
							if (called.size() != call.size())
							{
								throw ERROR_THROW_IN(300, lextable.table[j].sn, -1);
							}
							else {
								while (!call.empty())
									call.pop();
								while (!called.empty())
									called.pop();
								while (!buf.empty())
								{
									called.push(buf.top());
									buf.pop();
								}
							}
						}
					}
				}
			}
		}
	}


	void CheckReturn(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		bool error = false;
		int buf = 0; bool findHead = false, findLocalFunc = false;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_MAINFUNC)
				findHead = true;
			if (lextable.table[i].lexema[0] == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::F && lextable.table[i - 2].lexema[0] == 't')
			{
				findLocalFunc = true;
				if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::INT)
					buf = IT::INT;
				else { buf = IT::STR; }
				continue;
			}
			if (lextable.table[i - 1].lexema[0] == LEX_RETURN)
			{
				if (findHead && !findLocalFunc)
					continue;
				if (idtable.table[lextable.table[i].idxTI].iddatatype != buf)
				{
					throw ERROR_THROW_IN(302, lextable.table[i].sn, i - 1);
					error = true;
				}
				findLocalFunc = false;
			}
		}
		// 2Часть
		int bufType = 0;
		bool findFunc = false;
		stack<LT::Entry> st;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::F && lextable.table[i - 2].lexema[0] == 't' && lextable.table[i - 1].lexema[0] == LEX_FUNCTION)
			{
				if (idtable.table[lextable.table[i].idxTI].iddatatype == IT::INT) bufType = IT::INT;
				else bufType = IT::STR;
				findFunc = true;
				continue;
			}
			if (findFunc)
			{
				if (lextable.table[i].lexema[0] == LEX_RETURN)
				{
					if (bufType == IT::INT)
					{
						while (lextable.table[i].lexema[0] != LEX_SEMICOLON)
						{
							if (lextable.table[i].lexema[0] == LEX_ID || lextable.table[i].lexema[0] == LEX_LITERAL) st.push(lextable.table[i]);
							i++;
						}
						while (!st.empty())
						{
							if (idtable.table[st.top().idxTI].iddatatype != IT::INT)
								throw ERROR_THROW_IN(302, lextable.table[i].sn, i - 1);
							st.pop();
						}
						findFunc = false;
					}
					if (bufType == IT::STR)
					{
						while (lextable.table[i].lexema[0] != LEX_SEMICOLON)
						{
							if (lextable.table[i].lexema[0] == LEX_ID || lextable.table[i].lexema[0] == LEX_LITERAL) st.push(lextable.table[i]);
							else if (lextable.table[i].lexema[0] == LEX_OPERATOR) throw ERROR_THROW_IN(312, lextable.table[i].sn, i - 1);
							i++;
						}
						while (!st.empty())
						{
							if (idtable.table[st.top().idxTI].iddatatype != IT::STR)
								throw ERROR_THROW_IN(302, lextable.table[i].sn, i - 1);
							st.pop();
						}
						findFunc = false;
					}
				}
			}
		}
	}


	void CheckAssignValue(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		bool error = false;
		stack<LT::Entry> st;
		int buf = 0; bool findFunc = false, findOperator = false;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i - 1].lexema[0] == LEX_RAV && lextable.table[i - 2].lexema[0] == LEX_ID)
			{

				buf = idtable.table[lextable.table[i - 2].idxTI].iddatatype;
				while (lextable.table[i].lexema[0] != LEX_SEPARATOR)
				{
					if (lextable.table[i].lexema[0] == LEX_OPERATOR)
						findOperator = true;
					if (lextable.table[i - 1].lexema[0] == LEX_ID && idtable.table[lextable.table[i - 1].idxTI].idtype == IT::F && lextable.table[i].lexema[0] == LEX_LEFTHESIS)
					{
						while (lextable.table[i].lexema[0] != LEX_RIGHTHESIS)
							i++;
					}
					if (lextable.table[i].lexema[0] == LEX_ID || lextable.table[i].lexema[0] == LEX_LITERAL)
						st.push(lextable.table[i]);
					i++;
				}
				while (!st.empty())
				{
					if (buf == IT::STR && findOperator)
						throw ERROR_THROW_IN(304, lextable.table[i].sn, -1);

					if (idtable.table[st.top().idxTI].iddatatype != buf)
					{
						if (findOperator)
						{
							throw ERROR_THROW_IN(304, lextable.table[i].sn, -1);

						}
						else {
							throw ERROR_THROW_IN(303, lextable.table[i].sn, 3);
						}
					}
					st.pop();
				}
				findOperator = false;
			}
		}
	}

	void CheckAllowedValue(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		bool findStr = false, findOp = false;

		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_LITERAL && idtable.table[lextable.table[i].idxTI].iddatatype == IT::INT
				&& (idtable.table[lextable.table[i].idxTI].value.vint == 2147483647 || idtable.table[lextable.table[i].idxTI].value.vint == -2147483648))
				throw ERROR_THROW_IN(305, lextable.table[i].sn, i);
		}
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_RAV)
			{
				findStr = false;
				while (lextable.table[i].lexema[0] != LEX_SEPARATOR)
				{
					if (lextable.table[i].lexema[0] == LEX_ID && idtable.table[lextable.table[i].idxTI].iddatatype == IT::STR)
						findStr = true;
					if (findStr && lextable.table[i].lexema[0] == LEX_OPERATOR && idtable.table[lextable.table[i].idxTI].value.op != '+')
						throw ERROR_THROW_IN(306, lextable.table[i].sn, -1);
					i++;
				}
			}
			if (lextable.table[i].lexema[0] == 'i' && lextable.table[i].idxTI == TI_NULLIDX && lextable.table[i - 1].lexema[0] != 't' && lextable.table[i - 1].lexema[0] != 'f')
				throw ERROR_THROW_IN(307, lextable.table[i].sn, -1);
		}
	}

	void CheckFunc(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_ID && (idtable.table[lextable.table[i].idxTI].id == STANDART1 || idtable.table[lextable.table[i].idxTI].id == STANDART2))
			{
				int j = i;
				while (j >= i - 3)
				{
					if (lextable.table[i].lexema[0] == LEX_CREATE || lextable.table[i].lexema[0] == LEX_FUNCTION || lextable.table[i].lexema[0] == 't')
						throw ERROR_THROW_IN(308, lextable.table[i].sn, -1);
					j--;
				}
			}
		}
	}
	
	void CheckRecursion(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		char tempFunc[10];
		bool findFunc = false;
		bool checkRet = false;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_RETURN) checkRet = true;
			if (lextable.table[i].lexema[0] == LEX_ID && lextable.table[i - 1].lexema[0] == LEX_FUNCTION && idtable.table[lextable.table[i].idxTI].idtype == IT::F)
			{
				findFunc = true;
				strcpy_s(tempFunc, idtable.table[lextable.table[i].idxTI].id);
				continue;
			}
			if (findFunc)
			{
				if (lextable.table[i].lexema[0] == LEX_ID)
				{
					if (strcmp(idtable.table[lextable.table[i].idxTI].id, tempFunc) == 0)
					{
						throw ERROR_THROW_IN(311, lextable.table[i].sn, -1);
					}
				}

				if (checkRet && (lextable.table[i].lexema[0] == LEX_SEMICOLON && lextable.table[i - 1].lexema[0] == LEX_BRACELET))
				{
					memset(tempFunc, '\0', strlen(tempFunc));
					checkRet = false;
					findFunc = false;
				}
			}
		}
	}
	void CheckPrint(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		IT::IDDATATYPE bufType{};
		stack<LT::Entry> st;
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_PRINT)
			{
				i++;
				while (lextable.table[i].lexema[0] != LEX_SEMICOLON)
				{
					if (lextable.table[i].lexema[0] == LEX_ID || lextable.table[i].lexema[0] == LEX_LITERAL || lextable.table[i].lexema[0] == LEX_OPERATOR) st.push(lextable.table[i]);
					i++;
				}
				if (!st.empty())
				{
					bufType = idtable.table[st.top().idxTI].iddatatype;
				}
				if (bufType == IT::INT)
				{
					while (!st.empty())
					{
						if (st.top().idxTI != (int)TI_NULLIDX && idtable.table[st.top().idxTI].iddatatype != IT::INT) throw ERROR_THROW_IN(304, lextable.table[i].sn, i - 1);
						st.pop();
					}
				}
				if (bufType == IT::STR)
				{

					while (!st.empty())
					{
						if(st.top().lexema[0] == LEX_OPERATOR) throw ERROR_THROW_IN(312, lextable.table[i].sn, i - 1);
						if (st.top().idxTI != (int)TI_NULLIDX && idtable.table[st.top().idxTI].iddatatype != IT::STR) throw ERROR_THROW_IN(304, lextable.table[i].sn, i - 1);
						st.pop();
					}
				}
			}
		}
	}
	void CheckStaticParm(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		for (int i = 0; i < lextable.size; i++)
		{
			if (lextable.table[i].lexema[0] == LEX_ID && idtable.table[lextable.table[i].idxTI].idtype == IT::F)
			{
				if (strcmp(idtable.table[lextable.table[i].idxTI].id, "copystr") == 0 || strcmp(idtable.table[lextable.table[i].idxTI].id, "concatstr") == 0)
				{
					if (idtable.table[lextable.table[i+2].idxTI].iddatatype == IT::STR && idtable.table[lextable.table[i+4].idxTI].iddatatype == IT::STR && lextable.table[i+5].lexema[0] == LEX_RIGHTHESIS) continue;
					else throw ERROR_THROW_IN(313, lextable.table[i].sn, i - 1);
				}
			}
		}
	}

	void Semantic_analysis(Log::LOG log, LT::LexTable lextable, IT::IdTable idtable)
	{
		CheckAssignValue(log, lextable, idtable);
		CheckAllowedValue(log, lextable, idtable);
		CheckParm(log, lextable, idtable);
		CheckFunc(log, lextable, idtable);
		CheckRecursion(log, lextable, idtable);
		CheckStaticParm(log, lextable, idtable);
		CheckPrint(log, lextable, idtable);
		CheckReturn(log, lextable, idtable);
	}
}