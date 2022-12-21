#pragma once
#define LEXEMA_FIXSIZE	1		// фиксированный размер лексемы
#define LT_MAXSIZE		4096	// максимальное количество строк в таблице лексем
#define LT_TI_NULLIDX	0xffffffff	// нет элемента таблицы идентификаторов
#define LEX_INTEGER		't'	//	лексема для integer
#define LEX_STRING		't'	//	лексема для string
#define LEX_LITERAL		'l'	//	лексема для литерала
#define LEX_ID			'i'	//	лексема для идентификатора
#define LEX_FUNCTION	'f'	//	лексема для function
#define LEX_MAINFUNC	'm'	//	лексема для mainfunc
#define LEX_CREATE		'c'	//	лексема для create
#define LEX_RETURN		'r'	//	лексема для return
#define LEX_PRINT		'p'	//	лексема для print
#define LEX_SEMICOLON	';'	//	лексема для ;
#define LEX_SEPARATOR   ';'	//	лексема для ;
#define LEX_COMMA		','	//	лексема для ,
#define LEX_LEFTBRACE	'<'	//	лексема для <
#define LEX_BRACELET	'>'	//	лексема для >
#define LEX_LEFTHESIS	'('	//	лексема для (
#define LEX_RIGHTHESIS	')'	//	лексема для )
#define LEX_PLUS		'v'	//	лексема для +
#define LEX_MINUS		'v'	//	лексема для -
#define LEX_STAR		'v'	//	лексема для *
#define LEX_DIRSLASH	'v'	//	лексема для /
#define LEX_PERCENT		'v' //  лексема для % 
#define LEX_RAV			'='	//	лексема для =
#define LEX_CONDITION	'u'	//	лексема для -> condition = условие
#define LEX_GO			'g'	//	лексема для go
#define LEX_OPERATOR	'v'	//	лексема для операторов

namespace LT		// таблицы лексем
{
	struct Entry	// строка таблицы лексем
	{
		char lexema[LEXEMA_FIXSIZE];	// лексема
		int sn;							// номер строки в исходном тексте
		int idxTI;						// индекс в таблице идентификаторов или LT_TINULLIDX
		int priority;					// приоритет оператора
	};

	struct LexTable			// экземпляр таблицы лексем
	{
		int maxsize;		// емкость таблицы лексем < LT_MAXSIZE
		int size;			// текущий размер таблицы лексем < maxsize 
		Entry* table;		// массив строк таблицы лексем
	};

	LexTable Create(	// создать таблицу лексем
		int size		// емкость таблицы лекем < LT_MAXSIZE
	);
	void Add(			// добавить строку в таблицу лексем
		LexTable& lextable,		// экземпляр таблицы лексем
		Entry entry				// строка таблицы лексем
	);
	Entry GetEntry(			// получить строку таблицы лексем
		LexTable& lextable,	// экземпляр таблицы лексем
		int n				// номер получаемой строки
	);
	void Delete(LexTable& lextable);	// удалить таблицу лексем (освободить память)
};