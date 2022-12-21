#include "FST.h"
#include <iostream>
#include < string.h  >   //Для memset

namespace FST
{
	RELATION::RELATION(char c, short ns)
	{
		symbol = c;
		nnode = ns;
	};

	NODE::NODE()
	{
		n_relation = 0;
		RELATION* relations = NULL;
	};

	NODE::NODE(short n, RELATION rel, ...)
	{
		n_relation = n;
		RELATION* p = &rel;
		relations = new RELATION[n];
		for (short i = 0; i < n; i++) relations[i] = p[i];
	};

	bool step(FST& fst, short*& rstates) // один шаг автомата
	{
		bool rc = false;
		std::swap(rstates, fst.rstates);	// смена массивов
		for (short i = 0; i < fst.nstates; i++)
		{
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; j++)
				{
					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					};
				};
		};
		return rc;
	};

	bool execute(FST& fst)	// выполнить распознование цепочки
	{
		short* rstates = new short[fst.nstates];
		memset(rstates, 0xff, sizeof(short) * fst.nstates);
		short lstring = strlen(fst.string);
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++)
		{
			fst.position++;					// продвинули позицию
			rc = step(fst, rstates);	// один шаг автомата
		};
		//delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	};

	FST::FST(char* s, short ns, NODE n, ...)
	{
		string = s;
		nstates = ns;
		nodes = new NODE[ns];
		NODE* p = &n;
		for (int k = 0; k < ns; k++) nodes[k] = p[k];
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short) * nstates);
		rstates[0] = 0;
		position = -1; // чтобы с 0 начинать в execute
	};
}
