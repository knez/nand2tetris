#include <fstream>
#include <string>

class CodeWriter {
	
	private:
		int skip_counter = 0;
		std::ofstream out;
		std::string fileName;
		std::string functionName;
	
	public:
	   ~CodeWriter();
		CodeWriter( const std::string & );
		void setFileName ( const std::string & );
		void writeInit();
		void writeArithmetic( const std::string & );
		void writePushPop( const std::string &,
						   const std::string, int );
		void writeLabel ( const std::string & );
		void writeIf ( const std::string & );
		void writeGoto ( const std::string & );
		void writeFunction ( const std::string &, int );
		void writeCall ( const std::string &, int );
		void writeReturn ();
};
