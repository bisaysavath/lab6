// Michael Cuvin, Jeremy Mann, Kyle Fox, Bounpaseuth Saysavath
// Lab 6
// CS 254

#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<sstream>
#include<queue>

using namespace std;

const string S_TO_D_COMMAND_VALUE = "40000810";
const string D_TO_S_COMMAND_VALUE = "40000C18";
//const string S_TO_D_COMMAND_RANGE[] =  {"40000818", "4000081C", "40000820", "40000824", "40000828", "4000082C", "40000830", "40000834",
//										"40000838", "4000083C"};
const int NUMBER_OF_WORDS_IN_A_LINE = 14;

void outputToScreen( const string &address, const string &data, const int &size, const string &cycle );
unsigned int convertHexStringToUnsignedInt( const string &hexVariable );
int convertSizeTypeToInt( const string &sizeType );
string convertHexIntToDecimalString( const int &hexValue );
string wordFields(int wordNum, int wordData);

// Tool Function to pause the screen
void pause( std::string prompt = "Press ENTER to continue..." );
// Funtion to get boolean from user
bool	getBool( std::string prompt );

int main( int argc, char *argv[] )
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

	//int a = 0xabc;

	//cout << convertHexStringToUnsignedInt("40000818") << endl;
	//cout << convertHexIntToDecimalString( 0x40000818 ) << endl;

	//if( convertHexIntToDecimalString( 0x40000818 ) == convertHexIntToDecimalString( convertHexStringToUnsignedInt( "40000818" ) ) )
	//	cout << "Same" << endl;

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
		unsigned int byteCount = convertHexStringToUnsignedInt( data );
		cout << "Number of words: " << byteCount/2 << endl;

		// Output line number in a log file of the current address
		cout << "Line number: " << (numberOfWordEncounter / NUMBER_OF_WORDS_IN_A_LINE) + 1 << endl;

		queue<string> wordsData;

		if (byteCount > 0)
		{
			if (address == S_TO_D_COMMAND_VALUE)
			{
				// search from the address of 40000818 to 40000C14
				int nextSearchingAddress = 0x40000818;
				int stopSearchingAddress = 0x40000C14;
				string nextWordOnALine;
				int loopCount = 0;
				bool stopSearch = false;

				do
				{
					do
					{
						inFile >> nextWordOnALine;
						loopCount++;

						// If in the next 11 words the nextWordOnALine is not the address we expect to encouter break the loop
						if( loopCount == 11 && (convertHexIntToDecimalString( convertHexStringToUnsignedInt( nextWordOnALine ) )
							!= convertHexIntToDecimalString( nextSearchingAddress ) ) )
						{
							stopSearch = true;
							break;
						}

					} while( convertHexIntToDecimalString( convertHexStringToUnsignedInt( nextWordOnALine ) )
						!= convertHexIntToDecimalString( nextSearchingAddress ) );

					if ( stopSearch == true )
						break;

					inFile >> nextWordOnALine;

					// Store data in a  wordsData queue file structure
					wordsData.push( nextWordOnALine );

					// Next Address increases by 4
					nextSearchingAddress += 4;

					// Reset loopCount to 0
					loopCount = 0;

					// Discard the next two words
					inFile >> nextWordOnALine;
					inFile >> nextWordOnALine;

					//if( convertHexIntToDecimalString( 0x40000818 ) == convertHexIntToDecimalString( convertHexStringToUnsignedInt( "40000818" ) ) )
					//	cout << "Same" << endl;

				} while( nextSearchingAddress <= stopSearchingAddress );
			}
			//else
				// search from the address of 40000C20 to 4000101C
		}


		cout << "Data in this commands are:" << endl;
		while ( !wordsData.empty() )
		{
			cout << wordsData.front().substr(0,4) << " " << wordsData.front().substr(4,8) << endl;
			wordsData.pop();
		}

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

string convertHexIntToDecimalString( const int &hexValue )
{
	int x;
	stringstream ss;
	ss << dec << hexValue;

	return ss.str();
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

//parses word data to find command from specified bits
string wordFields(int wordNum, int wordData)
{
    string output;
    stringstream ss;
    switch(wordNum)
    {
        case 0:
        {
        int rec_Ctrl = (wordData >> 13) & 0b11;
        ss << "Word 0: rec_Ctrl = " << rec_Ctrl;
        switch(rec_Ctrl)
            {
            case 0:
                ss << " (no recording)\n";
                break;
            case 2:
                ss << " (no processing)\n";
                break;
            case 3:
				ss << " (processing & recording)\n";
                break;
            default:
                ss << " (unknown)\n";
                break;
            }
        break;
        }

        case 1:
        {
        int cmd_Type = (wordData >> 13) &0b111;
        ss << "Word 1: cmd_Type = " << cmd_Type;
        switch(cmd_Type)
            {
            case 4:
                ss << " Type A\n";
                break;
            case 5:
                ss << " Type B\n";
                break;
            case 6:
                ss << " Type C\n";
                break;
            default:
                ss << " Type Unkown\n";
                break;
            }
        break;
        }

        case 4:
        {
        int rec_Raw = wordData & 0b1;
        ss << "Word 4: rec_Raw = " << rec_Raw;
        switch(rec_Raw)
            {
            case 0:
                ss << ": Disable\n";
                break;
            case 1:
                ss << ": Enable\n";
                break;
            default:
                ss << ": Unknown\n";
                break;
            }
        break;
        }

        case 5:
        {
        int cmd_ID = wordData & 0b1111111;
        ss << "Word 5: cmd_ID = " << cmd_ID << "\n";
        break;
        }

        case 10:
        {
        int num_Responses = (wordData >> 11) & 0b11111;
        ss << "Word 10: num_Responses = " << num_Responses << "\n";
        break;
        }

        case 15:
        {
        int reset_Enable = (wordData >> 2) & 0b1;
        ss << "Word 15: reset_Enable = " << reset_Enable;
        switch(reset_Enable)
            {
            case 0:
                ss << " Disable\n";
                break;
            case 1:
                ss << " Enable\n";
                break;
            default:
                ss << " Unknown\n";
                break;
            }
            break;
        }

        case 22:
        {
        int direction = (wordData >> 3) & 0b1;
        ss << "Word 22: Direction = " << direction;
        switch(direction)
            {
            case 0:
                ss << " Right\n";
                break;
            case 1:
                ss << " Left\n";
                break;
            default:
                ss << " Unknown\n";
                break;
            }
            break;
        }

        case 32:
        {
        int num_Samples = wordData & 0b11111111111111;
        ss << "Word 32: Number of Samples = " << num_Samples << "\n";
        }
    }

    output = ss.str();
    return output;
}
