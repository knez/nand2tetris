#include <fstream>
#include <string>

class CodeWriter {
	
	private:
		int skip_counter = 0;
		std::ofstream out;
		std::string fileName;
	
	public:
	   ~CodeWriter();
		CodeWriter( const std::string & );
		void setFileName ( const std::string & );
		void writeArithmetic( const std::string & );
		void writePushPop( const std::string &,
						   const std::string, int );
};
