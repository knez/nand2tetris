#include "CodeWriter.h"

CodeWriter::~CodeWriter() 
{
	this->out.close();
}

CodeWriter::CodeWriter ( const std::string & file )
{
	this->out.open ( file, std::ios::trunc );
}

void CodeWriter::setFileName ( const std::string & fileName )
{
	this->fileName = fileName;
}

void CodeWriter::writeInit()
{
	// SP = 256
	// call Sys.init
	out << "@256\nD=A\n@SP\nM=D\n";
	this->writeCall ( "Sys.init", 0 );
}

void CodeWriter::writeArithmetic ( const std::string & cmd )
{
	out << "@SP" << std::endl;
	
	if 	( cmd == "add" ) out << "AM=M-1\nD=M\nA=A-1\nM=M+D\n";	
	else if ( cmd == "sub" ) out << "AM=M-1\nD=M\nA=A-1\nM=M-D\n";
	else if ( cmd == "neg" ) out << "A=M-1\nM=-M\n";
	else if ( cmd == "not" ) out << "A=M-1\nM=!M\n";
	else if	( cmd == "and" ) out << "AM=M-1\nD=M\nA=A-1\nM=M&D\n";
	else if ( cmd == "or"  ) out << "AM=M-1\nD=M\nA=A-1\nM=M|D\n";
	
	else if ( cmd == "eq" ) {
		out << "AM=M-1\nD=!M\nA=A-1\nMD=M+D\n"
			<< "@SKIP" << skip_counter 
			<< "\nD+1;JEQ\n@SP\nA=M-1\nM=0\n"
			<< "(SKIP" << skip_counter++ << ")\n";
	}
	else if ( cmd == "gt" ) {
		out << "AM=M-1\nD=M\nA=A-1\nMD=M-D\n"
			<< "M=-1\n@SKIP" << skip_counter 
			<< "\nD;JGT\n@SP\nA=M-1\nM=0\n"
			<< "(SKIP" << skip_counter++ << ")\n";
	}
	else if ( cmd == "lt" ) {
		out << "AM=M-1\nD=M\nA=A-1\nMD=M-D\n"
			<< "M=-1\n@SKIP" << skip_counter
			<< "\nD;JLT\n@SP\nA=M-1\nM=0\n"
			<< "(SKIP" << skip_counter++ << ")\n";
	}
}

void CodeWriter::writePushPop ( const std::string & cmd,
								const std::string   seg, int i)								
{
	out << "@" << i << std::endl;
	out << "D=A" << std::endl;
	
	if ( cmd == "C_PUSH" )
	{	
		if ( seg == "local" ) out << "@LCL\nA=M+D\nD=M\n";
		else if ( seg == "argument" ) out << "@ARG\nA=M+D\nD=M\n";
		else if ( seg == "this" ) out << "@THIS\nA=M+D\nD=M\n";
		else if ( seg == "that" ) out << "@THAT\nA=M+D\nD=M\n";
		else if ( seg == "temp" ) out << "@R5\nA=A+D\nD=M\n";
		else if ( seg == "pointer" ) out << "@" << 3 + i << "\nD=M\n";
		else if ( seg == "static" ) out << "@" << fileName << "." << i 
														   << "\nD=M\n";
		out << "@SP\nAM=M+1\nA=A-1\nM=D" << std::endl;
	}
	else
	{
		if ( seg == "local" ) out << "@LCL\nD=M+D\n";
		else if ( seg == "this" ) out << "@THIS\nD=M+D\n";
		else if ( seg == "that" ) out << "@THAT\nD=M+D\n";
		else if ( seg == "argument" ) out << "@ARG\nD=M+D\n";
		else if ( seg == "temp" ) out << "@R5\nD=A+D\n";
		else if ( seg == "pointer" ) out << "@" << 3 + i << "\nD=A\n";
		else if ( seg == "static" ) out << "@" << fileName << "." << i 
														   << "\nD=A\n";			

		// swap A and D without temp var
		out << "@SP\nAM=M-1\nA=M+D\nD=A-D\n";
		out << "A=A-D\nM=D" << std::endl;	
	}
}

void CodeWriter::writeLabel ( const std::string & label )
{
	out << "(" << functionName << "$" << label << ")" << std::endl;
}

void CodeWriter::writeIf ( const std::string & label )
{
	out << "@SP\nAM=M-1\nD=M\n"
		<< "@" << functionName << "$" << label << "\n"
		<< "D;JNE\n";
}

void CodeWriter::writeGoto ( const std::string & label )
{
	out << "@" << functionName << "$" << label << "\n"
		<< "0;JMP\n";
}

void CodeWriter::writeFunction ( const std::string & functionName, 
													int numLocals )
{
	this->functionName = functionName;
	out << "(" << functionName << ")\n";
	for ( int i = 0; i < numLocals; i++ ) {
		out << "@SP\nAM=M+1\nA=A-1\nM=0\n";
	}
}

void CodeWriter::writeCall(const std::string & functionName, int numArgs)
{
		// push ret
	out << "@RET_" << skip_counter << "\n"
		<< "D=A\n@SP\nAM=M+1\nA=A-1\nM=D\n"
		// push lcl, arg, this, that
		<< "@LCL\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n"
		<< "@ARG\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n"
		<< "@THIS\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n"
		<< "@THAT\nD=M\n@SP\nAM=M+1\nA=A-1\nM=D\n"
		// ARG = SP - n - 5
		<< "@SP\nD=M\n@5\nD=D-A\n"
		<< "@" << numArgs << "\n"
		<< "D=D-A\n@ARG\nM=D\n"
		// LCL = SP
		<< "@SP\nD=M\n@LCL\nM=D\n"
		// goto functionName
		<< "@" << functionName << "\n0;JMP\n"
		// ret label
		<< "(" << "RET_" << skip_counter++ << ")\n";
}

void CodeWriter::writeReturn()
{
		// Save return address
	out << "@5\nD=A\n@LCL\nA=M-D\nD=M\n@RET\nM=D\n"
		// *ARG = pop(), SP = ARG + 1
		<< "@SP\nA=M-1\nD=M\n@ARG\nA=M\nM=D\n"
		<< "D=A+1\n@SP\nM=D\n"
		// Restore that, this, arg, lcl
		<< "@LCL\nD=M\n@TMP\nAM=D-1\nD=M\n"
		<< "@THAT\nM=D\n@TMP\nAM=M-1\nD=M\n"
		<< "@THIS\nM=D\n@TMP\nAM=M-1\nD=M\n"
		<< "@ARG\nM=D\n@TMP\nAM=M-1\nD=M\n"
		<< "@LCL\nM=D\n"
		// Goto RET
		<< "@RET\nA=M\n0;JMP\n";
}
