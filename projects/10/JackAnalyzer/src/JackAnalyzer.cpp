#include <iostream>
#include <cstring>
#include <fstream>
#include <experimental/filesystem>

#include "CompilationEngine.h"

namespace fs = std::experimental::filesystem;

void compile ( const std::string & f )
{ 
	JackTokenizer tknzr ( f );
	
	size_t pos = f.rfind ( '.' );
	std::ofstream out ( f.substr(0, pos) + ".xml", std::ios::trunc );

	try	{
		CompilationEngine ( tknzr, out );
	} catch ( ... ) {
		std::cerr << "Error compiling file " << f << std::endl;
	}
	out.close();
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
		std::cerr << "Wrong number of arguments" << std::endl;
		return EXIT_FAILURE;
	}

	std::string arg = parseArg ( argv[1] );

	if ( ! fs::is_directory ( arg ) )
		compile ( arg );
	else
	{	
		for ( auto& p: fs::directory_iterator ( arg ) ) 
		{
			if ( p.path().extension() == ".jack" )
				compile ( p.path().string() );
		}
	} 
		
	return EXIT_SUCCESS;
}
