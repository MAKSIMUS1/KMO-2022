#pragma once
#define LEXEMA_FIXSIZE	1		// ������������� ������ �������
#define LT_MAXSIZE		4096	// ������������ ���������� ����� � ������� ������
#define LT_TI_NULLIDX	0xffffffff	// ��� �������� ������� ���������������
#define LEX_INTEGER		't'	//	������� ��� integer
#define LEX_STRING		't'	//	������� ��� string
#define LEX_LITERAL		'l'	//	������� ��� ��������
#define LEX_ID			'i'	//	������� ��� ��������������
#define LEX_FUNCTION	'f'	//	������� ��� function
#define LEX_MAINFUNC	'm'	//	������� ��� mainfunc
#define LEX_CREATE		'c'	//	������� ��� create
#define LEX_RETURN		'r'	//	������� ��� return
#define LEX_PRINT		'p'	//	������� ��� print
#define LEX_SEMICOLON	';'	//	������� ��� ;
#define LEX_SEPARATOR   ';'	//	������� ��� ;
#define LEX_COMMA		','	//	������� ��� ,
#define LEX_LEFTBRACE	'<'	//	������� ��� <
#define LEX_BRACELET	'>'	//	������� ��� >
#define LEX_LEFTHESIS	'('	//	������� ��� (
#define LEX_RIGHTHESIS	')'	//	������� ��� )
#define LEX_PLUS		'v'	//	������� ��� +
#define LEX_MINUS		'v'	//	������� ��� -
#define LEX_STAR		'v'	//	������� ��� *
#define LEX_DIRSLASH	'v'	//	������� ��� /
#define LEX_PERCENT		'v' //  ������� ��� % 
#define LEX_RAV			'='	//	������� ��� =
#define LEX_CONDITION	'u'	//	������� ��� -> condition = �������
#define LEX_GO			'g'	//	������� ��� go
#define LEX_OPERATOR	'v'	//	������� ��� ����������

namespace LT		// ������� ������
{
	struct Entry	// ������ ������� ������
	{
		char lexema[LEXEMA_FIXSIZE];	// �������
		int sn;							// ����� ������ � �������� ������
		int idxTI;						// ������ � ������� ��������������� ��� LT_TINULLIDX
		int priority;					// ��������� ���������
	};

	struct LexTable			// ��������� ������� ������
	{
		int maxsize;		// ������� ������� ������ < LT_MAXSIZE
		int size;			// ������� ������ ������� ������ < maxsize 
		Entry* table;		// ������ ����� ������� ������
	};

	LexTable Create(	// ������� ������� ������
		int size		// ������� ������� ����� < LT_MAXSIZE
	);
	void Add(			// �������� ������ � ������� ������
		LexTable& lextable,		// ��������� ������� ������
		Entry entry				// ������ ������� ������
	);
	Entry GetEntry(			// �������� ������ ������� ������
		LexTable& lextable,	// ��������� ������� ������
		int n				// ����� ���������� ������
	);
	void Delete(LexTable& lextable);	// ������� ������� ������ (���������� ������)
};