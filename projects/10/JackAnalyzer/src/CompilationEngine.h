#include <fstream>
#include "JackTokenizer.h"

class CompilationEngine 
{
	private:
		int indent;
		JackTokenizer & tknzr;
		std::ofstream & out;
		
		void writeTag ( const std::string & );
		bool isOP ();
		void eatType();		
		void eat( const std::string & );		
		void compileClass();				
		void compileClassVarDec();
		void compileSubroutine();
		void compileSubroutineBody();
		void compileParameterList();
		void compileVarDec();
		void compileStatements();
		void compileDo();
		void compileLet();
		void compileWhile();
		void compileReturn();
		void compileIf();
		void compileExpression();
		void compileTerm();
		void compileExpressionList();
	
	public:
		CompilationEngine ( JackTokenizer &, std::ofstream & );
};
