// Michael Cuvin, Jeremy Mann, Kyle Fox, Bounpaseuth Saysavath
// Lab 6
// CS 254

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<sstream>

using namespace std;

const string S_TO_D_COMMAND_VALUE = "40000810";
const string D_TO_S_COMMAND_VALUE = "40000C18";
const int NUMBER_OF_WORDS_IN_A_LINE = 14;

void outputToScreen( const string &address, const string &data, const int &size, const string &cycle );
unsigned int convertHexaToUnsignedInt( const string &hexVariable );
int convertSizeTypeToInt( const string &sizeType );

// Tool Function to pause the screen
void pause( std::string prompt = "Press ENTER to continue..." );
// Funtion to get boolean from user
bool	getBool( std::string prompt );

int main( void )
{
	ifstream inFile;
	int numberOfWordEncounter = 0;
	inFile.open( "test_data.log" );

	if( !inFile.is_open( ) )
	{
		cout << "Can't open test_data.log" << endl;
		pause( "Press enter to exit." );
		exit( 1 );
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
			numberOfWordEncounter++;
		} while( address != S_TO_D_COMMAND_VALUE && address != D_TO_S_COMMAND_VALUE );

		inFile >> data;
		inFile >> size;
		inFile >> cycle;

		numberOfWordEncounter += 3;

		int sizeInInt = convertSizeTypeToInt( size );

		// Output the variables to the screen
		outputToScreen( address, data, sizeInInt, cycle );

		// Convert the hex in data variable to unsigned int
		unsigned int wordCount = convertHexaToUnsignedInt( data );
		cout << "Number of words: " << wordCount << endl;

		// Output line number in a log file of the current address
		cout << "Line number: " << (numberOfWordEncounter / NUMBER_OF_WORDS_IN_A_LINE) + 1 << endl;

		//if (wordCount > 0)
		//{
		//	if (address == S_TO_D_COMMAND_VALUE)
		//	{
		//		// search from the address of 40000818 to 40000C14
		//		int startSearchingAddress = 0x40000818;
		//		int stopSearchingAddress = 0x40000C14;
		//		string nextWordOnALine;
		//		
		//		do
		//		{
		//			do
		//			{
		//				inFile >> nextWordOnALine;
		//			} while( nextWordOnALine != "40000818" );

		//			inFile >> data;

		//		} while ();
		//	}
		//	else
		//		// search from the address of 40000C20 to 4000101C 
		//}

		cout << endl;
	} while( getBool( "Keep looking?" ) && !inFile.eof( ) );


	inFile.close( );

	pause( "Press enter to exit." );
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
	// return stoi(hexVariable, nullptr, 16);
	// Alternative to stoi

	unsigned int x;
	stringstream ss;
	ss << hex << hexVariable;
	ss >> x;

	return x;
}

int convertSizeTypeToInt( const string &sizeType )
{
	// Size comes with D32 or D64. We need to get rid of the D and grab the number.
	string str = sizeType.substr( 1, 2 );
	// return stoi(str);
	// Alternative to stoi

	unsigned int x;
	stringstream ss;
	ss << dec << str;
	ss >> x;

	return x;
}

// assumes that keyboard input buffer is empty
void pause( std::string prompt )
{
	std::cout << prompt;
	std::cin.ignore( 999, '\n' );
}

bool getBool( std::string prompt )
{
	do
	{
		std::cout << prompt << ": ";
		char userAnswer;
		cin >> userAnswer;
		std::cout << endl;
		cin.ignore( 99, '\n' );
		if( userAnswer == 'y' || userAnswer == 'Y' ) return true;
		if( userAnswer == 'n' || userAnswer == 'N' ) return false;
		std::cout << "Enter y, yes, n, or no." << std::endl;
	} while( true );

}