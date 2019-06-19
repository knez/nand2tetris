#include <string>
#include <fstream>

class Parser {
	
	private:
		std::ifstream input;
		std::string cmd;
		
	public:
		Parser ( char * );
		bool hasMoreCommands();
		void advance();
		void reset();
		std::string commandType();
		std::string symbol();
		std::string dest() const;
		std::string comp() const;
		std::string jump() const;
};
