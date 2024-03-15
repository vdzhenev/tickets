#ifndef __HALL_HPP
#define __HALL_HPP

#include "seat.hpp"

typedef unsigned int uint;

const short HALLS_COUNT = 3;	///< Брой на предварително зададените зали

/*! Зала */

struct Hall
{
	uint rows;			///< Брой редове
	uint cols;			///< Брой колони (места на всеки ред)
	uint number;		///< Номер на залата
	Seat* seats;		///< Масив, който съдържа информация за всяко място в залата
	char* e_name;		///< Име на събитието, което ще се състои в залата


	/**@name Голяма четворка */
	///@{
	Hall(uint r = 1, uint c = 1, uint n = 0, const char* e = "-");
	Hall(const Hall&);
	Hall& operator=(const Hall&);
	~Hall();
	///@}

	/**@name set-функции 
	*	Предварително освобождават динамично заделената памет (където е необходимо)
	*/
	///@{
	void setName(const char*);
	void setSeats();
	void copySeats(const Seat*);
	///@}

	const uint numberOfSeats(STATUS s);	//!< връща брой места със статус, съвпадащ с подадения


};

#endif