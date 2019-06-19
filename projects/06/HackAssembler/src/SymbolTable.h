#include <string>
#include <unordered_map>

class SymbolTable {
		
	private:
		std::unordered_map<std::string, int>::iterator it;
		std::unordered_map<std::string, int> map;
	
	public:
		SymbolTable();
		int GetAddress();
		void addEntry ( std::string, int );
		bool contains ( std::string );
};
