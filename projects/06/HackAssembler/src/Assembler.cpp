#include <iostream>
#include <fstream>
#include <bitset>

#include "Parser.h"
#include "Code.h"
#include "SymbolTable.h"

std::string getName ( std::string arg )
{	
	std::size_t pos = arg.find_last_of('.');
	return arg.substr(0, pos) + ".hack";
}

void first_pass ( SymbolTable & s, Parser & p ) 
{	
	for ( int ROM = 0; p.hasMoreCommands(); ROM++ )
	{
		p.advance ();
		
		if ( p.commandType() == "L_COMMAND" )
		{
			s.addEntry ( p.symbol(), ROM-- );
		}
	}
	p.reset();
}

int main ( int argc, char * argv[] ) {
				
	if ( argc != 2 ) 
	{
		std::cout << "Wrong number of arguments" << std::endl;
		return EXIT_FAILURE;
	}
	
	int RAM = 0x10;
	Code c; SymbolTable s;
	Parser p ( argv[1] );
	
	std::ofstream out (getName(argv[1]), std::ios::trunc);
	std::bitset<16> b;
	std::string symbol;

	first_pass ( s, p );
	
	while ( p.hasMoreCommands() )
	{
		p.advance();

		if ( p.commandType() == "A_COMMAND" )
		{
			symbol = p.symbol();
			if ( ! isdigit ( symbol.front() ) )
			{
				if ( s.contains ( symbol ) ) {
					b = s.GetAddress();
				}
				else {
					s.addEntry ( symbol, RAM );
					b = RAM++;
				}
			} else {
				b = std::stoi ( symbol );
			}
			out << b << std::endl;
		}
		else if ( p.commandType() == "C_COMMAND" )
		{
			out << c.decode(p.comp(), p.dest(), p.jump()) << std::endl;
		}
	}	
	
	out.close();
	return EXIT_SUCCESS;
}
