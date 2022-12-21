#include "stdafx.h"
#include <iostream>
#include "LT.h"
#include "Error.h"

namespace LT
{
	LexTable Create(int size)
	{
		LexTable* tabl = new LexTable;
		if (size > LT_MAXSIZE)
		{
			throw ERROR_THROW(113);
		}
		tabl->maxsize = size;
		tabl->size = 0;
		tabl->table = new Entry[size];
		return *tabl;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size + 1 > lextable.maxsize)
		{
			throw ERROR_THROW(114);
		}

		lextable.table[lextable.size] = entry;
		lextable.size += 1;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		return lextable.table[n];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
		delete& lextable;
	}
};