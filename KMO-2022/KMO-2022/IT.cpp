#include "stdafx.h"
#include "IT.h"
#include "Error.h"

namespace IT
{
	IdTable Create(int size)
	{
		IdTable* tabl = new IdTable;
		if (size > TI_MAXSIZE)
		{
			throw ERROR_THROW(115);
		}
		tabl->maxsize = size;
		tabl->size = 0;
		tabl->table = new Entry[size];
		return *tabl;
	}

	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size + 1 > idtable.maxsize)
		{
			throw ERROR_THROW(116);
		}

		idtable.table[idtable.size] = entry;
		idtable.size += 1;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE * 2])
	{
		int n = 0;
		bool flag = false;
		while (n < idtable.size)
		{
			if (strcmp(idtable.table[n].id, id) == 0)
			{
				flag = true;
				break;
			}
			n += 1;
		}
		if (flag)
			return n;
		else
			return TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
		delete& idtable;
	}
};