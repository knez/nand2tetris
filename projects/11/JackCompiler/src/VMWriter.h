#include <fstream>
			   
class VMWriter
{
	private:
		int labelCount;
		std::ofstream out;
		void writeSegment ( int );	
	
	public:
		VMWriter ( const std::string & );
	   ~VMWriter();
	    int labelInc();
		void writePush ( int, int );
		void writePop ( int, int );
		void writeArithmetic ( const char );
		void writeLabel ( const std::string &, int );
		void writeGoto ( const std::string &, int );
		void writeIf ( const std::string &, int );
		void writeCall ( const std::string &, int );
		void writeFunction ( std::string &, int );
		void writeReturn();
};
