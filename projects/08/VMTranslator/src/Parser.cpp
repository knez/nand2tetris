#include "Parser.h"

Parser::Parser ( const std::string & file ) 
{
	input.open ( file );
	cmd = "";
}

bool Parser::hasMoreCommands()
{
	return input.peek() != EOF;
}

void Parser::advance()
{
	getline ( input, cmd, '\n' );
	// consume new line character
	//input.ignore();
	
	while ( cmd.empty() || cmd.at(0) == '/' )
	{
		getline ( input, cmd, '\n' );
		//input.ignore();		
	}
	// remove inline comments
	if ( std::size_t pos = cmd.find ( '/' ) )
	{
		while ( cmd[pos - 1] == ' ' ) pos--;
		cmd = cmd.substr ( 0, pos );
	}
}

std::string Parser::commandType() const
{	
	if ( cmd.front() == 'p' ) 
	{
		if ( ( cmd.at(1) ) == 'u' )
			return "C_PUSH";
		else
			return "C_POP";
	}

	if ( cmd.length() <= 3  ) return "C_ARITHMETIC";
	if ( cmd.front() == 'r' ) return "C_RETURN";
	if ( cmd.front() == 'i' ) return "C_IF";
	if ( cmd.front() == 'f' ) return "C_FUNCTION";		
	if ( cmd.front() == 'g' ) return "C_GOTO";
	if ( cmd.front() == 'l' ) return "C_LABEL";
	
	return "C_CALL";	
}

std::string Parser::arg1() const
{
	// arithmethic
	if ( cmd.length() <= 3 )
		return cmd;

	std::size_t pos1 = cmd.find(' ');
	std::size_t pos2 = cmd.rfind(' ');
	
	return cmd.substr (pos1 + 1, pos2 - pos1 - 1);
}

int Parser::arg2() const
{
	std::size_t pos = cmd.rfind(' ');
	return std::stoi ( cmd.substr ( pos + 1 ) );
}
