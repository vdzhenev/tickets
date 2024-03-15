#include <iostream>
#include "hall.hpp"

Hall::Hall(uint r, uint c, uint n, const char* e) : rows(r), cols(c), number(n), e_name(nullptr), seats(nullptr)
{
	setSeats();
	setName(e);
}

Hall::Hall(const Hall& h) : rows(h.rows), cols(h.cols), number(h.number), e_name(nullptr), seats(nullptr)
{
	setSeats();
	copySeats(h.seats);
	setName(h.e_name);
}

void Hall::setSeats()				/** Първоначално задаване на местата - освобождава заделената памет за места и заделя динамично памет за (редове*колони) на брой места. Всяко място е със статус "свободно" */
{
	delete[] seats;
	uint size = rows*cols;
	seats = new Seat[size];
	for (uint i = 0; i < size; i++)
	{
		seats[i].setStat(available);
	}

}

void Hall::copySeats(const Seat* s)		/** Копира местата от подаден масив с места */
{
	unsigned size = rows*cols;
	for (size_t i = 0; i < size; i++)
	{
		seats[i] = s[i];
	}
}

void Hall::setName(const char* n)
{
	delete[] e_name;
	uint size = strlen(n);
	e_name = new char[size + 1];
	for (uint i = 0; i < size; i++)
	{
		e_name[i] = *(n + i);
	}
	e_name[size] = '\0';
}


const uint Hall::numberOfSeats(STATUS s)
{
	uint cnt = 0;
	for (uint i = 0; i < rows*cols; i++)
	{
		if (seats[i].getStat() == s)
			cnt++;
	}
	return cnt;
}

Hall& Hall::operator=(const Hall& h)
{
	if (this != &h)
	{
		rows = h.rows;
		cols = h.cols;
		number = h.number;
		setSeats();
		copySeats(h.seats);
		setName(h.e_name);
	}
	return *this;
}

Hall::~Hall()
{
	delete[] seats;
	delete[] e_name;
}