// Jim Hester
// Some usefull tools

#include"tools.h"

#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<time.h>

namespace tools_namespace
{

	const std::string WHITESPACE = " \t\n\r";

	int randomInt( int lowerBound, int upperBound )
	{
		static bool initialized = false;
		if ( !initialized )
		{
			srand((unsigned int)time(0));
			initialized = true;
		}
		return int(lowerBound + (1 + upperBound - lowerBound) * (rand() / (RAND_MAX + 1.0)));
	}

	bool isEmpty( std::string s )
	{
		return std::string::npos == s.find_first_not_of( WHITESPACE );
	}

	void openIfStream( std::ifstream &inStream, std::string prompt )
	{
		do
		{
			inStream.open( getLine( prompt ) );
			if ( !inStream.fail() ) break;
			std::cout << "File Open Failure." << std::endl;
			inStream.clear();
		} while (true);
	}
	void openOfStream( std::ofstream &outStream, std::string prompt )
	{
		do
		{
			outStream.open( getLine( prompt ) );
			if ( !outStream.fail() ) break;
			std::cout << "File Open Failure." << std::endl;
			outStream.clear();
		} while (true);
	}

	// returns how many characters wide an int will be on output
	int numberWidth( int n )
	{
		if ( n == 0 ) return 1;
		int widthAccumulator = 0;
		if ( n < 0 )
		{
			++widthAccumulator;
			n = -n;
		}
		for ( ; n > 0 ; n /= 10 )
			++widthAccumulator;
		return widthAccumulator;
	}

	// produce a std::string with repeated instances of a character
	std::string swab( char c, int lengthNeeded )
	{
		std::string result;
		for ( ; lengthNeeded > 0 ; --lengthNeeded )
			result += c;
		return result;
	}

	void swap( int &a, int &b )
	{
		int temp = a;
		a = b;
		b = temp;
	}

	bool isIn( std::string pattern, std::string source )
	{
		return std::string::npos != source.find(pattern);
	}

	bool isInCaseless( std::string pattern, std::string source )
	{
		return isIn( toLower(pattern), toLower(source) );
	}

	std::string substitute( std::string source, std::string pattern, std::string replacement )
	{
		std::string::size_type location = source.find(pattern);
		return ( location == std::string::npos )
				? source
				: ( source.substr(0,location)
					+ replacement
					+ source.substr( location+pattern.length(), source.length()-location) );
	}

	std::string pluralize( int count, std::string singular, std::string plural )
	{
		return (count == 1) ? singular : plural;
	}

	char toLower( char c )
	{
		return ( c >= 'A' && c <= 'Z' )
				? static_cast<char>(static_cast<int>(c) - static_cast<int>('A') + static_cast<int>('a'))
				: c;
	}

	std::string toLower( std::string s )
	{
		for ( std::string::size_type index = 0 ; index < s.length() ; ++index )
			s[index] = toLower( s[index] );
		return s;
	}

	bool noCaseEQ( std::string a, std::string b ) { return toLower(a)==toLower(b); }
	bool noCaseNE( std::string a, std::string b ) { return toLower(a)!=toLower(b); }
	bool noCaseLT( std::string a, std::string b ) { return toLower(a)< toLower(b); }
	bool noCaseLE( std::string a, std::string b ) { return toLower(a)<=toLower(b); }
	bool noCaseGT( std::string a, std::string b ) { return toLower(a)> toLower(b); }
	bool noCaseGE( std::string a, std::string b ) { return toLower(a)>=toLower(b); }

	bool isWhitespace( char c )
	{
		return std::string::npos != WHITESPACE.find(c);
	}

	void chomp( std::string &s )
	{
		// remove leading spaces
		while ( s.length() > 0 && isWhitespace(s[0]) )
			s.erase(0,1);
		// remove trailing spaces
		while ( s.length() > 0 && isWhitespace(s[s.length()-1]) )
			s.erase(s.length()-1,1);
	}

	std::string extractWord( std::string &source )
	{
		chomp(source);
		if ( source == "" ) return "";
		std::string::size_type spaceLocation = source.find_first_of(WHITESPACE);
		if ( spaceLocation == std::string::npos )
		{
			std::string word = source;
			source = "";
			return word;
		}
		std::string::size_type wordLength = spaceLocation;
		std::string word = source.substr(0,wordLength);
		source.erase(0,wordLength);
		chomp(source);
		return word;
	}

	char getCharInString( std::string prompt, std::string chars )
	{
		while (true)
		{
			std::cout << prompt;
			char userAnswer = getChar( prompt );
			if ( std::string::npos != chars.find( userAnswer ) ) return userAnswer;
			std::cout << "Error: Input must be one of chars in \"" << chars << "\". Try again." << std::endl;
		}
	}

	char getBoundedChar( std::string prompt, char lowerBound, char upperBound )
	{
		while (true)
		{
			std::cout << prompt;
			char userAnswer = getChar( prompt );
			if ( lowerBound <= userAnswer && userAnswer <= upperBound ) return userAnswer;
			std::cout << "Error: Input must be in the range ['" << lowerBound << "','" << upperBound << "']. Try again." << std::endl;
		}
	}

	int getBoundedInt( std::string prompt, int lowerBound, int upperBound )
	{
		std::ostringstream errorMessage;
		errorMessage
			<< "Range error: Input must be in the range ["
			<< lowerBound
			<< ".."
			<< upperBound
			<< "].";
		return getBoundedIntWithErrorMessage( prompt, lowerBound, upperBound, errorMessage.str() );
	}

	int getBoundedIntWithErrorMessage( std::string prompt, int lowerBound, int upperBound, std::string errorMessage )
	{
		do
		{
			int userInput = getInt( prompt );
			if ( userInput >= lowerBound && userInput <= upperBound )
			{
				return userInput;
			}
			std::cout << errorMessage << std::endl;
		} while (true);
	}

	int getPercent( std::string prompt )
	{
		return getBoundedIntWithErrorMessage( prompt, 0, 100,
				"Range Error: Value must be a percent between 0 and 100." );
	}

	int getPositiveInt( std::string prompt )
	{
		return getBoundedIntWithErrorMessage( prompt, 1, INT_MAX,
				"Range Error: Value must be a positive number." );
	}

	int getNonNegativeInt( std::string prompt )
	{
		return getBoundedIntWithErrorMessage( prompt, 0, INT_MAX, "Range Error: Value must be non-negative." );
	}

	template <class Item>
	Item getStreamExtraction (std::string prompt, Item itemForTypeResolution, std::string itemName )
	{
		do
		{
			std::istringstream inputLine( getLine(prompt) );
			Item userInput;
			inputLine >> userInput;
			if ( inputLine.fail() )
			{
				std::cout << "Input format error: please give an " << itemName << "." << std::endl;
				continue;
			}
			char junk;
			inputLine >> junk;
			if ( !inputLine.fail() )
			{
				std::cout << "Input format error: please give ONLY an " << itemName << "." << std::endl;
				continue;
			}
			return userInput;
		}
		while ( true );
	}
	int		getInt		( std::string prompt ) { return getStreamExtraction( prompt, int(), "integer" ); }
	float	getFloat	( std::string prompt ) { return getStreamExtraction( prompt, float(), "float" ); }
	char	getChar		( std::string prompt ) { return getStreamExtraction( prompt, char(), "character" ); }
	std::string	getString	( std::string prompt ) { return getStreamExtraction( prompt, std::string(), "std::string" ); }

	std::string getLine( std::string prompt )
	{
		do
		{
			std::cout << prompt;
			std::string userInput;
			getline( std::cin, userInput );
			if ( !std::cin.fail() )
			{
				return userInput;
			}
			std::cout << "Input error. Please try again." << std::endl;
			std::cin.clear();
			flushInput();
		}
		while ( true );
	}

	bool getBool( std::string prompt )
	{
		do
		{
			std::string userAnswer = toLower(getString( prompt ));
			if ( userAnswer == "y" || userAnswer == "yes" ) return true;
			if ( userAnswer == "n" || userAnswer == "no" ) return false;
			std::cout << "Enter y, yes, n, or no." << std::endl;
		} while ( true );
	}

	// assumes that keyboard input buffer is empty
	void pause( std::string prompt )
	{
		std::cout << prompt;
		std::cin.ignore(999,'\n');
	}

	void flushInput( istream &stream )
	{
		std::cin.ignore(999,'\n');
	}

}