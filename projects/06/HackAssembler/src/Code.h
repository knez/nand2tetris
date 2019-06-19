#include <string>
#include <map>

class Code {
	
	private:
		std::string comp;
		static const std::map<std::string,std::string> c_map;
		static const std::map<std::string,std::string> d_map;
		static const std::map<std::string,std::string> j_map;
	
	
	public:
		std::string decode  (const std::string &, 
							 const std::string &,
							 const std::string &);			
};
