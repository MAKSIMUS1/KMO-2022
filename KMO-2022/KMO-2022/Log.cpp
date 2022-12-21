#include "stdafx.h"	
#include "Log.h"	
#include <time.h>
#include <iostream>
#include <iomanip>

using namespace std;

namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (log.stream->fail())
			throw ERROR_THROW(112);
		wcscpy_s(log.logfile, logfile);
		return log;
	}
	void WriteLine(LOG log, const char* c, ...)
	{
		const char** ptr = &c;
		int i = 0;
		while (ptr[i] != "")
			*log.stream << ptr[i++];
		*log.stream << endl;
	}
	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		const wchar_t** ptr = &c;
		char temp[100];
		int i = 0;
		while (ptr[i] != L"")
		{
			wcstombs(temp, ptr[i++], sizeof(temp));
			*log.stream << temp;
		}
		*log.stream << endl;
	}
	void WriteLog(LOG log)
	{
		tm* timeinf;
		time_t timet;
		time(&timet);
		timeinf = localtime(&timet);
		char temp[100];
		strftime(temp, sizeof(temp), "\n------------ПРОТОКОЛ------------\n ----%d.%m.%y %T----\n ", timeinf);
		*log.stream << temp;
	}
	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "\n---- Исходные данные ----\n" << in.text
			<< "\n\nВсего символов: " << in.size << endl
			<< "Всего строк: " << in.lines << endl
			<< "Пропущено: " << in.ignor << endl;
	}
	void WriteParm(LOG log, Parm::PARM parm)
	{
		char in_text[PARM_MAX_SIZE];
		char out_text[PARM_MAX_SIZE];
		char log_text[PARM_MAX_SIZE];
		wcstombs(in_text, parm.in, PARM_MAX_SIZE);
		wcstombs(out_text, parm.out, PARM_MAX_SIZE);
		wcstombs(log_text, parm.log, PARM_MAX_SIZE);
		*log.stream << "\n ---- Параметры ---- \n\n-in: " << in_text
			<< "\n-out: " << out_text
			<< "\n-log: " << log_text << endl;
	}
	void WriteError(LOG log, Error::ERROR error)
	{
		if (log.stream)
		{
			*log.stream << "\nОшибка " << error.id
				<< ": " << error.message
				<< "\nСтрока " << error.inext.line
				<< " позиция " << error.inext.col << endl;
			Close(log);
		}
		else
		{
			cout << "\nОшибка " << error.id
				<< ": " << error.message
				<< "\nСтрока " << error.inext.line
				<< " позиция " << error.inext.col << endl;
		}
	}

	/*void LTtoLog(Lex::LEX lex, LOG::log)
	{
		*log.stream << "\nТаблица лексем:" << endl;

		*log.stream << setfill('-') << std::setw(87) << '-' << endl;
		*log.stream <<"Номер лексемы"<<"|"<< "Лексема" << " | " << "Номер строки в исходном тексте" << " | " << "Индекс в таблице идентификаторов" << " | " << "Приоритет оператора" <<endl;
		*log.stream << setw(87) << '-' << endl;
		for (int i = 0; i < lextable.size; i++)
		{
			*log.stream << setfill('0') << setw(4) << right << i << " | ";
			*log.stream << setfill(' ') << setw(13) << left << lextable.table[i].lexema[0] << " | ";
			*log.stream << setfill(' ') << setw(13) << left << lextable.table[i].sn << " | ";
			*log.stream << setfill(' ') << setw(13) << left << lextable.table[i].idxTI << " | ";
			if (lextable.table[i].lexema[0] == 'v')
			{
				*log.stream << setfill(' ') << setw(13) << left << lextable.table[i].priority << " | ";
			}
			else {
				*log.stream << setfill(' ') << setw(13) << left << "-" << " | ";
			}
		}
	}*/

	void Close(LOG log)
	{
		log.stream->close();
		delete log.stream;
	}
}