#include <iostream>
#include "seat.hpp"

Seat::Seat(STATUS s, const char* n, unsigned long long c) : stat(s), note(nullptr), code(c)
{
	setNote(n);
}

Seat::Seat(const Seat& s) :stat(s.stat), note(nullptr), code(s.code)
{
	setNote(s.note);
}

void Seat::setNote(const char* n)
{
	delete[] note;
	unsigned size = strlen(n);
	note = new char[size + 1];
	for (unsigned i = 0; i < size; i++)
	{
		note[i] = *(n + i);
	}
	note[size] = '\0';
}

Seat& Seat::operator=(const Seat& s)
{
	if (this != &s)
	{
		stat = s.stat;
		setNote(s.note);
		code = s.code;
	}
	return *this;
}

Seat::~Seat()
{
	delete[] note;
}

/**
*	Кода съдържа в себе си информация за датата, за която е закупено мястото, номера на залата, реда и колоната на които се намира мястото
*	Броят цифри в кода е различен, спрямо това колко редове и колони има залата и нейния номер
*	Пример: Закупено място за дата 2020 6 7, в зала 1, на ред 2, място 3 ще изглежда така - 202006071203
*@param h номер на залата
*@param r ред на мястото
*@param r_all брой редове на залата, в която е мястото
*@param c колона на мястото
*@param c_all брой колони на залата, в която е мястото
*/
void Seat::genCode(unsigned short y,unsigned short m, unsigned short d, unsigned short h, unsigned r,unsigned r_all, unsigned c, unsigned c_all)
{
	code = (y * 100 + m) * 100 + d;
	code = code*findDigits(h) + h;
	code = code*findDigits(r_all) + r;
	code = code*findDigits(c_all) + c;
}

unsigned short Seat::findDigits(unsigned n)
{
	unsigned short tmp = 10;
	while (n/10)
	{
		n /= 10;
		tmp *= 10;
	}
	return tmp;
}