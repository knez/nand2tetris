#include "Parser.h"

Parser::Parser ( char * file )
{	
	// load the file for reading
	this->input.open ( file );
	
	// set current command to NULL
	this->cmd = "";
}

bool Parser::hasMoreCommands()
{	
	return input.peek() != EOF;
}

void Parser::advance()
{		
	getline ( input >> std::ws, cmd, '\n' );
	// consume new line character
	// input.ignore();
	
	std::size_t pos = cmd.find("//");
	if ( cmd.empty() || pos == 0 )
		advance();
	else
		cmd = cmd.substr ( 0, cmd.find(' ') );
}

void Parser::reset()
{	
	input.clear();
	input.seekg ( 0, std::ios::beg );
}

std::string Parser::commandType()
{	
	char first = this->cmd.at(0);
	
	if ( first == '@' ) return "A_COMMAND";
	if ( first == '(' ) return "L_COMMAND";
	
	return "C_COMMAND";
}

std::string Parser::symbol()
{	
	std::size_t c = cmd.size();
	if ( this->cmd.at(0) == '(' ) c-=2;
	return cmd.substr ( 1, c );
}

std::string Parser::dest() const
{
	std::size_t pos = cmd.find("=");
	
	// found
	if ( pos != std::string::npos )
		return cmd.substr ( 0, pos );
	
	return "";
}

std::string Parser::comp() const
{
	std::size_t pos = cmd.find("=");
	
	// found
	if ( pos != std::string::npos )
		return cmd.substr ( pos + 1 ); 
	
	pos = cmd.find(";");
	return cmd.substr ( 0, pos );	
}

std::string Parser::jump() const
{	
	std::size_t pos = cmd.find(";");
	
	// found
	if ( pos != std::string::npos )
		return cmd.substr ( pos + 1 ); 
		
	return "";
}
