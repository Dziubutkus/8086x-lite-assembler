/*
*************************************************************************************************
Author:			Dziugas Butkus
Contributing:	Israel Muro, Ian Oliver, Nick Shannon, Adam Gunell, Isaac Haas
Instructor:		Victoria Eisele
Title:			Project Assembler Part 4
Description:	A simulated processor with small memory and registers using a very extremely lite 8086x assembly code
Part 4:			Compares 2 integers and outputs the larger number
*************************************************************************************************
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctype.h>

using namespace std;
char ASM_FILE_NAME[] = "dziugas4.asm";

const int MAX = 150;  //size of simulators memory
const int COL = 7;	 //number of columns for output

// REGISTERS
const int AXREG = 0;
const int BXREG = 1;
const int CXREG = 2;
const int DXREG = 3;

// Commands
const int HALT = 5;
const int MOVREG = 192;
const int MOVMEM = 224;
const int ADDREG = 160;
const int CMP = 96;


enum paramType { reg, mem, constant, arrayBx, arrayBxPlus, none };

typedef short int Memory;

class Registers
{
public:
	int AX;
	int BX;
	int CX;
	int DX;
	int instrAddr;
	int flag;
} regis;

Memory memory[MAX] = { 0 };
int address;

void printMemoryDump(); // prints memeory with integers commands
void convertToMachineCode(ifstream &fin); // assembles code from.asm file and puts into memory
void convertToNumber(string line, int &start, int &value); // takes a line and converts digits to integer
int whichReg(char regLetter);  // returns the number of the register
void fillMemory(); // changes the code to machine code and places commands into memory
void runCode(); // read memory and compiles it
void changeToLowerCase(string &line); // changes strings to lower case letters
void splitCommand(string line, string &command, string &oper1, string &oper2); // splits a line from .asm file to command, oper1, oper2
int getValueFromReg(int botBits); // finds register by botBits and returns register's value

/*****************************************************************************
// convertToMachineCode - assembles code from .asm file and puts into memory
// Parameters:
// fin - a line from asm file
// Author: Dziugas Butkus
// Contributors: Israel Muro, Ian Oliver, Nick Shannon, Adam Gunell
*****************************************************************************/
void convertToMachineCode(ifstream &fin)
{
	string line; // full command
	string command; // the asm command
	string oper1, oper2; // the two operands, could be empty
	int machineCode = 1;
	int value; // temp value
	int value2; // temp value
	int length; // oper's lengths to remove '[]'
	int temp; // temporary int
	getline(fin, line, '\n');
	if (!empty(line)) // If line is not empty
	{
		changeToLowerCase(line);

		splitCommand(line, command, oper1, oper2);
		if (command[0] == 'h') // halt
		{
			memory[address] = HALT; // 0000 0101
			address++;
		}
		else if (command[0] == 'p')
		{
			machineCode = 7;
			memory[address] = machineCode;
			address++;
		}
		else if (command[0] == 'g')
		{
			machineCode = 6;
			memory[address] = machineCode;
			address++;
		}
		else if (command[0] == 'c') // cmp reg reg, cmp reg []
		{
			machineCode = CMP;
			machineCode += (whichReg(oper1[0]) << 3);
			if (oper2[0] == '[')
			{
				machineCode += 6;
			}
			else
			{
				machineCode += (whichReg(oper2[0]));
			}
			memory[address] = machineCode;
			address++;
		}
		else if (command[0] == 'j') // jump [address]
		{
			machineCode = 8;
			switch (command[1])
			{
			case 'n':
				machineCode += 1;
				break;
			case 'b':
				if (command[2] == 'e')
				{
					machineCode += 3;
				}
				else
				{
					machineCode += 2;
				}
				break;
			case 'a':
				if (command[2] == 'e')
				{
					machineCode += 5;
				}
				else
				{
					machineCode += 4;
				}
				break;
			case 'm':
				machineCode += 6;
				break;
			}
			memory[address] = machineCode;
			address++;
			// jump to address
			length = oper1.length();
			oper1 = oper1.substr(1, length - 2);
			value = stoi(oper1);
			memory[address] = value;
			address++;
		}
		else if (command[0] == 'm') // move into a register
		{
			if (isdigit(oper2[0]))
			{
				machineCode = MOVREG;
				machineCode += (whichReg(oper1[0]) << 3);
				machineCode += 7;
				memory[address] = machineCode;
				address++;
				value = stoi(oper2);
				memory[address] = value;
				address++;
			}
			else if (oper1[0] == '[') // mov [address] reg
			{
				machineCode = MOVMEM;
				machineCode += (whichReg(oper2[0]) << 3);
				machineCode += 6; // [xxxx] = 110
				length = oper1.length();
				oper1 = oper1.substr(1, length - 2);
				value = stoi(oper1);
				memory[address] = machineCode;
				address++;
				memory[address] = value;
				address++;
			}
			else if (oper2[0] == '[') // mov reg [address]
			{
				length = oper2.length();
				oper2 = oper2.substr(1, length - 2);
				value = stoi(oper2);
				machineCode = MOVREG;
				machineCode += (whichReg(oper1[0]) << 3);
				machineCode += 6; // address
				memory[address] = machineCode;
				address++;
				memory[address] = value;
				address++;
			}
			else if (!isdigit(oper2[0]) && !isdigit(oper1[0])) // mov reg reg
			{
				machineCode = MOVREG;
				machineCode += (whichReg(oper1[0]) << 3);
				machineCode += (whichReg(oper2[0]));
				memory[address] = machineCode;
				address++;
			}
		}
		else if (command[0] == 'a') // add reg constant
		{
			if (isdigit(oper2[0]))
			{
				machineCode = ADDREG;
				machineCode += (whichReg(oper1[0]) << 3);
				machineCode += 7;
				memory[address] = machineCode;
				address++;
				value = stoi(oper2);
				memory[address] = value;
				address++;
			}
			else if (!isdigit(oper2[0]) && !isdigit(oper1[0])) // add reg reg
			{
				machineCode = ADDREG;
				machineCode += (whichReg(oper1[0]) << 3);
				machineCode += (whichReg(oper2[0]));
				memory[address] = machineCode;
				address++;
			}
		}
		else if (empty(oper1) && empty(oper2))
		{
			value = stoi(command);
			memory[address] = value;
			address++;
		}
	}
	else // If line is empty increment address
	{
		address++;
	}
}

// SYSK based arch
// IBM assembler
int main()
{
	printMemoryDump();
	fillMemory();
	runCode( );
	printMemoryDump();

	cout << endl;
	system("pause");
	return 0;
}
/************************************************************/
/*fillMemory																*/
/*		changes the code to machine code and places the
commands into the memory. */
void fillMemory()
{
	address = 0;
	ifstream fin(ASM_FILE_NAME);
	if (fin.fail())
	{
		cout << "Can't read the file." << endl;
		system("pause");
		exit(1);
	}
	else
	{
		for (int i = 0; i < MAX && !fin.fail(); i++)
		{
			convertToMachineCode(fin);
			//cout << "*";
		}
	}
	
}

/************************************************************/
/*	splitCommand											*/
/*	splits asm line to command, oper1, oper2				*/
/*	Author: Dziugas Butkus									*/
/************************************************************/
void splitCommand(string line, string &command, string &oper1, string &oper2)
{
	int space, length1, length2;

	// Command
	space = line.find(' '); // finds where space is
	command.assign(line, 0, space); // line.substr
	length2 = command.length();
	length1 = line.length() - length2;
	line = line.substr(space + 1, length1);

	// Oper1
	space = line.find(' '); // finds where space is
	oper1.assign(line, 0, space); // line.substr
	length2 = oper1.length();
	length1 = line.length() - length2;
	line = line.substr(space + 1, length1);

	// Oper2
	space = line.find(' '); // finds where space is
	oper2.assign(line, 0, space); // line.substr
	length2 = oper1.length();
	length1 = line.length() - length2;
	line = line.substr(space + 1, length1);

}
/****************************************************************************/
/*	getValueFromReg															*/
/*	Finds the register by botBits, returns its value (for 'mov reg reg')	*/
/*	Author: Dziugas Butkus													*/
/****************************************************************************/
int getValueFromReg(int botBits)
{
	int value = 0;
	if (botBits == 0)
	{
		value = regis.AX;
		return value;
	}
	else if (botBits == 1)
	{
		value = regis.BX;
		return value;
	}
	else if (botBits == 2)
	{
		value = regis.CX;
		return value;
	}
	else if (botBits == 3)
	{
		value = regis.DX;
		return value;
	}
	return -1;
}
/************************************************************/
/*	whichReg												*/
/*	changes the letter of the registar to a number.			*/
/************************************************************/
int whichReg(char regLetter)
{
	if (regLetter == 'a')
	{
		return AXREG;
	}
	else if (regLetter == 'b')
	{
		return BXREG;
	}
	else if (regLetter == 'c')
	{
		return CXREG;
	}
	else if (regLetter == 'd')
	{
		return DXREG;
	}
	return -1;  //something went wrong if -1 is returned
}

/************************************************************/
/*ConvertToNumber															*/
/*  takes in a line and converts digits to a integer			*/
/*  line - is the string of assembly code to convert			*/
/*  start - is the location where the line is being coverted, 
		it starts at the beginning of number and it passed 
		back at the next location */
/*  value - is the integer value of the digits in the code	*/
void convertToNumber(string line, int &start, int &value)
{
	char number[16];
	bool negative = false;
//	cout<< "in convertToNumber before function 1  start is "<<start<<endl;
	int i = 0;
	if (line[start] == '-')
	{
		start++;
		negative = true;
	}
	while (i<16 && line.size() > start&& isdigit(line[start]))
	{
		number[i] = line[start];
					//	cout<<line[start];
		i++;
		start++;
				//		cout<<i<<start;
	}
	number[i] = '\0';
	value = atoi(number);
	if (negative == true)
	{
		value = -value;
	}
//	cout<< "in convertToNumber after function 1  start is "<<start<<endl;
}

/************************************************************/
/*	printMemoryCommands										*/
/*	prints memory with letter commands						*/
/************************************************************/
void printMemoryCommands()
{
	int i= 0;
	int count;  // which column is currently outputting
	int moveColumn = MAX / COL;
	int index=0;
	while ( i< MAX)
	{
		count = 0;
		while (count < COL && index < MAX)
		{
			index = i + count * moveColumn;
			//As each type of instruction is added you will need to be able to deal with type of command
			//printcommand(index)
		}
		cout<<endl;
			
	}
	cout<<"RA "<< regis.AX << "  BX "<< regis.BX << "  CX "<< regis.CX << "  DX "<< regis.DX<<endl;
	cout<<"\tinstruction "<<regis.instrAddr<<"\tflag "<<regis.flag<<"\n\n";
}


/******************************************************************************************************/
/*printMemoryDump																					  */
/*prints memory by number																			  */
/*MAX is the amount of elements in the memory array (Vicki used 100)								  */
/*COL is the number of columns that are to be displayed (Vicki used 7; was originally called COLUMNS) */
/******************************************************************************************************/
void printMemoryDump()
{
	int numRows=MAX/COL+1;   //number of rows that will print
	int carryOver=MAX%COL;   //number of columns on the bottom row
	int location;   //the current location being called
	for(int row=0;row<numRows;row++)
	{
		location=row;
		//cout << setw( 3 ) << location << "." << setw( 5 ) << memory[location];
		for(int column=0;location<MAX&&column<COL;column++)
		{
			if(!(numRows-1==row&&carryOver-1<column))
			{
				cout<<setw(5)<<location<<"."<<setw(5)<<memory[location];
				location+=(numRows-(carryOver-1<column));
			}
		}
		cout<<endl;
		//cout<<setw(3);
	}

	cout<<endl;
	cout<<"AX: "<<regis.AX<<'\t';
	cout<<"BX: "<<regis.BX<<'\t';
	cout<<"CX: "<<regis.CX<<'\t';
	cout<<"DX: "<<regis.DX<<'\t';
	cout<<endl<<endl;
	cout<<"Instruction: "<<regis.instrAddr<<endl;
	cout<<"Flag: "<<regis.flag;
	
	cout<<endl<<endl;
}


void changeToLowerCase(string &line)
{
	
	int index=0;
	while (index < line.size())
	{
		line[index] = tolower(line[index]);
		index++;
	}
//	cout<<"the line in change"<<line;

}

/*****************************************************************************
// runCode - reads memory and compiles it
// Author: Dziugas Butkus
// Contributors: Israel Muro, Ian Oliver, Nick Shannon, Adam Gunell
*****************************************************************************/
void runCode()
{
	Memory command;
	Memory topBits, midBits, botBits;
	Memory reg;
	int valueFromReg = 0;
	int temp; // temporary int
	int value;
	int value2;
	address = 0;
	while (memory[address] != 5)
	{
		command = memory[address];
		address++;
		topBits = command & 224;
		midBits = command & 24;
		botBits = command & 7;

		if (topBits == 0 && midBits == 0 && botBits == 6) // get
		{
			cout << "Enter a number: ";
			cin >> regis.AX; // Should check if valid integer
		}
		else if (topBits == 0 && midBits == 0 && botBits == 7)
		{
			cout << "AX register value: " << regis.AX << endl;
		}
		else if (topBits == CMP) 
		{
			if (botBits < 4) // cmp reg reg
			{
				reg = midBits >> 3;
				temp = reg;
				value = getValueFromReg(temp);
				temp = botBits;
				value2 = getValueFromReg(temp);

				if (value > value2)
				{
					regis.flag = 1;
				}
				else if (value < value2)
				{
					regis.flag = -1;
				}
				else if (value == value2)
				{
					regis.flag = 0;
				}
			}
			else if (botBits == 6) // cmp reg []
			{
				reg = midBits >> 3;
				temp = reg;
				value = getValueFromReg(temp);
				value2 = memory[memory[address - 2]];
				if (value > value2)
				{
					regis.flag = 1;
				}
				else if (value < value2)
				{
					regis.flag = -1;
				}
				else if (value == value2)
				{
					regis.flag = 0;
				}
			}
		}
		else if (topBits == 0 && midBits == 8) // jump
		{
			if (botBits == 0 && regis.flag == 0)
			{
				address = memory[address];
			}
			else if (botBits == 1 && regis.flag == 1 || botBits == 1 && regis.flag == -1)
			{
				address = memory[address];
			}
			else if (botBits == 2 && regis.flag == -1)
			{
				address = memory[address];
			}
			else if (botBits == 3 && regis.flag == -1 || botBits == 3 && regis.flag == 0)
			{
				address = memory[address];
			}
			else if (botBits == 4 && regis.flag == 1)
			{
				address = memory[address];
			}
			else if (botBits == 5 && regis.flag == 1 || botBits == 4 && regis.flag == 0)
			{
				address = memory[address];
			}
			else if (botBits == 6)
			{
				address = memory[address];
			}
		}
		else if (topBits == ADDREG)  // add reg reg
		{
			if (midBits < 4 && botBits < 4)
			{			
				reg = midBits >> 3;
				temp = botBits;
				valueFromReg = getValueFromReg(temp);
				if (reg == AXREG)
				{
					regis.AX += valueFromReg;
				}
				else if (reg == BXREG)
				{
					regis.BX += valueFromReg;
				}
				else if (reg == CXREG)
				{
					regis.CX += valueFromReg;
				}
				else if (reg == DXREG)
				{
					regis.DX += valueFromReg;
				}
			}
			else if (botBits == 7) // add reg constant
			{
				if (reg == AXREG)
				{
					regis.AX += memory[address];
					address++;
				}
				else if (reg == BXREG)
				{
					regis.BX += memory[address];
					address++;
				}
				else if (reg == CXREG)
				{
					regis.CX += memory[address];
					address++;
				}
				else if (reg == DXREG)
				{
					regis.DX += memory[address];
					address++;
				}
			}
		}
		else if (topBits == MOVREG)
		{
			reg = midBits >> 3;
			if (botBits < 4 && reg < 4) // MOV reg reg **************************************
			{	
				temp = botBits;
				valueFromReg = getValueFromReg(temp);
				if (reg == AXREG)
				{
					regis.AX = valueFromReg;
				}
				else if (reg == BXREG)
				{
					regis.BX = valueFromReg;
				}
				else if (reg == CXREG)
				{
					regis.CX = valueFromReg;
				}
				else if (reg == DXREG)
				{
					regis.DX = valueFromReg;
				}
			}
			else if (botBits == 7) // MOV reg constant
			{
				if (reg == AXREG)
				{
					regis.AX = memory[address];
					address++;
				}
				else if (reg == BXREG)
				{
					regis.BX = memory[address];
					address++;
				}
				else if (reg == CXREG)
				{
					regis.CX = memory[address];
					address++;
				}
				else if (reg == DXREG)
				{
					regis.DX = memory[address];
					address++;
				}
			}
			else if (botBits == 6) // MOV reg mem **************************
			{
				if (reg == AXREG)
				{
					regis.AX = memory[memory[address]];
				}
				else if (reg == BXREG)
				{
					regis.BX = memory[memory[address]];
				}
				else if (reg == CXREG)
				{
					regis.CX = memory[memory[address]];
				}
				else if (reg == DXREG)
				{
					regis.DX = memory[memory[address]];
				}
			}
		}
		else if (topBits == MOVMEM)
		{
			reg = midBits >> 3;
			if (botBits == 6) // MOV mem reg
			{
				if (reg == AXREG)
				{
					memory[memory[address]] = regis.AX;
					address++;
				}
				else if (reg == BXREG)
				{
					memory[memory[address]] = regis.BX;
					address++;
				}
				else if (reg == CXREG)
				{
					memory[memory[address]] = regis.CX;
					address++;
				}
				else if (reg == DXREG)
				{
					memory[memory[address]] = regis.DX;
					address++;
				}
			
		}
		
		}
	}
}
	
