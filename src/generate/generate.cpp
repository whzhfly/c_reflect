#include "generate.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

Generate::Generate(std::string o_path): path(o_path) {};



void saveFile(const std::string& outpu_string, const std::string& output_file)
{
	fs::path out_path(output_file);

	if (!fs::exists(out_path.parent_path()))
	{
		fs::create_directories(out_path.parent_path());
	}
	// std::fstream output_file_stream(output_file, std::ios_base::out);
	std::fstream output_file_stream(output_file, std::ios_base::app);

	output_file_stream << outpu_string << std::endl;
	output_file_stream.flush();
	output_file_stream.close();
	return;
}

void Generate::TemplateRender(std::unique_ptr<TypeBase>& type){

	// 还得区分头文件和cpp文件
	// 先全部cpp吧
	// 没有继承其实没有 依赖

	Mustache::data class_defines(Mustache::data::type::list);

	if (type->GetType()== TypeBase::Type::Enum){
		// type.get() get object
		// auto* data = reinterpret_cast<P_Enum*>(type);
		// auto data = static_cast<std::unique_ptr<P_Enum>  *>(type);
		// unique_ptr<A> 与 unique_ptr<B> 之间无法直接转换，必须把原指针取出来再进行转换
		std::cout << typeid(type).name() << std::endl;
		auto x = reinterpret_cast<P_Enum*>(type.release());
		std::unique_ptr<P_Enum> enum_o(x);
		std::cout << typeid(enum_o).name() << std::endl;
	}
	else if (type->GetType()== TypeBase::Type::Class)
	{
		// std::cout<< "ddddddd"<< std::endl;
		auto cls = reinterpret_cast<P_Class*>(type.release());
		std::unique_ptr<P_Class> cls_ptr(cls);
		Mustache::data class_def;

		ClassRender(cls_ptr, class_def);
		class_defines.push_back(class_def);
	}
	else if (type->GetType()== TypeBase::Type::Function)
	{
		// // std::cout<< "ddddddd"<< std::endl;
		// auto cls = reinterpret_cast<P_Class*>(type.release());
		// std::unique_ptr<P_Class> cls_ptr(cls);
		// Mustache::data class_def;

		// FunctionRender(cls_ptr, class_def);
		// class_defines.push_back(class_def);
	}


	// tmp print todo

	// https://www.zhihu.com/question/460748427/answer/1899579618
	// to-learn
	// Mustache::mustache tmpl{
	// 	"{{#class_defines}}  AUTO-GEN {{class_name}}: {{#class_def}} {{methodName}} {{/class_def}}  {{/class_defines}}"
	// };

	// std::string redist_path4 = R"(C:\\Program Files (x86)\\Microsoft.NET\\RedistList)";
	std::string fozu = R"(
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
	{{method_type}} {{method_name}} () ;
{{/method_defines}}
{{#static_method_defines}}
	{{static_method_type}} {{static_field_name}} () ;
{{/static_method_defines}}
{{/class_def}}
}
{{/class_defines}}
)";
	Mustache::mustache tmpl{fozu};

	// tmpl.render({"class_defines", class_defines}, std::cout);

	auto render_string =  tmpl.render({"class_defines", class_defines});

	saveFile(render_string, "../tmp.cpp");
}


void Generate::TemplateRender(std::vector<std::unique_ptr<TypeBase>>& types, const Options& options){

	// header-file
	// TemplateRender()header-file
	std::cout<<"TemplateRender:  " <<types.size()<<std::endl;
	// cpp-file
	for (auto& type : types){
		TemplateRender(type);
	}

}

void Generate::FunctionRender(std::unique_ptr<P_Function>& functionPtr){

};

void Generate::EnumRender(std::unique_ptr<P_Enum>& enumPtr){

};


void Generate::ClassRender(std::unique_ptr<P_Class>& classPtr, Mustache::data& class_def){
	// 返回一个xx
	class_def.set("class_name", classPtr->className);

	// class_def.set("class_base_class_size", std::to_string(class_temp->m_base_classes.size()));
	// class_def.set("class_need_register", true);
	// method test

	// field method
	Mustache::data field_defines(Mustache::data::type::list);
	Mustache::data static_field_defines(Mustache::data::type::list);
	Mustache::data method_defines(Mustache::data::type::list);
	Mustache::data static_method_defines(Mustache::data::type::list);

	for(auto arg : classPtr->Fields){
		Mustache::data field_def;
		// std::cout<<"  "<<arg.Name<< "  "<< arg.Type <<std::endl;
		field_def.set("field_name", arg.Name);
		field_def.set("field_type", arg.Type);
		// const 等修饰符等
		field_defines.push_back(field_def);
	}

	// render static field
	// Mustache::data static_field_defines(Mustache::data::type::list);
	for(auto arg : classPtr->StaticFields){
		Mustache::data static_field_def;
		static_field_def.set("static_field_name", arg.Name);
		static_field_def.set("static_field_type", arg.Type);
		static_field_defines.push_back(static_field_def);
	}

	// render method
	// Mustache::data method_defines(Mustache::data::type::list);
	for(auto method : classPtr->Methods){
		Mustache::data method_def;
		method_def.set("method_name", method.funcName);
		method_def.set("method_type", method.ReturnType);
		method_defines.push_back(method_def);
	}

	// render static method
	// Mustache::data static_method_defines(Mustache::data::type::list);
	for(auto method : classPtr->StaticMethods){
		Mustache::data method_def;
		method_def.set("static_method_name", method.funcName);
		method_def.set("static_method_type", method.ReturnType);
		static_method_defines.push_back(method_def);
	}

	Mustache::data real_def;
	// real_def.set("methodName", "hello");
	real_def.set("field_defines", field_defines);
	real_def.set("static_field_defines", static_field_defines);
	real_def.set("method_defines", method_defines);
	real_def.set("static_method_defines", static_method_defines);


	class_def.set("class_def", real_def);
};
