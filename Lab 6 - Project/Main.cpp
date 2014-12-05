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

const string S_TO_D_COMMAND = "40000810";
const string D_TO_S_COMMAND = "40000C18";

unsigned int convertHexaToUnsignedInt (const string &hexaVariable);

int main(void)
{
	string address;
	string data;
	string size;
	string cycle;

	ifstream inFile;
	inFile.open("test_data.log");

	if (!inFile.is_open())
	{
		cout << "Can't open test_data.log" << endl;
		pause ("Press enter to exit.");
		exit(1);
	}
	do
	{
		do
		{
			inFile >> address;
		} while( address != S_TO_D_COMMAND && address != D_TO_S_COMMAND );

		inFile >> data;
		inFile >> size;
		inFile >> cycle;

		cout << "Address: " << address << endl;
		cout << "Data: " << data << endl;
		cout << "Size: " << size << endl;
		cout << "Cycle: " << cycle << endl;


		// Convert the hex in data variable to unsigned int
		unsigned int decimalValue = convertHexaToUnsignedInt (data);
		cout << "Number of words: " << decimalValue << endl;
		
		cout << endl;
	} while (getBool( "Keep looking? " ) && !inFile.eof());


	inFile.close();

	pause ("Press enter to exit.");
	return 0;
}

unsigned int convertHexaToUnsignedInt( const string &hexaVariable )
{
	unsigned int dataInBinary;
	stringstream ss;
	ss << hex << hexaVariable;
	ss >> dataInBinary;

	return dataInBinary;
}