#include <iostream>
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
		{
			c.writePushPop ( cmd, p.arg1(), p.arg2() );
		}
		else if ( cmd == "C_ARITHMETIC" ) 
		{
			c.writeArithmetic ( p.arg1() );
		}
	}
}

int main ( int argc, char * argv[] ) {
	
	if ( argc != 2 )
	{
		std::cout << "Wrong number of arguments" << std::endl;
		return EXIT_FAILURE;
	}

	CodeWriter c ( fs::path(argv[1]).stem().string() + ".asm" );

	if ( fs::is_directory ( argv[1] ) )
	{	
		for ( auto& p: fs::directory_iterator ( argv[1] ) ) 
		{
			if ( p.path().extension() == ".vm" )
				parseFile ( p.path().string(), c );
		}
	} 
	else
		parseFile ( argv[1], c );
	
	return EXIT_SUCCESS;
}
