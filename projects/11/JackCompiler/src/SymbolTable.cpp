#include "SymbolTable.h"

SymbolTable::SymbolTable() : counter() {}

void SymbolTable::startSubroutine()
{
	varScope.clear();
	// reset arg and lcl counters
	counter[2] = counter[3] = 0;
}

Identifier SymbolTable::searchTable ( const std::string & name )
{
	// Lookup identifiers in both scopes 
	std::unordered_map<std::string, Identifier>::iterator found;
	found = varScope.find ( name );	
	if ( found != varScope.end() )
		return found->second;
		
	found = classScope.find ( name );
	if ( found != classScope.end() )
		return found->second;
		
	return  Identifier { KIND::NONE };
}

void SymbolTable::define ( const std::string & name, 
						   const std::string & type, 
						   KIND kind )
{
	Identifier temp = { kind, counter[kind]++, type };
	if ( kind == KIND::STATIC || kind == KIND::FIELD )
		classScope.insert ( { name, temp } );
	else
		varScope.insert ( { name, temp } );
}
	
int SymbolTable::varCount ( KIND kind )
{
	return counter[kind];
}

int SymbolTable::kindOf ( const std::string & name )
{
	return searchTable ( name ).kind;
}

std::string SymbolTable::typeOf ( const std::string & name )
{
	return searchTable ( name ).type;
}

int SymbolTable::indexOf ( const std::string & name )
{
	return searchTable ( name ).index;
}
