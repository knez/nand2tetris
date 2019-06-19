#include <fstream>
#include <set>

class JackTokenizer 
{
	private:
		static const std::set<std::string> keywords;
		std::ifstream input;
		std::string currentToken;
		void skipComments ( char & );
	
	public:
		JackTokenizer ( const std::string & );
		bool hasMoreTokens();
		std::string token();
		std::string type();
		void advance();				
};
