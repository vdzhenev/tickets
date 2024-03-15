#ifndef __SEAT_HPP
#define __SEAT_HPP

/*! enum за статуса на мястото*/
enum STATUS
{
	available,	///< свободно
	booked,		///< запазено
	bought		///< закупено
};


class Seat
{
	unsigned long long code;		///< код на мястото
	STATUS stat;					///< статус
	char* note;						///< бележка, оставена при запазване

public:
	/**@name Голяма четворка */
	///@{
	Seat(STATUS s = available, const char* n = "-", unsigned long long c = 0);
	Seat(const Seat&);
	Seat& operator=(const Seat&);
	~Seat();
	///@}

	/**@name get-функции */
	///@{
	const unsigned long long& getCode(){ return code; }
	const STATUS& getStat(){ return stat; }
	const char* getNote(){ return note; }
	///@}

	/**@name set-функции
	*	Предварително освобождават динамично заделената памет (където е необходимо)
	*/
	///@{
	void setNote(const char*);
	void setStat(const STATUS s){ stat = s; }
	void setCode(const unsigned long long c){ code = c; }
	///@}

	void genCode(unsigned short y, unsigned short m, unsigned short d, unsigned short h, unsigned r,unsigned r_all, unsigned c,unsigned c_all); //!< генерира уникален код за мястото при закупуване
	unsigned short findDigits(unsigned);		//!< връща брой цифри в подадено число
};

#endif