/*
* File: task2.cpp
* Title: Management of a database containing operational amplifiers
* Version: 1.01
* Created: 23/01/2017
* Last edited: 02/02/2017
*
* Author: Daniel Glover
* ECS at Loughborough University
*
* Purpose of code: To allow a user to create, edit and manipulate a database
* of OPERATIONAL AMPLIFIERS to the required specification. This would be to
* ENTER data to a database, SAVE and LOAD the database from a file, SORT the
* database in regards to NAME and SLEW RATE, and finally to be able to
* DISPLAY the database. Any other options are additional to the spec.
*
* Limitations: If an error on input is made, the whole database is CLEARED,
* not individual OPERATIONAL AMPLIFIERS.
*/

#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>		//Had to add in <algorithm> to use the sort algorithm.
using namespace std;
#define DATABASE_FILENAME "database.txt"
#define DATABASE_MAX 10		//As with task 1.


class OpAmps
{
public:
	void EnterOpAmp();
	void DisplayOpAmp() const;
	void ClearOpAmp();
	friend ostream& operator<<(ostream& Output, const OpAmps& Save);
	friend istream& operator>>(istream& Input, OpAmps& Load);
	friend bool SortByName(const OpAmps &, const OpAmps &);
	friend bool SortBySlew(const OpAmps &, const OpAmps &);
private:
	char Name[20];
	unsigned int PinCount;
	double SlewRate;
};
//^^This class is 2nd level, and is responsible for performing^^
//^^the operations that the 1st level class AllOpAmps requires^^
//^^Definitions below^^

class AllOpAmps
{
public:
	AllOpAmps();
	~AllOpAmps();
	void AddOpAmp();
	void WriteToFile() const;
	void ReadFromFile();
	void SortDatabase();
	void DisplayDatabase() const;
	void ClearDatabase();
	void Initialise()
	{
		database_length = 0; //Initialise number of OpAmps to 0
	}
private:
	OpAmps OpAmp[DATABASE_MAX]; //Creates 10 objects of type OpAmps
	unsigned long database_length;
};
//^^This class is 1st level, responsible for higher^^
//^^level functions^^

AllOpAmps::AllOpAmps() //Assigns memory for database at beginning of program
{
	cout << "DATABASE AVAILABLE" << endl; //Ensures constructor works
}

AllOpAmps::~AllOpAmps() //Memory no longer needed after program ends
{
	cout << "DATABASE CLOSED" << endl; //Ensures destructor works
}

void OpAmps::EnterOpAmp() //Updates object with new OpAmp data
{
	cout << "Please enter details of new Operational Amplifier" << endl;
	cout << "-------------------------------------------------" << endl;
	cout << "Name (No Spaces): "; cin >> Name;
	cout << "Pin Count: "; cin >> PinCount;
	cout << "Slew Rate: "; cin >> SlewRate; //Inputs data to desired location
}

bool SortByName(const OpAmps &NameA, const OpAmps &NameB)	//Compares 2 arrays of
{															//characters, returns true
	return strcmp(NameA.Name, NameB.Name) < 0;				//if B greater than A,
}															//false otherwise

bool SortBySlew(const OpAmps &SlewA, const OpAmps &SlewB)	//Compares 2 doubles,
{															//returns true if B is
	return SlewA.SlewRate < SlewB.SlewRate;					//greater than A, false
}															//otherwise

void OpAmps::DisplayOpAmp() const //Displays OpAmp, no values changed
{
	cout << Name << "\t\t" << PinCount << "\t\t" << SlewRate << endl;
}

void OpAmps::ClearOpAmp() //Clears the OpAmp
{
	Name[20] = ' ';
	PinCount = 0;
	SlewRate = 0;
}

ostream& operator<<(ostream& Output, const OpAmps& Save)
{
	Output << Save.Name << endl << Save.PinCount << endl << Save.SlewRate << endl << endl;
	return Output;	//the overloading operator for saving to a file
}					//storing all necessary data types into Output

istream& operator>>(istream& Input, OpAmps& Load)
{
	Input >> Load.Name >> Load.PinCount >> Load.SlewRate;
	return Input;	//the overloading operator for loading from a file
}					//storing all necessary data types into Input

void AllOpAmps::AddOpAmp() //Enters data to new OpAmp object
{
	if (database_length == 10)
	{
		cerr << "ERROR: DATABASE FULL" << endl; //maximum number already exist
	}
	else
	{
		OpAmp[database_length].EnterOpAmp(); //Update data of new OpAmp object
		database_length++;	//Next empty OpAmp object, increasing the length to
	}						//accommodate
}

void AllOpAmps::WriteToFile() const
{
	fstream output_file;
	output_file.open(DATABASE_FILENAME, ios::out);
	if (!output_file.good()) //exits if file is unopened
	{
		cerr << "FATAL ERROR: Could not save to database";
		exit(1);
	}
	output_file << database_length << endl << endl; //input length of database to
	for (unsigned long i = 0; i < database_length; i++) //the file first
	{
		output_file << OpAmp[i]; //the ostream overload value is returned
	}
	cout << "SAVE SUCCESSFUL" << endl;
	output_file.close(); //closes for safety
}

void AllOpAmps::ReadFromFile()
{
	fstream input_file;
	input_file.open(DATABASE_FILENAME, ios::in);
	if (!input_file.good()) //exits if file is unopened
	{
		cerr << "FATAL ERROR: Could not load from database";
		exit(1);
	}
	input_file >> database_length; //first item of data will be database length
	for (unsigned long i = 0; i < database_length; i++)
	{
		input_file >> OpAmp[i]; //the istream overload value saves the information
	}					//to the necessary variables
}

void AllOpAmps::SortDatabase()
{
	char UserInput;		//User defined input for menu below. Unique from main menu

	cout << endl;
	cout << "Sorting options" << endl;
	cout << "---------------" << endl;
	cout << "1. To sort by name" << endl;
	cout << "2. To sort by slew rate" << endl;
	cout << "3. No sorting" << endl << endl;

	cout << "Enter your option: ";
	cin >> UserInput;
	cout << endl;

	switch (UserInput)
	{
	case '1':
		sort(OpAmp, OpAmp + database_length, SortByName);	//-Defines the first and last items to be sorted
		break;												//and then the member where the sorting will
	case '2':												//take place. (OpAmps::SortByName())
		sort(OpAmp, OpAmp + database_length, SortBySlew);	//-As with the first case, except the member is
		break;												//(OpAmps::SortBySlew())
	case '3':
		return;												//Returns to main menu, no value
	default:
		cout << "INVALID ENTRY" << endl << endl;			//Case for a wrong value being entered
		break;
	}
}

void AllOpAmps::DisplayDatabase() const
{
	if (database_length == 0)
	{
		cerr << "DATABASE EMPTY" << endl; //Display this, return to menu if no data is available
	}

	else
	{
		cout << "-----------------------------------------" << endl;
		cout << "Number of Operational Amplifiers: " << database_length << endl << endl;
		cout << "Name" << "\t\t" << "Pin Count" << "\t" << "Slew Rate" << endl; //format for database
		for (unsigned long i = 0; i < database_length; i++)
		{
			OpAmp[i].DisplayOpAmp(); //runs the DisplayOpAmp for each OpAmp in the loop
		}
		cout << "-----------------------------------------" << endl; //format for database
	}
}

void AllOpAmps::ClearDatabase() //Clears whole database if user deems necessary
{
	for (unsigned long i = 0; i < database_length; i++)
	{
		OpAmp[i].ClearOpAmp(); //runs the ClearOpAmp for each OpAmp in the loop
	}
	database_length = 0; //reinitializes database length
	cout << "DATABASE CLEARED" << endl;
}

int main()
{
	AllOpAmps OpAmpDatabase; //OpAmpDatabase of class AllOpAmps
	OpAmpDatabase.Initialise(); //initialise, just in case
	char UserInput; //initialises user input

	while (1)
	{
		cout << endl;
		cout << "Op-amp database menu" << endl;
		cout << "--------------------" << endl;
		cout << "1. Enter a new op-amp into the database" << endl;
		cout << "2. Save the database to disk" << endl;
		cout << "3. Load the database from disk" << endl;
		cout << "4. Sort the database" << endl;
		cout << "5. Display the database" << endl;
		cout << "6. Clear the database" << endl;
		cout << "7. Exit from the program" << endl << endl;

		cout << "Select menu option: "; cin >> UserInput;
		cout << endl;

		switch (UserInput)	//loads 1 of the 7 cases, or default, depending on
		{					//the user input
		case '1':
			OpAmpDatabase.AddOpAmp();
			break;
		case '2':
			OpAmpDatabase.WriteToFile();
			break;
		case '3':
			OpAmpDatabase.ReadFromFile();
			break;
		case '4':
			OpAmpDatabase.SortDatabase();
			break;
		case '5':
			OpAmpDatabase.DisplayDatabase();
			break;
		case '6':
			OpAmpDatabase.ClearDatabase(); //additional extra
			break;
		case '7':
			return 0;
		default:
			cout << "Please enter a number from the menu" << endl;
			break;
		}
	}
}