#include "VMWriter.h"

VMWriter::VMWriter ( const std::string & file ) : labelCount ( 0 )
{
	size_t pos = file.rfind ( '.' );
	out.open ( file.substr(0, pos) + ".vm", std::ios::trunc );	
}

int VMWriter::labelInc() 
{
	return labelCount++;	
}

void VMWriter::writeSegment ( int segment )
{
	switch ( segment )
	{
		case 0:  out << "static "; 	 break;
		case 1:	 out << "this ";  	 break;
		case 2:	 out << "argument "; break;
		case 3:	 out << "local ";  	 break;
		case 4:	 out << "constant "; break;
		case 5:	 out << "pointer ";	 break;
		case 6:  out << "that "; 	 break;
		case 7:  out << "temp ";	 break;
	}	
}

void VMWriter::writePush ( int segment, int index )
{
	out << "push "; 
	writeSegment ( segment );
	out << index << std::endl;
}

void VMWriter::writePop ( int segment, int index )
{
	out << "pop ";
	writeSegment ( segment );
	out << index << std::endl;
}

void VMWriter::writeLabel ( const std::string & label, int i )
{
	out << "label " << label << i << std::endl;
}

void VMWriter::writeGoto ( const std::string & label, int i )
{
	out << "goto " << label << i << std::endl;
}

void VMWriter::writeIf ( const std::string & label, int i )
{
	if ( label.find ( "WHILE" ) != std::string::npos )
		out << "not" << std::endl;
	out << "if-goto " << label << i << std::endl;
}

void VMWriter::writeArithmetic ( const char op )
{
	switch ( op )
	{
		case '+': out << "add\n"; break;
		case '-': out << "sub\n"; break;
		case '&': out << "and\n"; break;
		case '|': out << "or\n";  break;
		case '<': out << "lt\n";  break;
		case '>': out << "gt\n";  break;
		case '=': out << "eq\n";  break;
		case '~': out << "not\n"; break;
		case 'n': out << "neg\n"; break;
		case '*': out << "call Math.multiply 2\n"; break;
		case '/': out << "call Math.divide 2\n"; break;
	}
}

void VMWriter::writeCall ( const std::string & name, int args )
{
	out << "call " << name << " " << args << std::endl;
}

void VMWriter::writeFunction ( std::string & name, int nLocals )
{
	out << "function " << name << " " << nLocals << std::endl;
}

void VMWriter::writeReturn() 
{
	out << "return" << std::endl;
}

VMWriter::~VMWriter () 
{
	out.close();
}
