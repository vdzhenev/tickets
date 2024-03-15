/** @file main.cpp */

#include <iostream>
#include <fstream>
#include "date.hpp"
#include "hall.hpp"
#include "seat.hpp"

using std::cout;
using std::cin;
using std::endl;

/**@name Глобални променливи */
///@{
const unsigned MAX_CMD_SIZE = 256;	///< Максимална дължина на входа
Date* ALL_DATES;					///< Масив с всички въведени дати
ushort DATE_CURR = 0;				///< Брой текущо въведени дати
ushort DATE_MAX = 0;				///< Максимален брой дати
///@}


/*! Структура за работа с командния ред */
struct CommandLine
{
	char inp[MAX_CMD_SIZE];			///< Масив, в който се записва входа
	unsigned p = 0;					///< Показва докъде е прочетен входа

	void extractCMD(char* str)		/** Извлича команда (или една дума) от входния низ */
	{
		clearWhitespaces();
		unsigned i = 0;
		for (; inp[p] != '\0' && inp[p] != ' ' && inp[p] != '\n';p++)
		{
			str[i++] = inp[p];
		}
		str[i] = '\0';
	}

	void extractDate(Date &d)		/** Извлича дата (три числа) от входния низ */
	{
		extractNumberShort(d.year);
		extractNumberShort(d.month);
		extractNumberShort(d.day);
	}

	void extractNumberShort(ushort &n)	/** Извлича число от тип unsigned short от входния низ */
	{
		clearWhitespaces();
		if (inp[p] != '\0')
			n = 0;
		for (; inp[p] != '\0' && inp[p] >= '0' && inp[p] <= '9'; p++)
		{
			n = n * 10 + inp[p] - '0';
		}
	}
	
	void extractUInt(uint &n)		/** Извлича число от тип unsigned int от входния низ */
	{
		clearWhitespaces();
		n = 0;
		for (; inp[p] != '\0' && inp[p] >= '0' && inp[p] <= '9'; p++)
		{
			n = n * 10 + inp[p] - '0';
		}
	}

	bool processCode(ushort &h, uint &rows, uint&cols)	/** Извлича от входа цифрите на число, като се придържа към формата, зададен в Seat::genCode()  */
	{													/** Първо извлича 4 цифри за година, после 2 за месец, 2 за ден и т.н. */
		clearWhitespaces();								/** (Броя на цифрите, извлечени за номер на зала, ред и колона се менят в зависимост залата) */
		ushort y = 0;									/** По извлечената от входа дата, създава временна променлива от тип Date и сравнява с всяка от записаните в ALL_DATES дати */
		for (int i = 0; i < 4 && inp[p] != '\0'; i++)	/** Ако открие съответствие сравнява извлечените цифри за номер на зала, ред и колона с всяка от залите на датата */
			y = y * 10 + inp[p++] - '0';				/** Ако и те съвпадат проверява статуса на мястото. Ако е закупено кодът от входния низ е валиден. */
		ushort m = 0;
		for (int i = 0; i < 2 && inp[p] != '\0'; i++)
			m = m * 10 + inp[p++] - '0';
		ushort d = 0;
		for (int i = 0; i < 2 && inp[p] != '\0'; i++)
		{
			d = d * 10 + inp[p++] - '0';
		}
		h = HALLS_COUNT;
		ushort tmp = 0;
		do
		{
			h /= 10;
			tmp++;
		} while (h);
		for (int i = 0; i < tmp&& inp[p] != '\0'; i++)
		{
			h = h * 10 + inp[p++] - '0';
		}
		Date n_date(y, m, d);
		for (ushort i = 0; i < DATE_CURR; i++)
		{
			if (ALL_DATES[i]==n_date && h<HALLS_COUNT)
			{
				rows = ALL_DATES[i].halls[h].rows;
				cols = ALL_DATES[i].halls[h].cols;
				ushort tmp_r = 0, tmp_c = 0;
				do
				{
					rows /= 10;
					tmp_r++;
				} while (rows);
				for (int i = 0; i < tmp_r && inp[p]!='\0'; i++)
				{
					rows = rows * 10 +inp[p++]- '0';
				}
				do
				{
					cols /= 10;
					tmp_c++;
				} while (cols);
				for (int i = 0; i < tmp_c&& inp[p] != '\0'; i++)
				{
					cols = cols * 10 + inp[p++] - '0';
				}
				if (--rows <ALL_DATES[i].halls[h].rows && --cols<ALL_DATES[i].halls[h].cols)
				{
					if (ALL_DATES[i].halls[h].seats[rows*ALL_DATES[i].halls[h].cols+cols].getStat() == bought)
					{
						return true;
					}
				}
			}
		}
		return false;

	}

	void clearWhitespaces()		/** Премахва празните позиции до срещане на различен символ */
	{
		while (inp[p] == ' ' || inp[p] == '-' || inp[p] == '/') 	p++;
	}

	void extractLeftoverString(char* str)	/** Извлича останалия във входа низ като премахва празните позиции в края на низа */
	{
		clearWhitespaces();
		unsigned i = 0;
		for (; inp[p] != '\0' && inp[p] != '\n'; p++)
		{
			str[i++] = inp[p];
		}
		while (str[--i] == ' ')
		{
			;
		}
		i++;
		str[i] = '\0';
	}

	void extractName(char* str)			/** Извлича име - всичко до края на низа или до срещането на символа '/' */
	{
		clearWhitespaces();
		unsigned i = 0;
		for (; inp[p] != '\0' && inp[p] != '\n' && inp[p]!='/'; p++)
		{
			str[i++] = inp[p];
		}
		while (str[--i] == ' ')
		{
			;
		}
		i++;
		str[i] = '\0';
	}
};

ushort existsDate(const Date& d)	/** Търси дали дадена дата е записана в масива ALL_DATES и връща нейния индекс */
{
	ushort i = 0;
	for (i; i < DATE_CURR; i++)
	{
		if (ALL_DATES[i] == d)
		{
			break;
		}
	}
	return i;
}

void addDate(const Date& d)			/** Добавя дата към масива ALL_DATES. Ако текущият брой дати надвишава максималния се прави нов масив с по-голяма дължина, информацията от стария се копира в новия и старият се изтрива */
{
	if (DATE_CURR >= DATE_MAX)
	{
		DATE_MAX += 8;
		Date* n_dates = new Date[DATE_MAX];
		for (ushort i = 0; i < DATE_CURR; i++)
		{
			n_dates[i] = ALL_DATES[i];
		}
		delete[] ALL_DATES;
		ALL_DATES = n_dates;
	}
	ALL_DATES[DATE_CURR++] = d;
}

bool freeHall(const Date& d, ushort h)		/** Проверява дали дадена зала е свободна по подадена дата и номер на зала. 
											*	Проверява за съществуването на датата в масива. Ако е записана, проверява за събитие в дадената зала. Ако не е я добавя чрез addDate(). 
											*/
{
	ushort ind = existsDate(d);
	if (ind == DATE_CURR)
	{
		addDate(d);
		return true;
	}
	else
	{
		if (strcmp(ALL_DATES[ind].halls[h].e_name,"-"))
		{
			return false;
		}
	}
	ALL_DATES[ind].halls[h].setName(d.halls[h].e_name);
	return true;
	
}

/**@name Работа с файлове */
///@{
/** Запазва данните за място във файл. Записва кода, статуса, дължина на бележката и самата бележка. */
void saveSeat(std::ofstream& outfile, Seat& s)
{
	outfile.write((char*)&s.getCode(), sizeof(s.getCode()));;
	outfile.write((char*)&s.getStat(), sizeof(STATUS));
	ushort len = strlen(s.getNote());
	outfile.write((char*)&len, sizeof(len));
	outfile.write(s.getNote(), len*sizeof(char));
}

/** Запазва данните за зала във файл. Записва брой редове, колони, номер на залата. Запазва всяко от местата в залата чрез функцията saveSeat(). Накрая записва дължината на името на събитието и самото име */
void saveHall(std::ofstream& outfile, const Hall& h)
{
	outfile.write((char*)&h.rows, sizeof(h.rows));
	outfile.write((char*)&h.cols, sizeof(h.cols));
	outfile.write((char*)&h.number, sizeof(h.number));
	for (ushort i = 0; i < h.rows*h.cols; i++)
	{
		saveSeat(outfile,h.seats[i]);
	}
	ushort len = strlen(h.e_name);
	outfile.write((char*)&len, sizeof(len));
	outfile.write(h.e_name, len*sizeof(char));
}

/** Запазва данните за дата във файл. Записва година, месец, ден и запазва всяка от залите чрез функцията saveHall() */
void saveDate(std::ofstream& outfile, const Date& d)
{
	outfile.write((char*)&d.year, sizeof(d.year));
	outfile.write((char*)&d.month, sizeof(d.month));
	outfile.write((char*)&d.day, sizeof(d.day));

	for (ushort i = 0; i < HALLS_COUNT; i++)
	{
		saveHall(outfile, d.halls[i]);
	}
}

/** Запазва текущата информация във файл. Ако не е отворен файл презаписва "default.db". Предоставя възможност за отказ от запазване. Първо записва текущия брой дати, след което запазва всяка от датите чрез saveDate() */
void saveFile(char* filename)					
{
	bool flag = false;
	if (*filename=='\0')
	{
		char proceed = 'n';
		cout << "No opened file! This action will overwrite \"default.db\"!\n";
		do
		{
			char temp = ' ';
			cout << "Proceed ? [y / n]\n> ";
			cin.get(proceed);
			cin.get(temp);

		} while (proceed!='y'&&proceed!='n');
		if (proceed == 'y')
			strcpy_s(filename, MAX_CMD_SIZE, "default.db");
		else
		{
			cout << "Progress not saved! You can use command \"saveas\" to save to a different file!\n";
			flag = true;
		}
	}
	if (!flag)
	{
		std::ofstream outfile;
		outfile.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!outfile.is_open())
			cout << "Error! There was a problem while saving!\n";
		else
		{
			cout << "Saving data...\n";
			outfile.write((char*)&DATE_CURR, sizeof(DATE_CURR));
			for (ushort i = 0; i < DATE_CURR; i++)
			{
				saveDate(outfile, ALL_DATES[i]);
			}
			cout << "Successfully saved " << filename << "!\n";
			outfile.close();
		}
	}
}

/** Зарежда информацията за място от файл. Работи аналогично на saveSeat(), като използва set-функциите на Seat. Динамично заделя памет за бележката и после я освобождава */
void loadSeat(std::ifstream&infile, Seat& s)
{
	unsigned long long c;
	infile.read((char*)&c, sizeof(c));
	s.setCode(c);
	STATUS st;
	infile.read((char*)&st, sizeof(st));
	s.setStat(st);
	ushort len;
	infile.read((char*)&len, sizeof(len));
	char* note = new char[len + 1];
	infile.read(note, len*sizeof(char));
	note[len] = '\0';
	s.setNote(note);
	delete[]note;
}

/** Зарежда информацията за зала от файл. Работи аналогично на saveHall(), като използва set-функциите на Hall. Използва Hall::setSeats() и зарежда всяко от местата чрез loadSeat(). Динамично заделя памет за името на събитието и после я освобождава. */
void loadHall(std::ifstream& infile, Hall&h)
{
	infile.read((char*)&h.rows, sizeof(h.rows));
	infile.read((char*)&h.cols, sizeof(h.cols));
	infile.read((char*)&h.number, sizeof(h.number));
	h.setSeats();
	for (ushort i = 0; i < h.rows*h.cols; i++)
	{
		Seat s;
		loadSeat(infile, s);
		h.seats[i] = s;
	}
	ushort len;
	infile.read((char*)&len, sizeof(len));
	char* name = new char[len + 1];
	infile.read(name, len*sizeof(char));
	name[len] = '\0';
	h.setName(name);
	delete[] name;
}

/** Зарежда информацията за дата от файл. Работи аналогично на saveDate(), като използва Date::setDate(). Зарежда всяка от залите чрез loadHall() */
void loadDate(std::ifstream&infile, Date& d)
{
	ushort y, m, day;
	infile.read((char*)&y, sizeof(y));
	infile.read((char*)&m, sizeof(m));
	infile.read((char*)&day, sizeof(day));
	d.setDate(y, m, day);
	for (ushort i = 0; i < HALLS_COUNT; i++)
	{
		Hall h;
		loadHall(infile, h);
		d.halls[i] = h;
	}
}

/** Отваря файл и зарежда информацията за всяка от датите чрез loadDate() */
void openFile(char* filename)
{
	if (!filename)
		strcpy_s(filename, MAX_CMD_SIZE, "default.db");

	std::ifstream infile;
	infile.open(filename, std::ios::in | std::ios::binary);
	if (!infile.is_open())
		cout << "Error! Couldn't open file!\n";
	else
	{
		cout << "Loading data...\n";
		infile.read((char*)&DATE_CURR, sizeof(DATE_CURR));
		DATE_MAX = 8 * (DATE_CURR / 8) + 8;
		delete[] ALL_DATES;
		ALL_DATES = new Date[DATE_MAX];
		
		for (ushort i = 0; i < DATE_CURR; i++)
		{
			Date d;
			loadDate(infile, d);
			ALL_DATES[i] = d;
		}
		cout << "Successfully opened " << filename << "!\n";
	}
	
	infile.close();
}

///@}

/** Основното тяло на програмата 
*	Съдържа информация за текущо отворения файл, както и реализация на командите
*/
int main()
{
	char OPENED_FILE[MAX_CMD_SIZE];
	OPENED_FILE[0] = '\0';

	cout << "Type \"help\" for a list of available commands.\n";
	while (true)
	{
		CommandLine cmd;
		cout << "\n> ";
		cin.getline(cmd.inp, MAX_CMD_SIZE);

		char command[MAX_CMD_SIZE];
		cmd.extractCMD(command);

//!==========================================================================================================================================
		//! "help" - извежда списък с налични команди
		if (!strcmp(command, "help"))
		{
			cout << "\nThe following commands are supported:\n\n"

				<< "open <file>\t\t\t\t| Loads data from file with name <file>\n"
				<< "close <file>\t\t\t\t| Closes current file\n"
				<< "save	\t\t\t\t| Saves data in currently opened file\n"
				<< "saveas <file>\t\t\t\t| Saves data in file with name <file>\n"
				<< "help	\t\t\t\t| Displays this information\n"
				<< "exit	\t\t\t\t| Exits the program\n\n"

				<< "addevent <date> <hall> <name>	\t| Adds event with name <name> in hall <hall> on date <date>\n"
				<< "freeseats <date> <name>	\t\t| Shows available seats for event with name <name> on date <date>\n"
				<< "book <row> <seat> <date> <name> / <note>| Books <seat> on <row> on <date> for event <name> and adds a <note>\n"
				<< "unbook <row> <seat> <date> <name>\t| Unbooks <seat> on <row> on <date> for event <name>\n"
				<< "buy <row> <seat> <date> <name>\t\t| Purchases <seat> on <row> on <date> for event <name>\n"
				<< "bookings[<date>][<name>]\t\t| Shows number of booked seats on [<date>] for event [<name>]\n"
				<< "check <code>\t\t\t\t| Verifies the <code> you enter and returns your seat, row and hall\n"
				<< "report <from> <to> [<hall>]\t\t| Reports number of purchased tickets in period <from> <to>\n";
		}
//!==========================================================================================================================================
		//! "save" - записва информацията в текущо отворения файл чрез saveFile()
		else if (!strcmp(command, "save"))
		{
			saveFile(OPENED_FILE);
		}
//!==========================================================================================================================================
		//! "saveas" - записва информацията в подаден от потребителя файл чрез saveFile()
		else if (!strcmp(command, "saveas"))
		{
			cmd.extractLeftoverString(OPENED_FILE);
			saveFile(OPENED_FILE);
		}
//!==========================================================================================================================================
		//! "open" - отваря файл чрез openFile()
		else if (!strcmp(command, "open"))
		{
			cmd.extractLeftoverString(OPENED_FILE);
			openFile(OPENED_FILE);
		}
//!==========================================================================================================================================
		//! "close" - "затваря" файл
		else if (!strcmp(command, "close"))
		{
			OPENED_FILE[0] = '\0';
		}
//!==========================================================================================================================================
		//! "exit" - прекратява изпълнението на програмата
		else if (!strcmp(command, "exit"))
		{
			cout << "Exiting the program...\n";
			break;
		}
//!==========================================================================================================================================
		//! "addevent" - добавя събитие (дата) към масива. 
		//! Проверява дали залата е свободна на тази дата. Добавя я чрез freeHall()
		else if (!strcmp(command, "addevent"))
		{
			Date n_date;
			cmd.extractDate(n_date);
			if (!n_date.isValid())
				cout << "Invalid date!\n";
			else
			{
				ushort h = 0;
				cmd.extractNumberShort(h);
				if (h < HALLS_COUNT)
				{
					char e[MAX_CMD_SIZE];
					cmd.extractLeftoverString(e);
					n_date.halls[h].setName(e);
					if (freeHall(n_date, h))
					{
						cout << "Successfully added event:\nName: "
							<< ALL_DATES[DATE_CURR - 1].halls[h].e_name
							<< "\nHall: " << h
							<< "\nDate: " << ALL_DATES[DATE_CURR - 1] << endl;
					}
					else
						cout << "The hall that you requested is unavailable on " << n_date << "!\n";
				}
				else
					cout << "Error! Hall with number " << h << " does not exist!\n";
			}
		}
//!==========================================================================================================================================
		//! "freeseats" - извежда списък с места за дадено събитие на дадена дата
		else if (!strcmp(command, "freeseats"))
		{
			Date n_date;
			cmd.extractDate(n_date);
			bool flag = false;
			for (ushort i = 0; i < DATE_CURR; i++)
			{
				if (n_date == ALL_DATES[i])
				{
					char e[MAX_CMD_SIZE];
					cmd.extractLeftoverString(e);
					for (ushort j = 0; j < HALLS_COUNT; j++)
					{
						
						if (!strcmp(ALL_DATES[i].halls[j].e_name, e))
						{
							flag = true;
							uint r = ALL_DATES[i].halls[j].rows;
							uint c = ALL_DATES[i].halls[j].cols;
							uint size = r*c;
							cout << "\nHall " << j << ':' << endl;
							for (uint k = 0; k < size; k++)
							{
								if (!(k%c))
									cout << endl << "Row " << (k/c) +1<<"\t|";
								if (ALL_DATES[i].halls[j].seats[k].getStat() == available)
								{
									cout << (k%c) + 1 << '\t';
								}
								else if (ALL_DATES[i].halls[j].seats[k].getStat() == bought)
								{
									cout << "X\t";
								}
								else
									cout << (k%c) + 1 << "*\t";
							}
							cout << endl;
						}
						
					}
				}
			}
			cout << endl<<"Legend:\t * - BOOKED\t X - PURCHASED\n";
			if (!flag)
				cout << "No event matching the given description found!\n" << "If you wish to add it use \"addevent\"!\n";
		}
//!==========================================================================================================================================
		//! "book" - запазва място за дадено събитие. 
		//! Проверява дали събитието съществува и дали мястото е свободно.
		else if (!strcmp(command, "book"))
		{
			uint _r, _c;
			cmd.extractUInt(_r);
			cmd.extractUInt(_c);
			Date n_date;
			cmd.extractDate(n_date);
			char n_name[MAX_CMD_SIZE];
			cmd.extractName(n_name);
			char n_note[MAX_CMD_SIZE];
			cmd.extractLeftoverString(n_note);

			bool flag_event = false;
			bool flag_free = false;
			for (ushort i = 0; i < DATE_CURR; i++)
			{
				if (ALL_DATES[i] == n_date)
				{
					for (ushort j = 0; j < HALLS_COUNT; j++)
					{
						uint rows = ALL_DATES[i].halls[j].rows;
						uint cols = ALL_DATES[i].halls[j].cols;
						if (!(strcmp(ALL_DATES[i].halls[j].e_name,n_name))&&_r - 1 < rows&&_c - 1<cols)
						{
							flag_event = true;
							uint place = (_r - 1)*cols + _c - 1;
							if (ALL_DATES[i].halls[j].seats[place].getStat() == available)
							{
								flag_free = true;
								ALL_DATES[i].halls[j].seats[place].setStat(booked);
								ALL_DATES[i].halls[j].seats[place].setNote(n_note);
								cout << "Seat successfully booked! Added note: "<<n_note<<endl<<"To buy a seat, you can use command \"buy\"!\n";
								break;
							}
						}
					}
					break;
				}
			}
			if (flag_event&&!flag_free)
				cout << "Place not available! For a list of available seats, use command \"freeseats\"!\n";
			else if (!flag_event)
				cout << "Error! Event not found! You can add it using command \"addevent\"!\n";

		}
//!==========================================================================================================================================
		//! "unbook" - променя статуса на място от "запазено" на "свободно". 
		//! Проверява за съществуване на събитието и статус на мястото.
		else if (!strcmp(command, "unbook"))
		{
			uint _r, _c;
			cmd.extractUInt(_r);
			cmd.extractUInt(_c);
			Date n_date;
			cmd.extractDate(n_date);
			char n_name[MAX_CMD_SIZE];
			cmd.extractName(n_name);

			bool flag_event = false;
			bool flag_free = false;
			for (ushort i = 0; i < DATE_CURR; i++)
			{
				if (ALL_DATES[i] == n_date)
				{
					for (ushort j = 0; j < HALLS_COUNT; j++)
					{
						uint rows = ALL_DATES[i].halls[j].rows;
						uint cols = ALL_DATES[i].halls[j].cols;
						if (!(strcmp(ALL_DATES[i].halls[j].e_name, n_name)) && _r - 1 < rows&&_c - 1 < cols)
						{
							flag_event = true;
							uint place = (_r - 1)*cols + _c - 1;
							if (ALL_DATES[i].halls[j].seats[place].getStat() == booked)
							{
								flag_free = true;
								ALL_DATES[i].halls[j].seats[place].setStat(available);
								ALL_DATES[i].halls[j].seats[place].setNote("-");
								cout << "Seat successfully unbooked!\n";
								break;
							}
						}
					}
					break;
				}
			}
			if (flag_event&&!flag_free)
				cout << "Place not booked or already purchased! For a list of available seats, use command \"freeseats\"!\n";
			else if (!flag_event)
				cout << "Error! Event not found! You can add it using command \"addevent\"!\n";
		}
//!==========================================================================================================================================
		//! "buy" - променя статуса на място на "закупено" и генерира код за мястото чрез Seat::genCode().
		else if (!strcmp(command, "buy"))
		{
			uint _r, _c;
			cmd.extractUInt(_r);
			cmd.extractUInt(_c);
			Date n_date;
			cmd.extractDate(n_date);
			char n_name[MAX_CMD_SIZE];
			cmd.extractName(n_name);

			bool flag_event = false;
			bool flag_free = false;
			for (ushort i = 0; i < DATE_CURR; i++)
			{
				if (ALL_DATES[i] == n_date)
				{
					for (ushort j = 0; j < HALLS_COUNT; j++)
					{
						uint rows = ALL_DATES[i].halls[j].rows;
						uint cols = ALL_DATES[i].halls[j].cols;
						if (!(strcmp(ALL_DATES[i].halls[j].e_name, n_name)) && _r - 1 < rows && _c - 1 < cols)
						{
							flag_event = true;
							uint place = (_r - 1)*cols + _c - 1;
							if (ALL_DATES[i].halls[j].seats[place].getStat() != bought)
							{
								flag_free = true;
								ALL_DATES[i].halls[j].seats[place].setStat(bought);
								ALL_DATES[i].halls[j].seats[place].genCode(n_date.year, n_date.month, n_date.day, j, _r, rows, _c, cols);
								cout << "Purchase successful! Use the following code as verification:\n"
									<< ALL_DATES[i].halls[j].seats[place].getCode() << endl;
								break;
							}
						}
					}
					break;
				}
			}
			if (!flag_event)
				cout << "Error! Place or event not found!\n";
			else if (!flag_free)
				cout << "Error! The place you have chosen is not available!\n";
		}
//!==========================================================================================================================================
		//! "bookings" - Извежда информация за запазените места за дадено представление на дадена дата. 	
		else if (!strcmp(command, "bookings"))
		{
			Date d;
			Date idk(0,0,0);
			char name[MAX_CMD_SIZE];
			cmd.extractDate(d);
			cmd.extractLeftoverString(name);
			
			if (*name == '\0')
			{
				//! Ако не е подадено нито представление, нито дата - извежда информация за всички представления и дати
				if (d == idk)
				{
					for (ushort i = 0; i < DATE_CURR; i++)
					{
						for (ushort j = 0; j < HALLS_COUNT; j++)
						{
							uint cnt = ALL_DATES[i].halls[j].numberOfSeats(booked);
							if (cnt>0)
								cout << "Booked seats on " << ALL_DATES[i] << " for event " << ALL_DATES[i].halls[j].e_name << ": " << cnt<< endl;
						}
					}
				}
				//! Ако не е подадено представление - извежда за всички представления на дадената дата
				else
				{
					uint cnt = 0;
					for (ushort i = 0; i < DATE_CURR; i++)
					{
						if (ALL_DATES[i] == d)
						{
	
							for (ushort j = 0; j < HALLS_COUNT; j++)
							{
								cnt += ALL_DATES[i].halls[j].numberOfSeats(booked);
							}
							break;
						}
					}
					if (!cnt)
						cout << "No events took place on " << d << "!\n";
					else
						cout << "Booked seats on " << d << ": " << cnt<<endl;
				}
			}
			//! Ако не е подадена дата - извежда за даденото преставление на всички дати
			else if (d == idk && *name != '\0')
			{
				bool flag = false;
				for (ushort i = 0; i < DATE_CURR; i++)
				{
					for (ushort j = 0; j < HALLS_COUNT; j++)
					{
						if (!strcmp(ALL_DATES[i].halls[j].e_name, name))
						{
							flag = true;
							uint cnt = ALL_DATES[i].halls[j].numberOfSeats(booked);
							if (cnt)
								cout << "Booked seats on " << ALL_DATES[i] << " for event " << name << ": " << cnt<<endl;
						}
					}
				}
				if (!flag)
					cout << "Event not found!\n";
			}
			else
			{
				for (ushort i = 0; i < DATE_CURR; i++)
				{
					if (ALL_DATES[i] == d)
					{
						for (ushort j = 0; j < HALLS_COUNT; j++)
						{
							if (!strcmp(ALL_DATES[i].halls[j].e_name, name))
							{
								cout << "Booked seats on " << d << " for event " << name << ": " << ALL_DATES[i].halls[j].numberOfSeats(booked)<<endl;
							}
						}
					}
				}
			}
		}
//!==========================================================================================================================================
		//! "check" - Проверява дали даден код е валиден чрез CommandLine::processCode()
		else if (!strcmp(command, "check"))
		{
			uint c, r;
			ushort h;
			if (cmd.processCode(h, r, c))
			{
				cout << "Code verified! Your place is:\n"
					<< "Hall: " << h << "\nRow: " << r + 1 << "\nSeat: " << c + 1<<endl;
			}
			else
				cout << "Error! Invalid code!\n";
		}
//!==========================================================================================================================================
		//! "report" - Извежда справка за всички закупени места в дадена зала за даден период
		else if (!strcmp(command, "report"))
		{
			Date d_from;
			Date d_to;
			cmd.extractDate(d_from);
			cmd.extractDate(d_to);
			ushort h = HALLS_COUNT;
			cmd.extractNumberShort(h);
			uint total = 0;
			if (h < HALLS_COUNT)
			{
				for (ushort i = 0; i < DATE_CURR; i++)
				{
					uint cnt = 0;
					if (ALL_DATES[i] >= d_from && ALL_DATES[i] <= d_to)
					{
						uint rows = ALL_DATES[i].halls[h].rows;
						uint cols = ALL_DATES[i].halls[h].cols;
						uint size = rows*cols;
						for (uint j = 0; j < size; j++)
						{
							if (ALL_DATES[i].halls[h].seats[j].getStat() == bought)
								cnt++;
						}
						total += cnt;
						cout << "Event:\t" << ALL_DATES[i].halls[h].e_name << "\nDate:\t" << ALL_DATES[i] << "\nSold:\t" << cnt << endl << endl;
					}
				}
				cout << "[Hall "<<h<<"] Total amount of tickets sold in period: " << total<<endl;
			}
			//! Ако не е подаден номер на зала извежда информация за всички зали
			else
			{
				for (ushort i = 0; i < DATE_CURR; i++)
				{
					
					if (ALL_DATES[i] >= d_from && ALL_DATES[i] <= d_to)
					{
						for (ushort k = 0; k < HALLS_COUNT; k++)
						{
							uint cnt = 0;
							if (!strcmp(ALL_DATES[i].halls[k].e_name, "-"))
								continue;
							uint rows = ALL_DATES[i].halls[k].rows;
							uint cols = ALL_DATES[i].halls[k].cols;
							uint size = rows*cols;
							for (uint j = 0; j < size; j++)
							{
								if (ALL_DATES[i].halls[k].seats[j].getStat() == bought)
									cnt++;
							}
							total += cnt;
							cout << "Event:\t" << ALL_DATES[i].halls[k].e_name <<"\nHall:\t"<<k<< "\nDate:\t" << ALL_DATES[i] << "\nSold:\t" << cnt << endl << endl;

						}
					}
				}
				cout << "Total amount of tickets sold in period: " << total;
			}
		}
//!==========================================================================================================================================
		//! Накрая освобождаваме паметта, заделена за датите
		else
		{
			cout << "Unknown command! For a list of available commands, type \"help\"!";
		}
			
	}
	delete[] ALL_DATES;
	return 0;
}