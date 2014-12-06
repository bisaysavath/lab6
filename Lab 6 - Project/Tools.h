// Jim Hester
// Some usefull tools

#include<string>
#include<iostream>
#include<fstream>

using namespace std;

#ifndef TOOLS_LOCK
#define TOOLS_LOCK

namespace tools_namespace
{
	extern const std::string WHITESPACE;

	int		getInt		( std::string prompt );
	float	getFloat	( std::string prompt );
	char	getChar		( std::string prompt );
	std::string	getString	( std::string prompt );
	template <class Item> Item getStreamExtraction (std::string prompt, Item itemForTypeResolution, std::string itemName ); 

	char getBoundedChar( std::string prompt, char lowerBound, char upperBound );
	char getCharInString( std::string prompt, std::string chars );

	std::string	getLine				( std::string prompt );
	bool	getBool				( std::string prompt );
	int		getNonNegativeInt	( std::string prompt );
	int		getPositiveInt		( std::string prompt );
	int		getBoundedInt		( std::string prompt, int lowerBound, int upperBound );
	int		getPercent			( std::string prompt );
	int getBoundedIntWithErrorMessage( std::string prompt, int lowerBound, int upperBound, std::string errorMessage );

	void pause( std::string prompt = "Press ENTER to continue..." );
	void flushInput(istream &stream = cin);

	char	toLower( char c );
	std::string	toLower( std::string s );

	bool noCaseEQ( std::string a, std::string b );
	bool noCaseNE( std::string a, std::string b );
	bool noCaseLT( std::string a, std::string b );
	bool noCaseLE( std::string a, std::string b );
	bool noCaseGT( std::string a, std::string b );
	bool noCaseGE( std::string a, std::string b );

	bool isIn			( std::string pattern, std::string source );
	bool isInCaseless	( std::string pattern, std::string source );
	bool isWhitespace	( char c );
	bool isEmpty		( std::string s );

	void	chomp		( std::string &s );
	std::string	swab		( char c, int length );
	std::string	extractWord	( std::string &source );
	std::string	substitute	( std::string source, std::string pattern, std::string replacement );
	std::string	pluralize	( int count, std::string singular, std::string plural );

	void swap( int &a, int &b );
	int numberWidth( int n );
	int randomInt( int lowerBound, int upperBound );

	void openIfStream( std::ifstream &inStream, std::string prompt );
	void openOfStream( std::ofstream &outStream, std::string prompt );
}

#endif

