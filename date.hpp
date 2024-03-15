#ifndef __DATE_HPP
#define __DATE_HPP

#include "hall.hpp"

typedef unsigned short ushort;


/*! Дата 
*	Всяка дата се състои от година, месец, ден и Hall::HALLS_COUNT на брой зали (предварително зададени)
*/
struct Date
{
	ushort year;		///< година
	ushort month;		///< месец
	ushort day;			///< ден
	Hall halls[HALLS_COUNT];	///< зали


	/**@name Голяма четворка */
	///@{
	Date(ushort y = 0, ushort m = 1, ushort d = 1);
	Date(const Date&);
	Date& operator=(const Date&);
	~Date() {};
	///@}

	/**@name set-функции */
	///@{
	void setHalls(const Hall*);
	void setDate(ushort y, ushort m, ushort d){ year = y; month = m; day = d; }
	///@}

	/** Предефинирана операция за изход - извежда датата във формат ISO 8601*/
	friend std::ostream& operator<<(std::ostream& o, Date& d)
	{
		return o << d.year << '-' << d.month << '-' << d.day;
	}

	bool isLeap(ushort y);							//!< Проверява дали годината е високосна
	bool isValid();									//!< Проверява дали датата е валидна

	/**@name Предефинирани операции за сравнение */
	///@{
	friend bool operator==(const Date& d1, const Date& d2)
	{
		return (d1.year == d2.year) && (d1.month == d2.month) && (d1.day == d2.day);
	}
	friend bool operator<=(const Date& d1, const Date& d2);
	friend bool operator>=(const Date& d1, const Date& d2);
	///@}
};



#endif