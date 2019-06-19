#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
	// Load predefined symbols
	
	map.insert ( { "SP", 	  0x0000 } );
	map.insert ( { "LCL", 	  0x0001 } );
	map.insert ( { "ARG", 	  0x0002 } );
	map.insert ( { "THIS", 	  0x0003 } );
	map.insert ( { "THAT", 	  0x0004 } );
	map.insert ( { "SCREEN",  0x4000 } );
	map.insert ( { "KBD", 	  0x6000 } );

	for ( int i = 0; i < 16; i++ ) {
		map.insert ( { "R" + std::to_string(i), i } );
	}
}

int SymbolTable::GetAddress()
{	
	return it->second;
}

void SymbolTable::addEntry ( std::string str, int x )
{
	map.insert ( { str, x } );
}

bool SymbolTable::contains ( std::string str )
{
	it = map.find ( str );
	
	if ( it != map.end() ) {
		return true;
	} else {
		return false;
	}
}
