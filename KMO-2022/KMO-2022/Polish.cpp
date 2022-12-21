#include "stdafx.h"
#include "Lexical_analysis.h"
#include <stack>
#include <queue>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdlib>

using namespace std;

namespace Polish {
	bool PolishNotation(int i, LT::LexTable& lextable, IT::IdTable& idtable)
	{
		stack<LT::Entry> stack;		// стек для операций
		queue<LT::Entry> queue;		// очередь для операндов
		LT::Entry temp;
		temp.idxTI = -1;
		temp.lexema[0] = '#';
		temp.lexema[1] = '\0';
		temp.idxTI = (int)TI_NULLIDX;
		temp.sn = -1;
		LT::Entry func;
		func.lexema[0] = '@';
		func.lexema[1] = '\0';
		func.idxTI = (int)TI_NULLIDX;
		LT::Entry commas;
		commas.idxTI = (int)TI_NULLIDX;
		int countComma = 0;			//подсчет количества запятых
		char* comma = new char[1] { "" };
		int countLex = 0;			// количество преобразованных лексем
		int posLex = i;				// запоминаем номер лексемы перед преобразованием
		bool findFunc = false;


		for (i; lextable.table[i].lexema[0] != LEX_SEPARATOR; i++, countLex++)
		{
			switch (lextable.table[i].lexema[0])
			{
			case LEX_ID:	// если идентификатор
			{
				queue.push(lextable.table[i]);   // помещаем в очередь
				continue;
			}
			case LEX_LITERAL:		// если литерал
			{
				queue.push(lextable.table[i]);	// помещаем в очередь
				continue;
			}
			case LEX_LEFTHESIS:		// если (
			{
				if (lextable.table[i - 1].idxTI != (int)TI_NULLIDX && idtable.table[lextable.table[i - 1].idxTI].idtype == IT::F)
					findFunc = true;
				stack.push(lextable.table[i]); // помещаем ее в стек
				continue;
			}
			case LEX_RIGHTHESIS:	// если )
			{
				if (findFunc)
				{
					itoa(++countComma, comma, 10);
					strcpy(commas.lexema, comma);
					stack.push(commas);
					stack.push(func);
					findFunc = false;
				}
				while (stack.top().lexema[0] != LEX_LEFTHESIS)	// пока не встретим (
				{
					queue.push(stack.top());	// выталкиваем из стека в очередь
					stack.pop();

					if (stack.empty())
						return false;
				}
				stack.pop();	// уничтожаем (
				continue;
			}
			case LEX_OPERATOR:	// если знак оператора
			{
				while (!stack.empty() && lextable.table[i].priority <= stack.top().priority)
					// пока приоритет текущего оператора 
					//меньше или равен приоритету оператора в вершине стека
				{
					queue.push(stack.top());	// выталкиваем со стека в выходную строку
					stack.pop();
				}
				stack.push(lextable.table[i]);
				continue;
			}
			case LEX_COMMA:
			{
				countComma++;
				continue;
			}
			}
		}
		while (!stack.empty())	// если стек не пустой
		{
			if (stack.top().lexema[0] == LEX_LEFTHESIS || stack.top().lexema[0] == LEX_RIGHTHESIS)
				return false;
			queue.push(stack.top());	// выталкиваем все в очередь
			stack.pop();
		}
		while (countLex != 0)		// замена текущего выражения в таблице лексем на выражение в ПОЛИЗ
		{
			if (!queue.empty()) {
				lextable.table[posLex++] = queue.front();
				//cout << lex.idtable.table[queue.front().idxTI].id;	// вывод в консоль
				queue.pop();
			}
			else
			{
				lextable.table[posLex++] = temp;			// может лучше перезаписать таблицу?
			}
			countLex--;
		}

		for (int i = 0; i < posLex; i++)		// восстановление индексов первого вхождения 
			//в таблицу лексем у операторов из таблицы идентификаторов
		{
			if (lextable.table[i].lexema[0] == LEX_OPERATOR || lextable.table[i].lexema[0] == LEX_LITERAL)
				idtable.table[lextable.table[i].idxTI].idxfirstLE = i;
		}
		return true;
	}

	void PolishStart(Lex_analysis::LEX lex)
	{
		for (int i = 0; i < lex.lextable.size; i++)
		{
			if (lex.lextable.table[i].lexema[0] == '=')
			{
				PolishNotation(i + 1, lex.lextable, lex.idtable);
			}
		}
	}
}