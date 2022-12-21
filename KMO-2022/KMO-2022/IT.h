#pragma once
#define ID_MAXSIZE		5			// максимальное кол-во символов в идентификаторе		
#define TI_MAXSIZE		4096		// максимальное кол-во строк в таблице идентификаторов
#define TI_INT_DEFAULT	0x00000000	// значение по умолчанию для типа integer
#define TI_STR_DEFAULT	0x00		// значение по умолчанию для типа string
#define TI_NULLIDX		0xffffffff	// нет элемента таблицы идентификаторов
#define TI_STR_MAXSIZE	255

namespace IT			// таблица идентификаторов
{
	enum IDDATATYPE { INT = 1, STR = 2 };			// типы данных идентификаторов: integer, string
	enum IDTYPE { V = 1, F = 2, P = 3, L = 4, OP = 5 };		// типы идентификаторов: переменная, функция, параметр, литерал, оператор


	struct Entry		// строка таблицы идентификаторов
	{
		int			idxfirstLE;			// индекс первой строки в таблице лексем
		char		id[ID_MAXSIZE*3+1];		// идентификатор (автоматически усекается до ID_MAXSIZE)
		IDDATATYPE	iddatatype;			// тип данных
		IDTYPE		idtype;				// тип идентификатора
		union
		{
			char	op;					//значение оператора
			int		vint;				// значение integer
			struct
			{
				int len;						// кол-во символов в string
				char str[TI_STR_MAXSIZE - 1];	// символы string
			}	vstr[TI_STR_MAXSIZE];			// значение string
		} value;	// значение идентификатора
	};
	struct IdTable			// экземпляр таблицы идентификаторов
	{
		int maxsize;		// емкость таблицы идентификаторов < TI_MAXSIZE
		int size;			// текущий размер таблицы идентификаторов < maxsize
		Entry* table;		// массив строк таблицы идентификаторов
	};
	IdTable Create(		// создать таблицу идентификаторов	
		int size		// емкость таблицы идентифиторов < TI_MAXSIZE
	);
	void Add(				// получить строку таблицы идентификаторов
		IdTable& idtable,	// экзмепляр таблицы идентификаторов
		Entry entry			// номер получаемой строки
	);
	Entry GetEntry(			// получить строку таблицы идентификаторов
		IdTable& idtable,	// экземпляр таблицы идентификаторов
		int n				// идентификатор
	);
	int IsId(				// возврат: номер строки (если есть), TI_NULLIDX(если нет)
		IdTable& idtable,	// экземпляр таблицы идентификаторов
		char id[ID_MAXSIZE]	// идентификатор
	);
	void Delete(IdTable& idtable);	// удалить таблицу лексем (освободить память)
};