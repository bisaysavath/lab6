// Michael Cuvin, Jeremy Mann, Kyle Fox, Bounpaseuth Saysavath
// Lab 6
// CS 254

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <time.h>

using namespace std;

// Tool Function to pause the screen
void pause( std::string prompt = "Press ENTER to continue..." );
// Function to get boolean from user
bool getBool( std::string prompt );

// We need to have a function that writes the output to a file

class wordParser
{
public:
    // Declare all the constants
    const string S_TO_D_COMMAND_VALUE = "40000810";
    const string D_TO_S_COMMAND_VALUE = "40000C18";

    const int S_TO_D_COMMAND_START_ADDRESS = 0x40000818;
    const int S_TO_D_COMMAND_END_ADDRESS = 0x40000C14;
    const int D_TO_S_COMMAND_START_ADDRESS = 0x40000C20;
    const int D_TO_S_COMMAND_END_ADDRESS = 0x4000101C;
    const int NUMBER_OF_WORDS_IN_A_LINE = 14;


    // Constructor takes one path to a file as a string argument and opens the inFile variable
    wordParser(const string &filePath)
    {
        openIfStream( filePath );
        openOfStream( "outputs.txt");
    }

    // Destructor closes the fstream files
    ~wordParser(void)
    {
        inFile.close();
        outFile.close();
    }

    // Perform a process
    void runAProcess(void);

    // Neccessary Functions
    vector<string> generateAddressRange( const int &start, const int &end, int increment = 4 );
    void output( const string &address, const string &data, const string &cycle, const int &lineNumber, const int &wordCount );
    unsigned int convertHexStringToUnsignedInt( const string &hexVariable );
    int getLineNumber( const int &numberOfWordEncounter );
    bool addressMatch( const string &currentAdrress, const vector<string> &addressRange, bool &lowToHigh );
    bool wordNumberToOutputMatch( const int &wordNumber );
    string wordFields( int wordNum, int wordData );
    void openIfStream( const string &filePath );
    void openOfStream( const string &fileName );
    // Convert a string to uppercase
    string toUpper( std::string s );

private:
    ifstream inFile;
    ofstream outFile;

    int numberOfWordEncounter = 0;

    string address;
    string data;
    string sizeData;
    string cycle;
};

int main( int argc, char *argv[] )
{
    clock_t time;
    time = clock();

    string filePath = argv[1];

    wordParser wordParserTest(filePath);

    wordParserTest.runAProcess();

    time = clock() - time;
    float seconds = ((float)time) / CLOCKS_PER_SEC;
    cout << "Completed in " << seconds << " secs."<< endl;

    pause( "Press enter to exit." );
    return 0;
}

void wordParser::runAProcess(void)
{
    vector<string> SToDCommandAddressRange = generateAddressRange( S_TO_D_COMMAND_START_ADDRESS, S_TO_D_COMMAND_END_ADDRESS );
    vector<string> DToSCommandAddressRange = generateAddressRange( D_TO_S_COMMAND_START_ADDRESS, D_TO_S_COMMAND_END_ADDRESS );
    bool reachTheEndOfAFile = false;

    while(true)
    {
        do
        {
            inFile >> address;
            numberOfWordEncounter++;

            if( inFile.eof() )
            {
                reachTheEndOfAFile = true;
                break;
            }

        } while( address != S_TO_D_COMMAND_VALUE && address != D_TO_S_COMMAND_VALUE );

        if( reachTheEndOfAFile )
            break;

        inFile >> data;
        inFile >> sizeData;
        inFile >> cycle;

        numberOfWordEncounter += 3;

        int wordCount = convertHexStringToUnsignedInt( data ) / 2;

        // Output the variables to the screen
        output( address, data, cycle, getLineNumber( numberOfWordEncounter ), wordCount );

        if( wordCount > 0 )
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
                if( lowToHigh )
                {
                    wordsData.push_back( nextWordOnALine.substr( 0, 4 ) );
                    wordsData.push_back( nextWordOnALine.substr( 4, 8 ) );
                }
                else
                {
                    wordsData.push_back( nextWordOnALine.substr( 4, 8 ) );
                    wordsData.push_back( nextWordOnALine.substr( 0, 4 ) );
                }

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

            if( lowToHigh )
            {
                for( int wordNumber = 0;( wordNumber < (wordsData.size()) ); wordNumber++ )
                {
                    if( wordNumberToOutputMatch( wordNumber ) )
                    {
                        outFile << "Line " << lineNumber[wordNumber] << ": ";
                        outFile << ( wordFields(wordNumber, convertHexStringToUnsignedInt( wordsData[wordNumber] ) ) );
                    }
                }
            }
            else
            {
                int index = 0;
                for( int wordNumber = wordsData.size() - 1; wordNumber >= 0; wordNumber-- )
                {
                    if( wordNumberToOutputMatch( wordNumber ) )
                    {
                        outFile << "Line " << lineNumber[index] << ": ";
                        outFile << ( wordFields(wordNumber, convertHexStringToUnsignedInt( wordsData[index] ) ) );
                    }

                    index++;
                }
            }
        }

        outFile << endl;
    }

    if( reachTheEndOfAFile )
        cout << "Process completed! Outputs are in outputs.txt" << endl;
    else
    {
        cout << "Error: Did not complete the process." << endl;
        exit(1);
    }
}

void wordParser::output( const string &address, const string &data, const string &cycle, const int &lineNumber, const int &wordCount )
{
    outFile << "Line " << lineNumber << ": " << ( (cycle == "Rd")? "Read " : "Write " );
    outFile << ((address == S_TO_D_COMMAND_VALUE) ? "S-to-D Command: " : "D-to-S Command: ");
    outFile << wordCount << " words" << endl;
}

unsigned int wordParser::convertHexStringToUnsignedInt( const string &hexVariable )
{
    // return stoi(hexVariable, nullptr, 16);
    // Alternative to stoi

    unsigned int x;
    stringstream ss;
    ss << hex << hexVariable;
    ss >> x;

    return x;
}



int wordParser::getLineNumber( const int &numberOfWordEncounter )
{
    return (numberOfWordEncounter / NUMBER_OF_WORDS_IN_A_LINE) + 1;
}

bool wordParser::addressMatch( const string &currentAdrress, const vector<string> &addressRange, bool &lowToHigh )
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

bool wordParser::wordNumberToOutputMatch( const int &wordNumber )
{
    int wordNumberNeededOutput[] = {0, 1, 4, 5, 10, 15, 22, 32, 37, 38, 40, 41};
    for( int index = 0; index < 12; index++ )
    {
        if( wordNumber == wordNumberNeededOutput[index])
        {
            return true;
        }
    }

    return false;
}

//parses word data to find command from specified bits
string wordParser::wordFields( int wordNum, int wordData )
{
    string output;
    stringstream ss;
    switch( wordNum )
    {
    case 0:
    {
              int rec_Ctrl = (wordData >> 13) & 0b11;
              ss << "Word 0: rec_Ctrl = " << rec_Ctrl;
              switch( rec_Ctrl )
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
              int cmd_Type = (wordData >> 13) & 0b111;
              ss << "Word 1: cmd_Type = " << cmd_Type;
              switch( cmd_Type )
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
              switch( rec_Raw )
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
               switch( reset_Enable )
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
               switch( direction )
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
               break;
    }

    case 37:
    {
               int parity = (wordData >> 15) & 0b1;
               ss << "Word 37: Parity = " << parity;
               switch( parity )
               {
               case 0:
                   ss << " Even\n";
                   break;
               case 1:
                   ss << " Odd\n";
                   break;
               default:
                   ss << " Unknown\n";
                   break;
               }
               break;
    }

    case 38:
    {
               int test = (wordData >> 14) & 0b1;
               ss << "Word 38: Test = " << test;
               switch( test )
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

    case 40:
    {
               int ctrl_Enable = (wordData >> 7) & 0b1;
               ss << "Word 40: ctrl_Enable = " << ctrl_Enable;
               switch( ctrl_Enable )
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

    case 41:
    {
               int code = (wordData >> 8) & 0b1111111;
               ss << "Word 41: Code = " << code << "\n";
    }
    }

    output = ss.str( );
    return output;
}

vector<string> wordParser::generateAddressRange( const int &start, const int &end, int increment )
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

string wordParser::toUpper( std::string s )
{
    for( std::string::size_type index = 0; index < s.length( ); ++index )
        s[index] = toupper( s[index] );
    return s;
}

void wordParser::openIfStream( const string &filePath )
{
    inFile.open( filePath );

    if( !inFile.is_open( ) )
    {
        cout << "Can't open test_data.log" << endl;
        pause( "Press enter to exit." );
        exit( 1 );
    }
}

void wordParser::openOfStream( const string &fileName )
{
    outFile.open( fileName );

    if( !outFile.is_open( ) )
    {
        cout << "Error: Can't write to the file." << endl;
        pause( "Press enter to exit." );
        exit( 1 );
    }
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
