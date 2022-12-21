#include "stdafx.h"
#include "GRB.h"
#include <iostream>
#define GRB_ERROR_SERIES 600

namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)

	Greibach greibach(
		NS('S'),
		TS('$'),
		10,
		// Стартовые правила, описывающее общую структуру программы
		Rule(NS('S'), GRB_ERROR_SERIES + 0,
			///	S -> m<N>;
			/// S -> tfiP<NrX;>;S
			2,
			Rule::Chain(5, TS('m'), TS('<'), NS('N'), TS('>'), TS(';')),
			Rule::Chain(12, TS('t'), TS('f'), TS('i'), NS('P'), TS('<'), NS('N'), TS('r'), NS('X'), TS(';'), TS('>'), TS(';'), NS('S'))
		),
		// Правила для конструкций в функциях
		Rule(NS('N'), GRB_ERROR_SERIES + 1,
			/// N -> cti;
			/// N -> cti;N
			/// N -> cti=E;
			/// N -> cti=E;N
			/// N -> pE;
			/// N -> pE;N
			/// N -> i=E;
			/// N -> i=E;N
			/// N -> gZ;
			/// N -> gZ;N
			10,
			Rule::Chain(4, TS('c'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(5, TS('c'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(6, TS('c'), TS('t'), TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(7, TS('c'), TS('t'), TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(3, TS('g'), NS('Z'), TS(';')),
			Rule::Chain(4, TS('g'), NS('Z'), TS(';'), NS('N'))
		),
		// Правила для выражений
		Rule(NS('E'), GRB_ERROR_SERIES + 2,
			/// E -> i
			/// E -> l
			/// E -> (E)
			/// E -> (E)M
			/// E -> i(W)
			/// E -> i()
			/// E -> i()M
			/// E -> iM
			/// E -> i(W)M
			/// E -> lM
			10,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(3, TS('i'), TS('('), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), TS(')'), NS('M')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M'))
		),
		// Проверка наличия параметров функции
		Rule(NS('P'), GRB_ERROR_SERIES + 3,
			2,
			/// P -> (F)
			/// P -> ()
			Rule::Chain(3, TS('('), NS('F'), TS(')')),
			Rule::Chain(2, TS('('), TS(')'))
		),
		// Проверка на правильность параметров функции при её объявлении
		Rule(NS('F'), GRB_ERROR_SERIES + 4,
			/// F -> ti
			/// F -> ti,F
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		// Проверка на правильность арифметических действий
		Rule(NS('M'), GRB_ERROR_SERIES + 5,
			1,
			/// M -> vE
			Rule::Chain(2, TS('v'), NS('E'))
		),
		// Проверка на правильность параметров вызываемой функции
		Rule(NS('W'), GRB_ERROR_SERIES + 6,
			4,
			/// W -> i
			/// W -> l
			/// W -> i,W
			/// W -> l,W
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		),
		// Проверка на правильность конструкции в цикле
		Rule(NS('Z'), GRB_ERROR_SERIES + 7,
			1,
			/// Z -> iul;<J>
			Rule::Chain(7, TS('i'), TS('u'), TS('l'), TS(';'), TS('<'), NS('J'), TS('>'))
		),
		// Правила для конструкций в теле функций
		Rule(NS('J'), GRB_ERROR_SERIES + 7,
			/// J -> pE;
			/// J -> pE;J
			/// J -> i=E;
			/// J -> i=E;J
			4,
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('J')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('J'))
		),
		// Проверка на для выражений в return
		Rule(NS('X'), GRB_ERROR_SERIES + 8,
			2,
			/// X -> l
			/// X -> i
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		)
	);


	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)	///Ïðåäñò. öåïî÷êè (ïðàâàÿ ñòîðîíà ïðàâèëà)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i)
		{
			nt[i] = (GRBALPHABET)p[i];
		}
	};
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)	///Ïðàâèëî â ãðàììàòèêå Ãðåéáàõ
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	};
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	};
	Greibach getGreibach()	///Ïîëó÷èòü ãðàììàòèêó
	{
		return greibach;
	};
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)	///Ïîëó÷èòü ¹ ïðàâèëà èëè -1
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
		{
			k++;
		}
		if (k < size) prule = rules[rc = k];
		return rc;
	};
	Rule Greibach::getRule(short n)	///Ïîëó÷èòü ïðàâèëî ïî íîìåðó
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	};
	char* Rule::getCRule(char* b, short nchain)	///ïîëó÷èòü ïðàâèëî â âèäå N->öåïî÷êà
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) ///Âåðíóòü ¹ ñëåä. çà j öåïî÷êè èëè -1
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t) ++j;
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	};
	char* Rule::Chain::getCChain(char* b)	///ïîëó÷èòü öåïî÷êó â ñèìâ. âèäå
	{
		for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};
};