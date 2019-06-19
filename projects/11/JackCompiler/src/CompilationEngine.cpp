#include "CompilationEngine.h"

CompilationEngine::CompilationEngine ( JackTokenizer & t, VMWriter & w )
									 : tknzr ( t ), writer ( w )
{
	compileClass();
}

inline std::string CompilationEngine::eatType()
{
	if (  tknzr.token() == "int" 
	   || tknzr.token() == "char" 
	   || tknzr.token() == "boolean" )
		return eat ( "keyword" );
	else 
		return eat ( "identifier" );
}

inline int CompilationEngine::isOP()
{
	switch ( tknzr.token().front() )
	{
		case '+': return '+';
		case '-': return '-';
		case '*': return '*';
		case '/': return '/';
		case '&': return '&';
		case '|': return '|';
		case '<': return '<';
		case '>': return '>';
		case '=': return '=';
		default : return 0;
	}
} 

std::string CompilationEngine::eat ( const std::string & token )
{
	std::string tkn = tknzr.token();
	
	if ( tknzr.token() != token && tknzr.type() != token )
		throw "Bad token";
	
	tknzr.advance();
	return tkn;
}

void CompilationEngine::compileClass()
{	
	eat ( "class" );
	className = eat ( "identifier" );
	eat ( "{" );
	compileClassVarDec();
	compileSubroutine();
	eat ( "}" );	
}

void CompilationEngine::compileClassVarDec()
{
	if ( tknzr.token() == "static" || tknzr.token() == "field" )
	{
		KIND kind = tknzr.token() == "static" ? KIND::STATIC : KIND::FIELD;
		eat ( "keyword" );
		type = eatType();
		name = eat ( "identifier" );
		table.define ( name, type, kind );
		while ( tknzr.token() == "," )
		{
			eat ( "," );
			name = eat ( "identifier" );
			table.define ( name, type, kind );
		}
		eat ( ";" );
		compileClassVarDec();
	}
}

void CompilationEngine::compileSubroutine()
{
	if ( tknzr.token() == "constructor" || tknzr.token() == "function" 
										|| tknzr.token() == "method" )
	{
		table.startSubroutine();  		// clear varScope table
		name = eat ( "keyword" ); 		// constructor|function|method

		if ( tknzr.token() == "void" )  // eat function return type
			type = eat ( "void" );
		else 
			type = eatType();

		subName = eat ( "identifier" ); // subroutine name
		if ( name == "method" ) 		// pass hidden object argument
			table.define ( "this", type, KIND::ARG );

		eat ( "(" );
		compileParameterList();
		eat ( ")" );
		compileSubroutineBody();
		compileSubroutine();
	}
}

void CompilationEngine::compileParameterList()
{
	std::string name;
	if ( tknzr.token() != ")" )
	{
		type = eatType();
		name = eat ( "identifier" );
		table.define ( name, type, KIND::ARG );
		while ( tknzr.token() == "," ) 
		{
			eat ( "," );
			type = eatType();
			name = eat ( "identifier" );
			table.define ( name, type, KIND::ARG );
		}
	}
}

void CompilationEngine::compileSubroutineBody()
{
	eat ( "{" );
	compileVarDec();
	subName = className + "." + subName;  // compute vm function name
	writer.writeFunction ( subName, table.varCount ( KIND::VAR ) );

	if ( name == "method" )	// set "this" to the current object
	{
		writer.writePush ( KIND::ARG, 0 );
		writer.writePop ( KIND::POINTER, 0 );
	}
	// allocate object fields and initialize "this" pointer	
	else if ( name == "constructor" )
	{
		writer.writePush ( KIND::CONST, table.varCount ( KIND::FIELD ) );
		writer.writeCall ( "Memory.alloc" , 1 );
		writer.writePop ( KIND::POINTER, 0 );
	}

	compileStatements();
	eat ( "}" );
}

void CompilationEngine::compileVarDec()
{
	std::string name;
	if ( tknzr.token() == "var" )
	{
		eat ( "var" );
		type = eatType();
		name = eat ( "identifier" );
		table.define ( name, type, KIND::VAR );
		while ( tknzr.token() == "," ) 
		{
			eat ( "," );
			name = eat ( "identifier" );
			table.define ( name, type, KIND::VAR );
		}
		eat ( ";" );
		compileVarDec();
	}
}

void CompilationEngine::compileStatements ()
{	
	while ( true )
	{
		if 		( tknzr.token() == "let" ) 	  compileLet();
		else if ( tknzr.token() == "if" )	  compileIf();
		else if ( tknzr.token() == "while" )  compileWhile();
		else if ( tknzr.token() == "do" ) 	  compileDo();
		else if ( tknzr.token() == "return" ) compileReturn();
		else break;
	}
}

void CompilationEngine::compileLet()
{
	eat ( "let" );
	type = eat ( "identifier" );
	if ( tknzr.token() == "[" )	// array handling
	{
		writer.writePush (table.kindOf ( type ), table.indexOf ( type ));
		eat ( "[" );
		compileExpression();
		eat ( "]" );
		writer.writeArithmetic ( '+' );
		eat ( "=" );
		compileExpression();
		eat ( ";" );
		writer.writePop  ( KIND::TEMP, 0 );
		writer.writePop  ( KIND::POINTER, 1 );
		writer.writePush ( KIND::TEMP, 0 );
		writer.writePop  ( KIND::THAT, 0 );
	}
	else
	{
		eat ( "=" );
		compileExpression();
		eat ( ";" );
		writer.writePop ( table.kindOf ( type ), table.indexOf ( type ) );
	}
}

void CompilationEngine::compileIf ()
{	
	int i = writer.labelInc();
	eat ( "if" );
	eat ( "(" );
	compileExpression();
	eat ( ")" );
	writer.writeIf ( "IF_TRUE", i );
	writer.writeGoto ( "IF_FALSE", i );
	writer.writeLabel ( "IF_TRUE", i );
	eat ( "{" );
	compileStatements ();
	eat ( "}" );
	writer.writeGoto ( "IF_END", i );
	writer.writeLabel ( "IF_FALSE", i );
	if ( tknzr.token() == "else" )
	{
		eat ( "else" );
		eat ( "{" );
		compileStatements ();
		eat ( "}" );
	}
	writer.writeLabel ( "IF_END", i );
}

void CompilationEngine::compileWhile ()
{
	int i = writer.labelInc();
	writer.writeLabel ( "WHILE_EXP", i );
	eat ( "while" );
	eat ( "(" );
	compileExpression();
	eat ( ")" );
	writer.writeIf ( "WHILE_END", i );
	eat ( "{" );	
	compileStatements ();
	eat ( "}" );
	writer.writeGoto  ( "WHILE_EXP", i );
	writer.writeLabel ( "WHILE_END", i );
}

void CompilationEngine::compileDo()
{
	std::string name;
	int args = 0;
	eat ( "do" );
	name = eat ( "identifier" );
	if ( table.kindOf ( name ) != KIND::NONE ) // local object found
	{
		args++;
		writer.writePush ( table.kindOf(name), table.indexOf(name) );
		name = table.typeOf ( name );
	}
	else if ( tknzr.token() == "(" )	// compile internal class method
	{
		args++;
		writer.writePush ( KIND::POINTER, 0 );
		name = className + "." + name;
	}
	if ( tknzr.token() == "." ) 	// compile static or object calls
	{
		name += eat ( "." );
		name += eat ( "identifier" );
	}
	eat ( "(" );
	args += compileExpressionList();
	eat ( ")" );
	eat ( ";" );
	writer.writeCall ( name, args );	
	writer.writePop ( KIND::TEMP, 0 );
}

void CompilationEngine::compileReturn()
{
	eat ( "return" );
	if ( tknzr.token() != ";" )
		compileExpression();
	else
		writer.writePush ( KIND::CONST, 0 );
	eat ( ";" );
	writer.writeReturn();	
}

void CompilationEngine::compileExpression()
{
	char op;
	compileTerm();
	if ( ( op = isOP () ) )
	{
		eat ( tknzr.token() );
		compileExpression();
		writer.writeArithmetic ( op );
	}
}

void CompilationEngine::compileTerm()
{
	int args = 0;
	std::string name;

	if ( tknzr.type() == "identifier" )
	{
		name = eat ( "identifier" );
		
		if ( table.kindOf ( name ) != KIND::NONE ) // local variable found
		{
			args++;
			writer.writePush ( table.kindOf(name), table.indexOf(name) );
			name = table.typeOf ( name );

			if ( tknzr.token() == "[" )	// access array
			{
				eat ( "[" );
				compileExpression();
				eat ( "]" );
				writer.writeArithmetic ( '+' );
				writer.writePop  ( KIND::POINTER, 1 );
				writer.writePush ( KIND::THAT, 0 ); 
			}
		}		
		if ( tknzr.token() == "(" || tknzr.token() == "." )
		{	
			if ( tknzr.token() == "." ) 
			{
				name += eat ( "." );
				name += eat ( "identifier" );
			}
			eat ( "(" );
			args += compileExpressionList();
			eat ( ")" );	
			writer.writeCall ( name, args );
		}
	}
	else if ( tknzr.type() == "symbol" )
	{
		// unary operators
		char op = tknzr.token().front();
		if ( op == '~' )
		{
			eat ( "~" );
			compileTerm();
			writer.writeArithmetic ( '~' );
		} 
		else if ( op == '-' )
		{
			eat ( "-" );
			compileTerm();
			writer.writeArithmetic ( 'n' );			
		}
		else
		{
			eat ( "(" );
			compileExpression();
			eat ( ")" );
		}
	}
	else if ( tknzr.type() == "integerConstant" )
	{
		name = eat ( tknzr.token() );
		writer.writePush ( KIND::CONST, stoi ( name ) );
	}
	else if ( tknzr.type() == "keyword" )
	{
		if ( tknzr.token() == "true" )
		{
			eat ( "true" );
			writer.writePush ( KIND::CONST, 0 );
			writer.writeArithmetic ( '~' );	
		}
		else if ( tknzr.token() == "false" )
		{
			eat ( "false" );
			writer.writePush ( KIND::CONST, 0 );		
		}
		else if ( tknzr.token() == "this" )	
		{
			eat ( "this" );
			writer.writePush ( KIND::POINTER, 0 );			
		}
		else 	// null
		{
			eat ( "null" );
			writer.writePush ( KIND::CONST, 0 );
		}
	}
	else if ( tknzr.type() == "stringConstant" ) 
	{
		name = eat ( "stringConstant" );
		writer.writePush ( KIND::CONST, name.size() - 2 );
		writer.writeCall ( "String.new", 1 );
		for ( size_t i = 1; i < ( name.size() - 1 ) ; i++ )
		{
			writer.writePush ( KIND::CONST, name[i] );
			writer.writeCall ( "String.appendChar", 2 );
		}	
	}
}

int CompilationEngine::compileExpressionList()
{
	int args = 0;
	if ( tknzr.token() != ")" )
	{
		args++;
		compileExpression();
		while ( tknzr.token() == "," )
		{
			args++;
			eat ( "," );
			compileExpression();
		}
	}
	return args;
}
