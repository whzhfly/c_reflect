#include "../cursor/cursor.hpp" // include cursor
#include "../parse.typeinfo/types.hpp"
#include <unordered_map>
#include <iostream>
#include <memory>
#include "../generate/generate.hpp"


struct SchemaModule{
	const Options* options;
	vector<unique_ptr<TypeBase>>* types;
};	// store file type inf


class FileParse{
public:
	void ParseFile(std::string file_name);
	void Serialize();
	std::unordered_map<std::string, SchemaModule> m_schema_modules;
	std::vector<Generate*> m_generators;
	// std::string fileName;
};


