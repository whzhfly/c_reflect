

#include <string>

using namespace std;



std::string template_include = R"(
#include "meta.hpp"
#include "{{file_name}}"
#include "reference.hpp"
#include <map>
)";



// std::string template_class_helper_define = R"(
// {{#class_defines}}
// #define {{class_name}}_Helper MetaClass<{{class_name}}>
// {{#class_def}}
// #define Monster_Function_Run_Se Function<decltype(&Monster::monster_run), (&Monster::monster_run)>
// #define Monster_Static_Function_Die_Se StaticFunction<void(*)(int ), Monster::monster_die>
// {{/class_def}}
// {{#class_defines}}
// )";

std::string template_base_define =R"(
template <typename T, T t>
class Function{};

template <typename T, T t>
class StaticFunction;


)";

std::string template_class_helper_define = R"(
{{#class_defines}}
template <>
MetaClass<{{class_name}}>{
public:
	Reference GetField(
			const Reference& o, const std::string& name) const;
	Reference SetField(
			const Reference& o, const std::string& name, const Reference& val) const;

	int GetFieldCount() const;
	int GetMethodCount() const;
	int GetStaticMethodCount() const;

	const std::string& GetName() const;
	MetaMethod* GetMethod(std::string name) const; // 使用指针即可

	using function_map = std::map<std::string, MetaMethod*>;
	static function_map funcMap;
}


// static method

// method

{{#method_defines}}

class Function<decltype(&{{class_name}}}::{{method_name}}), (&{{class_name}}}::{{method_name}})> : public MetaMethod{
	Object operator()(const Reference& o, std::vector<void*> args);

	int GetArgsCount() const;

	const std::string& GetName() const;

	Reference Invoke(
			const Reference& o, const std::string& name) const;
}
{{/method_defines}}


{{#static_method_defines}}

class StaticFunction<{{return_type}}}(*)({{arg_type_list}}), {{class_name}}}::{{method_name}}> : public MetaMethod{
	Object operator()(const Reference& o, std::vector<void*> args);

	int GetArgsCount() const;

	const std::string& GetName() const;

	Reference Invoke(
			const Reference& o, const std::string& name) const;
}
{{/static_method_defines}}

{{/class_defines}}
)";
