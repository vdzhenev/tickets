#include <iostream>
#include "date.hpp"

Hall HALL1(3, 5, 0);
Hall HALL2(5, 10, 1);
Hall HALL3(10, 12, 2);
Hall ALL_HALLS[HALLS_COUNT] = { HALL1, HALL2, HALL3 };

Date::Date(ushort y, ushort m, ushort d) : year(y), month(m), day(d)
{
	setHalls(ALL_HALLS);
}

Date::Date(const Date&d) : year(d.year), month(d.month), day(d.day)
{
	setHalls(d.halls);
}

Date& Date::operator=(const Date& d)
{
	if (this != &d)
	{
		year = d.year;
		month = d.month;
		day = d.day;
		setHalls(d.halls);
	}
	return *this;
}

void Date::setHalls(const Hall* h)
{
	for (short i = 0; i < HALLS_COUNT; i++)
	{
		halls[i] = h[i];
	}
}

bool Date::isLeap(ushort y)
{
	if (((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0))
		return true;
	return false;
}

bool Date::isValid()
{
	if (year<1000 || year>9999)
		return false;
	if (month < 1 || month > 12)
		return false;
	if (day < 1 || day > 31)
		return false;

	if (month == 2)
	{
		if (isLeap(year))
			return (day <= 29);
		else
			return (day <= 28);
	}

	if (month == 4 || month == 6 || month == 9 || month == 11)
		return (day <= 30);

	return true;
}

bool operator<=(const Date& d1, const Date& d2)
{
	if (d1.year < d2.year)
		return true;
	else if (d1.year == d2.year)
	{
		if (d1.month < d2.month)
			return true;
		else if (d1.month == d2.month)
		{
			if (d1.day <= d2.day)
				return true;
		}
	}
	return false;
}

bool operator>=(const Date& d1, const Date& d2)
{
	if (d1.year > d2.year)
		return true;
	else if (d1.year == d2.year)
	{
		if (d1.month > d2.month)
			return true;
		else if (d1.month == d2.month)
		{
			if (d1.day >= d2.day)
				return true;
		}
	}
	return false;
}