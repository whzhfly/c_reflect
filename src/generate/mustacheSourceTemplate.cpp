

#include <string>



using namespace std;




std::string template_class_def = R"(
{{#class_defines}}
AUTO-GEN {{class_name}} (): {
{{#class_def}}
{{#field_defines}}
	{{field_type}} {{field_name}} ;
{{/field_defines}}
{{#static_field_defines}}
	{{static_field_type}} {{static_field_name}} ;
{{/static_field_defines}}
{{#method_defines}}
	{{"method_name"}} {{method_name}} () ;
{{/method_defines}}
{{#static_method_defines}}
	{{static_method_type}} {{static_field_name}} () ;
{{/static_method_defines}}
{{/class_def}}
}
{{/class_defines}}
)";


std::string template_class_dump = R"(


)";








std::string template_build_method_map =  R"(
	std::map<std::string, MetaMethod*> func_new_map = {
	{{#method_defines}}
	{ {{"method_name"}}, {{method_name}}_warpFunction  },
	{{/method_defines}}
};
)";


std::string template_get_method = R"(
MetaMethod* {{#class_defines}}::GetMethod(std::string name) const{
	auto func_iter = func_new_map.find(name);
	if(func_iter == func_new_map.end())
	{
		return nullptr;
	}
	return func_iter->second;
}";
)";

std::string template_call_function_by_name = R"(
template <typename T, typename ... Args>
Object {{#class_defines}}::CallFunctionByName(const Reference& obj, std::string name, Args... args){
	auto metaMethod = GetMethod(name);

	std::vector<void*> arg_pointers;
	(arg_pointers.push_back(reinterpret_cast<void*>(std::addressof(args))), ...);
	// arg_pointers.push_back(reinterpret_cast<void*>(std::addressof(args)));
	Object ret = metaMethod->operator()(obj, arg_pointers); // 把this 和 xx 放进去
	return ret;
};
)";