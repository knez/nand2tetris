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
	getline ( input, cmd, '\r' );
	input.ignore();
	
	if ( cmd.empty() || cmd.at(0) == '/' )
		advance();
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
	//if ( cmd.length() <= 3 )
	return "C_ARITHMETIC";
}

std::string Parser::arg1() const
{
	// Arithmetic
	if ( cmd.length() <= 3 )
		return cmd;
	
	std::size_t pos1 = cmd.find(' ');
	std::size_t pos2 = cmd.rfind(' ');
	
	return cmd.substr(pos1 + 1, pos2 - pos1 - 1);
}

int Parser::arg2() const
{
	return std::stoi(cmd.substr(cmd.rfind(' ') + 1));
}
