/*
*************************************************************************************************
Author:			Dziugas Butkus
Contributing:	Israel Muro, Ian Oliver, Nick Shannon, Adam Gunell, Jared C, Isaac
Instructor:		Victoria Eisele
Title:			Project Assembler
Due:			-
Submitted:		12/9/2017
Description:	A simulated processor with small memory and registers using a very extremely lite 8086x assembly code
Part 6:			Input 10 numbers into an array which are bigger than 0 or smaller than 100, add them and output the sum
Part 7:			Input 10 numbers into an array which are bigger than 0 or smaller than 100, add them and return sum by reference
Part 8:			Input 10 numbers into an array and sort them using bubble sort
Part 9:			Create a linked list, user inputs numbers until he enters 0 or exceeds the memory limit
*************************************************************************************************
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctype.h>

using namespace std;
char ASM_FILE_NAME[] = "linkedlist.asm";

//const int MAX = 570;  //size of simulators memory
//const int COL = 15;	 //number of columns for output
const int MAX = 200;
const int COL = 10;

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
const int SUBREG = 128;
const int CMP = 96;
const int JUMP = 8;


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
	int stack = MAX - 1;
} regis;

Memory memory[MAX] = { 0 };
int address;

void printMemoryDump(); // prints memeory with integers commands
void convertToMachineCode(ifstream &fin); // assembles code from .asm file and puts into memory
void convertToNumber(string line, int &start, int &value); // takes a line and converts digits to integer
int whichReg(char regLetter);  // returns the number of the register
void fillMemory(); // changes the code to machine code and places commands into memory
void runCode(); // read memory and compiles it
void changeToLowerCase(string &line); // changes strings to lower case letters
void splitCommand(string line, string &command, string &oper1, string &oper2); // splits a line from .asm file to command, oper1, oper2
int getValueFromReg(int botBits); // finds register by botBits and returns register's value
int removeBrackets(string oper); // removes brackets from operand and return the value
void processOperand(int &machineCode, string oper1, string oper2); // deals with repetitive code from convertToMachineCode(), takes care of machineCode and memory

/*****************************************************************************
// convertToMachineCode - assembles code from .asm file and puts into memory
// Parameters:
// fin - a line from asm file
// Author: Dziugas Butkus
// Contributors: Israel Muro, Ian Oliver, Nick Shannon, Adam Gunell, Isaac
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
		else if (command[0] == 's')
		{
			machineCode = SUBREG;
			processOperand(machineCode, oper1, oper2);
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
		else if (command[0] == 'f') // fun [] parameters
		{
			int params; // number of parameters
			memory[address] = 3;
			address++;
			value = removeBrackets(oper1);
			memory[address] = value;
			address++;
			value = stoi(oper2);
			params = value;
			memory[address] = value;
			address++;
			for (int i = 0; i < params; i++)
			{
				getline(fin, line, '\n');
				changeToLowerCase(line);
				splitCommand(line, command, oper1, oper2);
				if (isdigit(command[0]))
				{
					memory[address] = 7; // Parameter is constant
					address++;
					memory[address] = stoi(command); // Parameter's value
					address++;
				}
				else if (command[0] == 'a' || command[0] == 'b' || command[0] == 'c' || command[0] == 'd')
				{
					value = whichReg(command[0]);
					memory[address] = value;
					address++;
				}

				else if (command[0] == '[')
				{
					value = removeBrackets(command);
					memory[address] = 6;
					address++;
					memory[address] = value;
					address++;
				}
			}
		}
		else if (command[0] == 'r')
		{
			memory[address] = 4;
			address++;
			value = removeBrackets(oper1);
			memory[address] = value;
			address++;
		}
		else if (command[0] == 'c') // cmp
		{
			machineCode = CMP;
			processOperand(machineCode, oper1, oper2);
		}
		else if (command[0] == 'j') // jump [address]
		{
			machineCode = JUMP;
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
			value = removeBrackets(oper1);
			memory[address] = value;
			address++;
		}
		else if (command[0] == 'm') // move into a register
		{
			if (oper1[0] == '[') // mov [address] reg
			{
				machineCode = MOVMEM;
			}
			else
			{
				machineCode = MOVREG;
			}
			processOperand(machineCode, oper1, oper2);
		}
		else if (command[0] == 'a') // add reg constant
		{
			machineCode = ADDREG;
			processOperand(machineCode, oper1, oper2);
		}
		else if (command[0] == 'd')
		{
			memory[address] = 1;
			address++;
		}
		else if (empty(oper1) && empty(oper2) && command != "")
		{
			value = stoi(command);
			memory[address] = value;
			address++;
		}
		else
		{
			address++;
		}
	}
	else // If line is empty increment address
	{
		address++;
	}
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
		topBits = command & 224;
		midBits = command & 24;
		botBits = command & 7;
		reg = midBits >> 3;
		address++;
		//cout << address << " | " << topBits << " " << midBits << " " << botBits << endl;

		if (topBits == 0 && midBits == 0 && botBits == 3)
		{
			// Dump Registers, Stack, Flag and Return Address
			memory[regis.stack] = regis.AX;
			regis.stack -= 1;
			memory[regis.stack] = regis.BX;
			regis.stack -= 1;
			memory[regis.stack] = regis.CX;
			regis.stack -= 1;
			memory[regis.stack] = regis.DX;
			regis.stack -= 1;
			memory[regis.stack] = regis.flag;
			regis.stack -= 1;

			int functionAddress = memory[address];
			temp = memory[address + 1]; // How many parameters passed to function
			address += 2;
			for (int i = 1; i <= temp; i++) // Loop goes until there are parameters
			{
				if (memory[address] == 7)
				{
					memory[functionAddress - i] = memory[address + 1]; // Put parameter functionAddress - i
					address += 2;
				}
				else if (memory[address] == 6)
				{
					memory[functionAddress - i] = memory[memory[address + 1]]; // Put parameter functionAddress - i
					address += 2;
				}
				else if (memory[address] < 4)
				{
					value = getValueFromReg(memory[address]);
					memory[functionAddress - i] = value; // Put parameter functionAddress - i
					address++;
				}
			}
			memory[regis.stack] = address; // Dump address which we have to return to after fun
			address = functionAddress;
		}
		if (topBits == 0 && midBits == 0 && botBits == 4) // Ret
		{
			memory[memory[address]] = regis.AX;
			address = memory[regis.stack];
			//cout << address << endl;
			// Take registers and flag back from stack
			regis.flag = memory[regis.stack + 1];
			regis.DX = memory[regis.stack + 2];
			regis.CX = memory[regis.stack + 3];
			regis.BX = memory[regis.stack + 4];
			regis.AX = memory[regis.stack + 5];
			regis.stack = MAX - 1; // Reset stack to the end of the memory
		}
		else if (topBits == CMP)
		{
			temp = reg;
			value = getValueFromReg(temp);
			// Set values
			if (botBits < 4) // cmp reg reg
			{
				temp = botBits;
				value2 = getValueFromReg(temp);
			}
			else if (botBits == 6) // cmp reg []
			{
				value2 = memory[memory[address]];
				address++;
			}
			else if (botBits == 4) // cmp reg [bx]
			{
				value2 = memory[regis.BX];
				address++;
			}
			else if (botBits == 5)
			{
				value2 = memory[regis.BX + memory[address]];
				address++;
			}
			else if (botBits == 7) // cmp reg constant
			{
				value2 = memory[address];
				address++;
			}
			// Compare values
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
		else if (topBits == 0 && midBits == JUMP) // jump
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
			else if (botBits == 5 && regis.flag == 1 || botBits == 5 && regis.flag == 0)
			{
				address = memory[address];
			}
			else if (botBits == 6)
			{
				address = memory[address];
			}
			else
			{
				address++;
			}
			//address++;
		}
		else if (topBits == SUBREG)
		{
			if (reg == AXREG)
			{
				regis.AX -= memory[address];
				address++;
			}
			else if (reg == BXREG)
			{
				regis.BX -= memory[address];
				address++;
			}
			else if (reg == CXREG)
			{
				regis.CX -= memory[address];
				address++;
			}
			else if (reg == DXREG)
			{
				regis.DX -= memory[address];
				address++;
			}
		}
		else if (topBits == ADDREG)  // add reg reg
		{
			if (reg < 4 && botBits < 4)
			{
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
			else if (botBits == 6) // add reg []
			{
				if (reg == AXREG)
				{
					regis.AX += memory[memory[address]];
					address++;
				}
				else if (reg == BXREG)
				{
					regis.BX += memory[memory[address]];
					address++;
				}
				else if (reg == CXREG)
				{
					regis.CX += memory[memory[address]];
					address++;
				}
				else if (reg == DXREG)
				{
					regis.DX += memory[memory[address]];
					address++;
				}
			}
		}
		else if (topBits == MOVREG)
		{
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
			else if (reg < 4 && botBits == 7) // MOV reg constant
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
			else if (reg < 4 && botBits == 6) // MOV reg mem **************************
			{
				if (reg == AXREG)
				{
					regis.AX = memory[memory[address]];
					address++;
				}
				else if (reg == BXREG)
				{
					regis.BX = memory[memory[address]];
					address++;
				}
				else if (reg == CXREG)
				{
					regis.CX = memory[memory[address]];
					address++;
				}
				else if (reg == DXREG)
				{
					regis.DX = memory[memory[address]];
					address++;
				}
			}
			else if (botBits == 5)
			{
				if (reg == AXREG)
				{
					regis.AX = memory[regis.BX + memory[address]];
				}
				else if (reg == BXREG)
				{
					regis.BX = memory[regis.BX + memory[address]];
				}
				else if (reg == CXREG)
				{
					regis.CX = memory[regis.BX + memory[address]];
				}
				else if (reg == DXREG)
				{
					regis.DX = memory[regis.BX + memory[address]];
				}
				address++;
			}
			else if (reg < 4 && botBits == 4)
			{
				if (reg == AXREG)
				{
					regis.AX = memory[regis.BX];
				}
				else if (reg == BXREG)
				{
					regis.BX = memory[regis.BX];
				}
				else if (reg == CXREG)
				{
					regis.CX = memory[regis.BX];
				}
				else if (reg == DXREG)
				{
					regis.DX = memory[regis.BX];
				}
			}
		}
		else if (topBits == MOVMEM)
		{
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
			else if (botBits == 5)
			{
				if (reg == AXREG)
				{
					memory[regis.BX + memory[address]] = regis.AX;
				}
				else if (reg == BXREG)
				{
					memory[regis.BX + memory[address]] = regis.BX;
				}
				else if (reg == CXREG)
				{
					memory[regis.BX + memory[address]] = regis.CX;
				}
				else if (reg == DXREG)
				{
					memory[regis.BX + memory[address]] = regis.DX;
				}
				address++;
			}
			else if (botBits == 4)
			{
				if (reg == AXREG)
				{
					memory[regis.BX] = regis.AX;
				}
				else if (reg == BXREG)
				{
					memory[regis.BX] = regis.BX;
				}
				else if (reg == CXREG)
				{
					memory[regis.BX] = regis.CX;
				}
				else if (reg == DXREG)
				{
					memory[regis.BX] = regis.DX;
				}
			}
		}
		else if (topBits == 0 && midBits == 0 && botBits == 6)
		{
			cout << "Enter a number: ";
			cin >> regis.AX;
		}
		else if (topBits == 0 && midBits == 0 && botBits == 7)
		{
			cout << "AX register value: " << regis.AX << endl;
		}
		else if (topBits == 0 && midBits == 0 && botBits == 1)
		{
			printMemoryDump();
		}
	}
}

int main()
{
	fillMemory();
	printMemoryDump();
	runCode();
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
		}
	}

}

/*****************************************************************************
// processOperand - deals with repetitive code from convertToMachineCode(), takes care of machineCode and memory
// Parameters:
// machineCode, oper1, oper2
// Author: Dziugas Butkus
*****************************************************************************/
void processOperand(int &machineCode, string oper1, string oper2)
{
	int value;
	// oper1
	if (oper1[0] == '[') // mov [] reg
	{
		machineCode += (whichReg(oper2[0]) << 3);
		if (isdigit(oper1[1]))
		{
			machineCode += 6;
			memory[address] = machineCode;
			address++;
			value = removeBrackets(oper1);
			memory[address] = value;
			address++;
		}
		else if (oper1[3] == '+')
		{
			machineCode += 5;
			memory[address] = machineCode;
			address++;
			string operTemp;
			int valueTemp = oper1.length() - 2;
			operTemp = oper1.substr(4, valueTemp);
			memory[address] = stoi(operTemp);
			address++;
		}
		else if (oper1[1] == 'b' && oper1[oper1.length() - 1] == ']')
		{
			machineCode += 4;
			memory[address] = machineCode;
			address++;
		}
	}
	else
	{
		if (!isdigit(oper1[1])) // reg
		{
			machineCode += (whichReg(oper1[0]) << 3);
		}
		// oper2
		if (isdigit(oper2[0])) // constant
		{
			machineCode += 7;
			memory[address] = machineCode;
			address++;
			value = stoi(oper2);
			memory[address] = value;
			address++;
		}
		else if (oper2[0] != '[' && !isdigit(oper2[1])) // reg
		{
			machineCode += (whichReg(oper2[0]));
			memory[address] = machineCode;
			address++;
		}
		else if (oper2[0] == '[' && isdigit(oper2[1]))
		{
			machineCode += 6;
			memory[address] = machineCode;
			address++;
			value = removeBrackets(oper2);
			memory[address] = value;
			address++;
		}
		else if (oper2[0] == '[' && !isdigit(oper2[1]) && oper2[3] != '+')
		{
			machineCode += 4;
			memory[address] = machineCode;
			address++;
		}
		else if (oper2[0] == '[' && oper2[3] == '+')
		{
			machineCode += 5;
			memory[address] = machineCode;
			address++;
			string operTemp;
			int valueTemp = oper2.length() - 2;
			operTemp = oper2.substr(4, valueTemp);
			memory[address] = stoi(operTemp);
			address++;
		}
	}
}

/*****************************************************************************
// removeBrackets -removes brackets from operand and return the value
// Parameters:
// oper - operand from .asm file
// Return:
// value - the value of
// Author: Dziugas Butkus
*****************************************************************************/
int removeBrackets(string oper)
{
	int length, value;
	length = oper.length();
	oper = oper.substr(1, length - 2);
	value = stoi(oper);
	return value;
}

/************************************************************/
/*	splitCommand											*/
/*	splits asm line to command, oper1, oper2				*/
/*	Author: Dziugas Butkus									*/
/************************************************************/
void splitCommand(string line, string &command, string &oper1, string &oper2)
{
	int space, length1, length2, semicolon;

	//Remove comment
	semicolon = line.find(';');
	if (semicolon != 0)
	{
		line = line.substr(0, semicolon);
	}
	if (semicolon == 0)
	{
		command = "";
	}
	else
	{
		// Command
		space = line.find(' '); // finds where space is
		if (space != string::npos)
		{
			command.assign(line, 0, space); // line.substr
			length2 = command.length();
			length1 = line.length() - length2;
			line = line.substr(space + 1, length1);


			// Oper1
			space = line.find(' '); // finds where space is
			if (space != string::npos)
			{
				oper1.assign(line, 0, space); // line.substr
				length2 = oper1.length();
				length1 = line.length() - length2;
				line = line.substr(space + 1, length1);
			}
			else
			{
				oper1 = line;
			}
			// Oper2
			space = line.find(' '); // finds where space is
			oper2.assign(line, 0, space); // line.substr
			length2 = oper1.length();
			length1 = line.length() - length2;
			line = line.substr(space + 1, length1);

		}
		else
		{
			command = line;
		}
	}

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
	int i = 0;
	int count;  // which column is currently outputting
	int moveColumn = MAX / COL;
	int index = 0;
	while (i< MAX)
	{
		count = 0;
		while (count < COL && index < MAX)
		{
			index = i + count * moveColumn;
			//As each type of instruction is added you will need to be able to deal with type of command
			//printcommand(index)
		}
		cout << endl;

	}
	cout << "RA " << regis.AX << "  BX " << regis.BX << "  CX " << regis.CX << "  DX " << regis.DX << endl;
	cout << "\tinstruction " << regis.instrAddr << "\tflag " << regis.flag << "\n\n";
}


/******************************************************************************************************/
/*printMemoryDump																					  */
/*prints memory by number																			  */
/*MAX is the amount of elements in the memory array (Vicki used 100)								  */
/*COL is the number of columns that are to be displayed (Vicki used 7; was originally called COLUMNS) */
/******************************************************************************************************/
void printMemoryDump()
{
	int numRows = MAX / COL + 1;   //number of rows that will print
	int carryOver = MAX%COL;   //number of columns on the bottom row
	int location;   //the current location being called
	for (int row = 0; row<numRows; row++)
	{
		location = row;
		//cout << setw( 3 ) << location << "." << setw( 5 ) << memory[location];
		for (int column = 0; location<MAX&&column<COL; column++)
		{
			if (!(numRows - 1 == row&&carryOver - 1<column))
			{
				cout << setw(5) << location << "." << setw(5) << memory[location];
				location += (numRows - (carryOver - 1<column));
			}
		}
		cout << endl;
		//cout<<setw(3);
	}

	//cout << endl;
	cout << "AX: " << regis.AX << '\t';
	cout << "BX: " << regis.BX << '\t';
	cout << "CX: " << regis.CX << '\t';
	cout << "DX: " << regis.DX << '\t';
	cout << endl << endl;
	cout << "Address: " << address << endl;
	cout << "Flag: " << regis.flag << endl;
	//cout << "Stack: " << regis.stack;

	cout << endl;
}


void changeToLowerCase(string &line)
{

	int index = 0;
	while (index < line.size())
	{
		line[index] = tolower(line[index]);
		index++;
	}
	//	cout<<"the line in change"<<line;

}

// No Problems