
#include <iostream>
#include <string>
#include "../parse.typeinfo/types.hpp"
#include <unordered_map>
#include<memory>
#include "../3rd/mustache/mustache.hpp"
using namespace std;


// using namespace kainjow::mustache;
namespace Mustache = kainjow::mustache;

struct Options
{
	std::string include_path;
	std::string out_hpp_path;
	std::string out_cpp_path;
	//TODO: bool standalone = false;
};

class Generate{

public:
	Generate(std::string o_path);
	void TemplateRender(std::unique_ptr<TypeBase>& type);
	void TemplateRender(std::vector<std::unique_ptr<TypeBase>>& types, const Options& options);


	void ClassRender(std::unique_ptr<P_Class>& type, Mustache::data& class_def);
	void EnumRender(std::unique_ptr<P_Enum>& type);
	void FunctionRender(std::unique_ptr<P_Function>& type);

	std::string path;
};
