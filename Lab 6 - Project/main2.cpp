// Michael Cuvin, Jeremy Mann, Kyle Fox, Bounpaseuth Saysavath

// Lab 6

// CS 254



#include <iostream>

#include <fstream>

#include <string>

#include <cstdlib>

#include <sstream>

#include <vector>

#include <bitset>



using namespace std;



const string S_TO_D_COMMAND_VALUE = "40000810";

const string D_TO_S_COMMAND_VALUE = "40000C18";



const int S_TO_D_COMMAND_START_ADDRESS = 0x40000818;

const int S_TO_D_COMMAND_END_ADDRESS = 0x40000C14;

const int D_TO_S_COMMAND_START_ADDRESS = 0x40000C20;

const int D_TO_S_COMMAND_END_ADDRESS = 0x4000101C;

const int NUMBER_OF_WORDS_IN_A_LINE = 14;



void output( const string &address, const string &data, const string &cycle, const int &lineNumber, const int &wordCount );

unsigned int convertHexStringToUnsignedInt( const string &hexVariable );

string hexStringToBinaryString( const string &hexString );

string hexCharToBinaryString( const char &getChar );

//int convertHexIntToDecInt( const int &hexValue );

int convertStringToInt( const string &stringValue );

//string convertHexIntToDecimalString( const int &hexValue );

int getLineNumber( const int &numberOfWordEncounter );

bool addressMatch( const string &currentAdrress, const vector<string> &addressRange, bool &lowToHigh );



vector<string> generateAddressRange( const int &start, const int &end, int increment = 4 );



// Tool Function to pause the screen

void pause( std::string prompt = "Press ENTER to continue..." );

// Funtion to get boolean from user

bool getBool( std::string prompt );

// Convert a string to uppercase

std::string toUpper( std::string s );



int main( int argc, char *argv[] )

{

	ifstream inFile;

	inFile.open( "test_data.log" );



	if( !inFile.is_open( ) )

	{

		cout << "Can't open test_data.log" << endl;

		pause( "Press enter to exit." );

		exit( 1 );

	}



	int numberOfWordEncounter = 0;

	string address;

	string data;

	string size;

	string cycle;

	

	vector<string> SToDCommandAddressRange = generateAddressRange( S_TO_D_COMMAND_START_ADDRESS, S_TO_D_COMMAND_END_ADDRESS );

	vector<string> DToSCommandAddressRange = generateAddressRange( D_TO_S_COMMAND_START_ADDRESS, D_TO_S_COMMAND_END_ADDRESS );



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



		/*int sizeInInt = convertSizeTypeToInt( size );*/

		int wordCount = convertHexStringToUnsignedInt( data ) / 2;



		// Output the variables to the screen

		output( address, data, cycle, getLineNumber( numberOfWordEncounter ), wordCount );



		if (wordCount > 0)

		{

			vector<string> wordsData;

			vector<int> lineNumber;

			vector<string> addressRange;



			if( address == S_TO_D_COMMAND_VALUE )

			{

				// search from the address of 40000818 to 40000C14

				addressRange = SToDCommandAddressRange;

			}

			else if( address == D_TO_S_COMMAND_VALUE )

			{

				// search from the address of 40000C20 to 4000101C

				addressRange = DToSCommandAddressRange;

			}

			else

			{

				cout << "Fatal error." << endl;

				pause();

				exit (1);

			}



			string nextWordOnALine;

			int loopCount = 0;

			bool stopSearch = false;

			bool lowToHigh = true;

				

			do

			{

				inFile >> nextWordOnALine;

				numberOfWordEncounter++;

				loopCount++;

					



				if( loopCount == 11 && !addressMatch( nextWordOnALine, addressRange, lowToHigh ) )

				{

					stopSearch = true;

					break;

				}



			} while(  !addressMatch( nextWordOnALine, addressRange, lowToHigh ) );

			

			loopCount = 0;

			inFile >> nextWordOnALine;

			numberOfWordEncounter++;



			do

			{

				if( stopSearch == true )

					break;



				// Store data in a  wordsData queue file structure

				wordsData.push_back( nextWordOnALine.substr( 0, 4 ) );

				wordsData.push_back( nextWordOnALine.substr( 4, 8 ) );



				lineNumber.push_back( getLineNumber( numberOfWordEncounter ) );

				lineNumber.push_back( getLineNumber( numberOfWordEncounter ) );



				// Discard the next fourteen words

				for( int i = 1; i <= 14; i++ )

				{

					inFile >> nextWordOnALine;

					numberOfWordEncounter++;

				}



				loopCount++;



			} while( loopCount < (wordCount / 2) );



			cout << "Data in this command are:" << endl;

			if( !lowToHigh )

				cout << "High to Low" << endl;



			for( int wordNumber = 0;( wordNumber < (wordsData.size()) ); wordNumber++ )

			{

				cout << "Line " << lineNumber[wordNumber] << ": ";

				cout << ( hexStringToBinaryString( wordsData[wordNumber] ) ) << endl;

				//unsigned short shortInt = ((convertStringToInt( hexStringToBinaryString( wordsData[wordNumber] ) ) >> intArray[wordNumber]) );

				//cout << shortInt << endl;

				//bitset<16> bitset1{shortInt};

				//cout << bitset1 << endl;

			}

		}



		cout << endl;

	} while( getBool( "Keep looking?" ) && !inFile.eof( ) );





	inFile.close( );



	pause( "Press enter to exit." );

	return 0;

}



void output( const string &address, const string &data, const string &cycle, const int &lineNumber, const int &wordCount )

{

	cout << "Line " << lineNumber << ": " << ( (cycle == "Rd")? "Read " : "Write " ); 

	cout << ((address == S_TO_D_COMMAND_VALUE) ? "S-to-D Command: " : "D-to-S Command: ");

	cout << wordCount << " words" << endl;

}



unsigned int convertHexStringToUnsignedInt( const string &hexVariable )

{

	// return stoi(hexVariable, nullptr, 16);

	// Alternative to stoi



	unsigned int x;

	stringstream ss;

	ss << hex << hexVariable;

	ss >> x;



	return x;

}



string hexStringToBinaryString( const string &hexString )

{

	string stringBuilder;



	for( string::size_type index = 0; index < hexString.size(); index++ )

	{

		stringBuilder += hexCharToBinaryString( hexString[index] );

	}



	return stringBuilder;

}



string hexCharToBinaryString( const char &getChar )

{

	switch( getChar )

	{

	case '0':	return "0000";

	case '1':	return "0001";

	case '2':	return "0010";

	case '3':	return "0011";

	case '4':	return "0100";

	case '5':	return "0101";

	case '6':	return "0110";

	case '7':	return "0111";

	case '8':	return "1000";

	case '9':	return "1001";

	case 'A':	return "1010";

	case 'B':	return "1011";

	case 'C':	return "1100";

	case 'D':	return "1101";

	case 'E':	return "1110";

	case 'F':	return "1111";

	default:

		return "EROR";

		break;

	}

}



//int convertHexIntToDecInt( const int &hexValue )

//{

//	int x;

//	stringstream ss;

//	ss << hex << hexValue;

//	ss >> x;

//

//	return x;

//}



int convertStringToInt( const string &stringValue )

{

	// Size comes with D32 or D64. We need to get rid of the D and grab the number.

	//string str = sizeType.substr( 1, 2 );

	return stoi(stringValue, nullptr, 2);

	// Alternative to stoi



	/*long int x;

	stringstream ss;

	ss << stringValue;

	ss >> x;



	return x;*/

}



//string convertHexIntToDecimalString( const int &hexValue )

//{

//	stringstream ss;

//	ss << dec << hexValue;

//	

//	return ss.str();

//}



int getLineNumber( const int &numberOfWordEncounter )

{

	return (numberOfWordEncounter / NUMBER_OF_WORDS_IN_A_LINE) + 1;

}



bool addressMatch( const string &currentAdrress, const vector<string> &addressRange, bool &lowToHigh )

{

	for( unsigned int index = 0; index < addressRange.size(); index++ )

	{

		if( currentAdrress == addressRange[index] )

		{

			if( index > 0 )

				lowToHigh = false;

			return true;

		}

	}



	return false;

}



vector<string> generateAddressRange( const int &start, const int &end, int increment )

{

	int range = ( end - start ) / 2;

	int hexValue = start;



	vector<string> temp(range);



	for( int count = 0; count < range; count++ )

	{

		stringstream ss;

		ss << hex << hexValue;

		temp[count] = toUpper( ss.str() );

		hexValue += increment;

	}



	return temp;

}



// assumes that keyboard input buffer is empty

void pause( std::string prompt )

{

	std::cout << prompt;

	std::cin.ignore( 99, '\n' );

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



std::string toUpper( std::string s )

{

	for( std::string::size_type index = 0; index < s.length( ); ++index )

		s[index] = toupper( s[index] );

	return s;

}
