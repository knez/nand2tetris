#include <iostream>
#include <cstring>
#include <experimental/filesystem>

#include "Parser.h"
#include "CodeWriter.h"

namespace fs = std::experimental::filesystem;

void parseFile ( const std::string & path, CodeWriter & c )
{
	Parser p ( path );
	c.setFileName ( fs::path(path).stem() );
	
	std::string cmd;		
	while ( p.hasMoreCommands() ) 
	{
		p.advance();
		
		cmd = p.commandType();		
		if ( cmd == "C_PUSH" || cmd == "C_POP" ) 
			c.writePushPop ( cmd, p.arg1(), p.arg2() );
		else if ( cmd == "C_ARITHMETIC" ) 
			c.writeArithmetic ( p.arg1() );
		else if ( cmd == "C_LABEL" )
			c.writeLabel ( p.arg1() );
		else if ( cmd == "C_IF" )
			c.writeIf ( p.arg1() );
		else if ( cmd == "C_GOTO" )
			c.writeGoto ( p.arg1() );
		else if ( cmd == "C_FUNCTION" )
			c.writeFunction ( p.arg1(), p.arg2() );
		else if ( cmd == "C_RETURN" )
			c.writeReturn();
		else
			c.writeCall ( p.arg1(), p.arg2() );
	}
}

std::string parseArg ( char * str )
{
	// remove trailing slash
	size_t len = strlen ( str ) - 1;
	if ( str[len] == '/' )
		str[len] = '\0';
	return std::string ( str );	
}

int main ( int argc, char * argv[] ) {
	
	if ( argc != 2 )
	{
		std::cout << "Wrong number of arguments" << std::endl;
		return EXIT_FAILURE;
	}

	std::string arg = parseArg ( argv[1] );
	CodeWriter c ( fs::path(arg).stem().string() + ".asm" );

	if ( fs::is_directory ( arg ) )
	{	
		c.writeInit();
		for ( auto& p: fs::directory_iterator ( arg ) ) 
		{
			if ( p.path().extension() == ".vm" )
				parseFile ( p.path().string(), c );
		}
	} 
	else
		parseFile ( arg, c );
	
	return EXIT_SUCCESS;
}
