#include "JackTokenizer.h"
#include "VMWriter.h"
#include "SymbolTable.h"

class CompilationEngine 
{
	private:
		JackTokenizer & tknzr;
		VMWriter & writer;
		SymbolTable table;
		std::string subName, className;
		std::string name, type;
		
		int isOP();
		std::string eatType();		
		std::string eat ( const std::string & );		
		void compileClass();				
		void compileClassVarDec();
		void compileSubroutine();
		void compileSubroutineBody();
		void compileParameterList();
		void compileVarDec();
		void compileStatements ();
		void compileDo();
		void compileLet();
		void compileWhile ();
		void compileReturn();
		void compileIf ();
		void compileExpression();
		void compileTerm();
		int  compileExpressionList();
	
	public:
		CompilationEngine ( JackTokenizer &, VMWriter & );
};
