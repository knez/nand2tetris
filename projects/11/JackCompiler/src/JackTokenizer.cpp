#include "JackTokenizer.h"

JackTokenizer::JackTokenizer ( const std::string & file )
{
	input.open ( file );
	advance();
}

bool JackTokenizer::hasMoreTokens()
{
	return input.peek() != EOF;
}

std::string JackTokenizer::token()
{
	return currentToken;
}

const std::set<std::string> JackTokenizer::keywords
{
	"class", "method", "function",
	"constructor", "int", "boolean",
	"char", "void", "var", "static",
	"field", "let", "do", "if", "else",
	"while", "return", "true", "false",
	"null", "this"
};

void JackTokenizer::skipComments ( char & ch )
{
	input >> std::skipws >> ch;
	while ( ch == '/' )
	{
		if ( input.peek() == '*' ) {
			input.ignore ( 1000, '*' );
			while ( input.peek() != '/' )
				input.ignore ( 1000, '*' );
			input.ignore();
		}
		else if ( input.peek() == '/' )
			input.ignore(1000, '\n'); 
		else  // division op
			break; 
		input >> ch;
	}	
}

void JackTokenizer::advance()
{
	char ch;
	currentToken.clear();
	skipComments(ch);
	while ( std::isalnum ( ch ) )
	{
		currentToken += ch;
		input >> std::noskipws >> ch;
	}
	if ( ! currentToken.empty() ) { input.unget(); return; }
	currentToken += ch; 
	if ( ch != '"' ) return;
	input >> std::noskipws >> ch;
	while ( ch != '"' )
	{ 
		currentToken += ch;
		input >> std::noskipws >> ch;
	}
	currentToken += ch;
}

std::string JackTokenizer::type()
{
	if ( keywords.find(currentToken) != keywords.end() ) return "keyword";
	if ( currentToken.front() == '"' ) 		return "stringConstant";		
	if ( ispunct ( currentToken.front() ) ) return "symbol";
	if ( isdigit ( currentToken.front() ) ) return "integerConstant";
	return "identifier";
}

