#include <unordered_map>

enum KIND { STATIC, FIELD, ARG, VAR, CONST,
			POINTER, THAT, TEMP, NONE };

struct Identifier
{
	int kind, index;
	std::string type;
};

class SymbolTable
{
	private:
		int counter[4];
		std::unordered_map<std::string, Identifier> classScope;
		std::unordered_map<std::string, Identifier> varScope;
		Identifier searchTable ( const std::string & );
			
	public:
		SymbolTable();
		void print();
		void startSubroutine();
		void define ( const std::string &, const std::string &, KIND );
		int varCount( KIND );
		int indexOf ( const std::string & );
		int kindOf  ( const std::string & );
		std::string typeOf ( const std::string & );
};
