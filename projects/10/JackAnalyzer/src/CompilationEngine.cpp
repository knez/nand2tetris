#include <iomanip> 
#include "CompilationEngine.h"

CompilationEngine::CompilationEngine ( JackTokenizer & t, std::ofstream & o )
									   : indent ( 1 ), tknzr ( t ), out ( o )
{
	compileClass();
}

inline void CompilationEngine::eatType()
{
	if (  tknzr.token() == "int" 
	   || tknzr.token() == "char" 
	   || tknzr.token() == "boolean" )
		eat ( "keyword" );
	else 
		eat ( "identifier" );
}

inline bool CompilationEngine::isOP() {
	switch ( tknzr.token().front() ) {
		case '+': return true;
		case '-': return true;
		case '*': return true;
		case '/': return true;
		case '&': return true;
		case '|': return true;
		case '<': return true;
		case '>': return true;
		case '=': return true;
		default : return false;
	}
} 

inline void CompilationEngine::writeTag ( const std::string & tag )
{
	out << std::setw ( 2*indent + tag.size() );
	out << tag << std::endl;
}

void CompilationEngine::eat ( const std::string & token )
{
	std::string tkn = tknzr.token();
	
	if ( tknzr.token() != token && tknzr.type() != token )
		throw "Bad token";
	
	if ( tknzr.type() == "stringConstant" ) {
		tkn = tkn.substr ( 1, tkn.size() - 2 );
	}
	if ( tkn == "<" ) tkn = "&lt;";
	if ( tkn == ">" ) tkn = "&gt;";
	if ( tkn == "&" ) tkn = "&amp;";
	
	writeTag ( "<" + tknzr.type()  + "> "
				   + tkn +
			 " </" + tknzr.type()  + ">" );
	
	tknzr.advance();	
}

void CompilationEngine::compileClass()
{	
	out << "<class>\n";
		eat ( "class" );
		eat ( "identifier" );
		eat ( "{" );
		compileClassVarDec();
		compileSubroutine();
		eat ( "}" );
	out << "</class>";
}

void CompilationEngine::compileClassVarDec()
{
	if ( tknzr.token() == "static" || tknzr.token() == "field" )
	{
		writeTag ( "<classVarDec>" );
		indent++;
			eat ( "keyword" );
			eatType();
			eat ( "identifier" );
			while ( tknzr.token() == "," )
			{
				eat ( "," );
				eat ( "identifier" );
			}
			eat ( ";" );
		indent--;
		writeTag ( "</classVarDec>" );
		compileClassVarDec();
	}
}

void CompilationEngine::compileSubroutine()
{
	if ( tknzr.token() == "constructor" || tknzr.token() == "function" 
										|| tknzr.token() == "method" )
	{
		writeTag ( "<subroutineDec>" );
		indent++;
			eat ( "keyword" );		
			if ( tknzr.token() == "void" )
				eat ( "void" );
			else 
				eatType();
			eat ( "identifier" );
			eat ( "(" );
			compileParameterList();
			eat ( ")" );
			compileSubroutineBody();
		indent--;
		writeTag ( "</subroutineDec>" ); 
		compileSubroutine();
	}
}

void CompilationEngine::compileParameterList()
{
	writeTag ( "<parameterList>" );	
	if ( tknzr.token() != ")" )
	{
		indent++;
		eatType();
		eat ( "identifier" );
		while ( tknzr.token() == "," ) 
		{
			eat ( "," );
			eatType();
			eat ( "identifier" );
		}
		indent--;
	}
	writeTag ( "</parameterList>" );
}

void CompilationEngine::compileSubroutineBody()
{
	writeTag ( "<subroutineBody>" );
	indent++;
		eat ( "{" );
		compileVarDec();
		compileStatements();
		eat ( "}" );
	indent--;
	writeTag ( "</subroutineBody>" );		
}

void CompilationEngine::compileVarDec()
{
	if ( tknzr.token() == "var" )
	{
		writeTag ( "<varDec>" );	
		indent++;
			eat ( "var" );
			eatType();
			eat ( "identifier" );
			while ( tknzr.token() == "," ) 
			{
				eat ( "," );
				eat ( "identifier" );
			}
			eat ( ";" );
		indent--;
		writeTag ( "</varDec>" );
		compileVarDec();
	}
}

void CompilationEngine::compileStatements()
{	
	writeTag ( "<statements>" );	
	indent++;
	while ( true )
	{
		if 		( tknzr.token() == "let" ) 	  compileLet();
		else if ( tknzr.token() == "if" )  	  compileIf();
		else if ( tknzr.token() == "while" )  compileWhile();
		else if ( tknzr.token() == "do" ) 	  compileDo();
		else if ( tknzr.token() == "return" ) compileReturn();
		else break;
	}
	indent--;
	writeTag ( "</statements>" );
}

void CompilationEngine::compileLet()
{
	writeTag ( "<letStatement>" );
	indent++;
		eat ( "let" );
		eat ( "identifier" );
		if ( tknzr.token() == "[" ) 
		{
			eat ( "[" );
			compileExpression();
			eat ( "]" );
		}
		eat ( "=" );
		compileExpression();
		eat ( ";" );
	indent--;
	writeTag ( "</letStatement>" );
}

void CompilationEngine::compileIf()
{
	writeTag ( "<ifStatement>" );
	indent++;	
		eat ( "if" );
		eat ( "(" );
		compileExpression();
		eat ( ")" );
		eat ( "{" );
		compileStatements();
		eat ( "}" );
		if ( tknzr.token() == "else" )
		{
			eat ( "else" );
			eat ( "{" );
			compileStatements();
			eat ( "}" );
		}
	indent--;		
	writeTag ( "</ifStatement>" );
}

void CompilationEngine::compileWhile()
{
	writeTag ( "<whileStatement>" );
	indent++;
		eat ( "while" );
		eat ( "(" );
		compileExpression();
		eat ( ")" );
		eat ( "{" );
		compileStatements();
		eat ( "}" );
	indent--;
	writeTag ( "</whileStatement>" );
}

void CompilationEngine::compileDo()
{
	writeTag ( "<doStatement>" );
	indent++;
		eat ( "do" );
		eat ( "identifier" );
		if ( tknzr.token() == "." ) 
		{
			eat ( "." );
			eat ( "identifier" );
		}
		eat ( "(" );
		compileExpressionList();
		eat ( ")" );
		eat ( ";" );
	indent--;
	writeTag ( "</doStatement>" );
}

void CompilationEngine::compileReturn()
{
	writeTag ( "<returnStatement>" );
	indent++;
		eat ( "return" );
		if ( tknzr.token() != ";" )
			compileExpression();
		eat ( ";" );
	indent--;
	writeTag ( "</returnStatement>" );
}

void CompilationEngine::compileExpression()
{
	writeTag ( "<expression>" );
	indent++;
		compileTerm();
		while ( isOP () ) 
		{
			eat ( tknzr.token() );
			compileTerm();
		}
	indent--;
	writeTag ( "</expression>" );
}

void CompilationEngine::compileTerm()
{
	writeTag ( "<term>" );
	indent++;
		if ( tknzr.type() == "identifier" )
		{
			eat ( "identifier" );
			if ( tknzr.token() == "[" )
			{
				eat ( "[" );
				compileExpression();
				eat ( "]" );
			}
			else if ( tknzr.token() == "(" || tknzr.token() == "." )
			{
				if ( tknzr.token() == "." ) 
				{
					eat ( "." );
					eat ( "identifier" );
				}
				eat ( "(" );
				compileExpressionList();
				eat ( ")" );	
			}
		}
		else if ( tknzr.token() == "(" )
		{
			eat ( "(" );
			compileExpression();
			eat ( ")" );
		}
		else if ( tknzr.token() == "~" || tknzr.token() == "-" ) 
		{
			eat ( tknzr.token() );
			compileTerm();
		}
		else
			eat ( tknzr.token() );
	indent--;
	writeTag ( "</term>" );	
}

void CompilationEngine::compileExpressionList()
{
	writeTag ( "<expressionList>" );
	indent++;
	if ( tknzr.token() != ")" )
	{
		compileExpression();
		while ( tknzr.token() == "," )
		{
			eat ( "," );
			compileExpression();
		}
	}
	indent--;
	writeTag ( "</expressionList>" );
}
