// Bounpaseuth Saysavath
// Lab 6
// CS 254

#include"Tools.h"

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;
using namespace tools_namespace;

const string S_TO_D_COMMAND_VALUE = "40000810";
const string D_TO_S_COMMAND_VALUE = "40000C18";

void outputToScreen (const string &address, const string &data, const int &size, const string &cycle);
unsigned int convertHexaToUnsignedInt (const string &hexVariable);
int convertSizeTypeToInt (const string &sizeType);

int main(void)
{
	ifstream inFile;
	inFile.open("test_data.log");

	if (!inFile.is_open())
	{
		cout << "Can't open test_data.log" << endl;
		pause ("Press enter to exit.");
		exit(1);
	}

	string address;
	string data;
	string size;
	string cycle;

	do
	{
		do
		{
			inFile >> address;
		} while( address != S_TO_D_COMMAND_VALUE && address != D_TO_S_COMMAND_VALUE );

		inFile >> data;
		inFile >> size;
		inFile >> cycle;

		int sizeInInt = convertSizeTypeToInt( size );

		// Output the variables to the screen
		outputToScreen (address, data, sizeInInt, cycle);

		// Convert the hex in data variable to unsigned int
		unsigned int wordCount = convertHexaToUnsignedInt (data);
		cout << "Number of words: " << wordCount << endl;
		
		//if (wordCount > 0)
		//{
		//	if (address == S_TO_D_COMMAND_VALUE)
		//	{
		//		// search from the address of 40000818 to 40000C14
		//		int startSearchingAddress = 40000818;
		//		
		//		do
		//		{
		//			do
		//			{
		//				inFile >> address;
		//			} while( stoi(address) != startSearchingAddress );

		//		} while ();
		//	}
		//	else
		//		// search from the address of 40000C20 to 4000101C 
		//}

		cout << endl;
	} while (getBool( "Keep looking? " ) && !inFile.eof());


	inFile.close();

	pause ("Press enter to exit.");
	return 0;
}

void outputToScreen( const string &address, const string &data, const int &size, const string &cycle )
{
	cout << "Command Type: " << ((address == S_TO_D_COMMAND_VALUE) ? "S-to-D Command" : "D-to-S Command") << endl;
	cout << "Address: " << address << endl;
	cout << "Data: " << data << endl;
	cout << "Size: " << size << endl;
	cout << "Cycle: " << cycle << endl;
}

unsigned int convertHexaToUnsignedInt( const string &hexVariable )
{
	return stoi(hexVariable, nullptr, 16);
}

// Size comes with D32 or D64. We need to get rid of the D and grab the number.
int convertSizeTypeToInt( const string &sizeType )
{
	string str = sizeType.substr(1,2);
	return stoi(str);
}