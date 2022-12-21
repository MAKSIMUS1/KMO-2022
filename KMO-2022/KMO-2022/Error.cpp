#include "stdafx.h"
#include "Error.h"
namespace Error {
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустим код ошибки"),
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY(100, "Параметр -in должен быть задан"),
		ERROR_ENTRY_NODEF(101),ERROR_ENTRY_NODEF(102),ERROR_ENTRY_NODEF(103),
		ERROR_ENTRY(104, "Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(105),ERROR_ENTRY_NODEF(106),ERROR_ENTRY_NODEF(107),
		ERROR_ENTRY_NODEF(108),ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY(113, "<Lexical>: Превышено максимальное количество строк в таблице лексем"),
		ERROR_ENTRY(114, "<Lexical>: Превышено максимальное количество строк в таблице лексем"),
		ERROR_ENTRY(115, "<Lexical>: Превышено максимальное количество строк в таблице идентификаторов"),
		ERROR_ENTRY(116, "<Lexical>: Превышено максимальное количество строк в таблице идентификаторов"),
		ERROR_ENTRY(117, "<Lexical>: Лексема не была распознана"),
		ERROR_ENTRY(118, "<Lexical>: Превышена длина названия идентификатора"),
		ERROR_ENTRY(119, "<Lexical>: Превышена длина лексемы"),
		ERROR_ENTRY_NODEF10(120),ERROR_ENTRY_NODEF10(130),ERROR_ENTRY_NODEF10(140),ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160),ERROR_ENTRY_NODEF10(170),ERROR_ENTRY_NODEF10(180),ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY_NODEF100(200),
		ERROR_ENTRY(300, "<Semantic>: Количество параметров функции не совпадает."),
		ERROR_ENTRY(301, "<Semantic>: Типы формальных и фактических параметров функции не совпадают."),
		ERROR_ENTRY(302, "<Semantic>: Тип функции и тип возвращаемого значения не совпадают."),
		ERROR_ENTRY(303, "<Semantic>: Недопустимый тип присваиваемого значения."),
		ERROR_ENTRY(304, "<Semantic>: Операции над аргументами разных типов недопустимы."),
		ERROR_ENTRY(305, "<Semantic>: Число выходит за границы допустимого диапазона."),
		ERROR_ENTRY(306, "<Semantic>: Недопустимые операции со строками."),
		ERROR_ENTRY(307, "<Semantic>: Идентификатор не может быть использован до его объявления."),
		ERROR_ENTRY(308, "<Semantic>: Стандартные функции не требуют объявления."),
		ERROR_ENTRY(309, "<Semantic>: Использование необявленного идентификатора."),
		ERROR_ENTRY(310, "<Semantic>: Повторное обьявление идентификатора."),
		ERROR_ENTRY(311, "<Semantic>: Нельзя вызывать функцию внутри этой же функции."),
		ERROR_ENTRY(312, "<Semantic>: Операции над типом string недопустимы."),
		ERROR_ENTRY(313, "<Semantic>: Ошибка в параметрах статической функции."), 
		ERROR_ENTRY(314, "<Semantic>: Использование пустой строки запрещенно."), 
		ERROR_ENTRY(315, "<Semantic>: Запрещенно в возврате функции вызывать функцию."), 
		ERROR_ENTRY_NODEF(316),
		ERROR_ENTRY_NODEF(317), ERROR_ENTRY_NODEF(318), ERROR_ENTRY_NODEF(319),		
		ERROR_ENTRY_NODEF10(320),ERROR_ENTRY_NODEF10(330),ERROR_ENTRY_NODEF10(340),ERROR_ENTRY_NODEF10(350),
		ERROR_ENTRY_NODEF10(360),ERROR_ENTRY_NODEF10(370),ERROR_ENTRY_NODEF10(380),ERROR_ENTRY_NODEF10(390),
		ERROR_ENTRY_NODEF100(400),ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "<Syntactic>: Неверная структура программы."),
		ERROR_ENTRY(601, "<Syntactic>: Ошибочный оператор."),
		ERROR_ENTRY(602, "<Syntactic>: Ошибка в выражении."),
		ERROR_ENTRY(603, "<Syntactic>: Ошибка в параметрах функции."),
		ERROR_ENTRY(604, "<Syntactic>: Ошибка в параметрах вызываемой функции."),
		ERROR_ENTRY(605, "<Syntactic>: Ошибка в цикле."),
		ERROR_ENTRY(606, "<Syntactic>: Ошибка на правильность параметров вызываемой функции."),
		ERROR_ENTRY(607, "<Syntactic>: Ошибка на правильность тела цикла."),
		ERROR_ENTRY(608, "<Syntactic>: Ошибка в return."),
		ERROR_ENTRY(609, "<Syntactic>: Синтаксическая ошибка. Побробнее в log."),
		ERROR_ENTRY_NODEF10(610),ERROR_ENTRY_NODEF10(620),ERROR_ENTRY_NODEF10(630),ERROR_ENTRY_NODEF10(640),ERROR_ENTRY_NODEF10(650),ERROR_ENTRY_NODEF10(660),ERROR_ENTRY_NODEF10(670),ERROR_ENTRY_NODEF10(680),ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700),ERROR_ENTRY_NODEF100(800),ERROR_ENTRY_NODEF100(900)
	};

	ERROR geterror(int id)
	{
		if (id > 0 && id < ERROR_MAX_ENTRY)
			return errors[id];
		else
			return errors[0];
	}
	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		if (id > 0 && id < ERROR_MAX_ENTRY) {
			errors[id].inext.col = col;
			errors[id].inext.line = line;
			return errors[id];
		}
		else
			return errors[0];
	}
}