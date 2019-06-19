#include <string>
#include <fstream>

class Parser {
	
	private:
		std::string cmd;
		std::ifstream input;

	public:
		Parser ( const std::string & );
		bool hasMoreCommands();
		void advance();
		std::string commandType() const;
		std::string arg1() const;
		int arg2() const;
};
