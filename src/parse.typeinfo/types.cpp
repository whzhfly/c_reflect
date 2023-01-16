

#include "types.hpp"
#include <chrono>
#include <thread>


TypeBase::TypeBase(Cursor& rawCursor):
	cursor(rawCursor), full_name_(rawCursor.GetFullDisplay()), parent_cursor(nullptr)
	{

		file_ = "tmp_file_name";
		// std::this_thread::sleep_for(std::chrono::milliseconds(30));
		// std::cout << "TypeBase COnstruct"<<std::endl; 
		std::cout << "TypeBase COnstruct1111"<< file_ <<std::endl; 
		// file_ = rawCursor.GetSourceFileName(); // file 容易出问题
	};

TypeBase::TypeBase(Cursor& rawCursor, Cursor* parent):
	cursor(rawCursor) ,full_name_(rawCursor.GetFullDisplay()), parent_cursor(parent)
	{

		// std::cout << "TypeBase COnstruct2222"<<std::endl; 
		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		// file_ = parent_cursor->GetSourceFileName(); // file 容易出问题 需要 parent_cursor
		file_ = "tmp_file_name";
	};


TypeBase::~TypeBase() = default;


/*
	Enum
*/

P_Enum::P_Enum(Cursor& rawCursor) : TypeBase(rawCursor), enumName(Cursor::Convert(clang_getCursorSpelling(rawCursor.m_cursor))) {};

TypeBase::Type P_Enum::GetType() const{
	return Type::Enum;
}

void P_Enum::BuildTypeData(){
	auto ambPtr = [](CXCursor c_cursor, CXCursor parent, CXClientData client_data){
			if (clang_getCursorKind(c_cursor) == CXCursor_EnumConstantDecl)
			{
				string name = Cursor::Convert(clang_getCursorSpelling(c_cursor));
				int value = static_cast<int>(clang_getEnumConstantDeclValue(c_cursor));
				reinterpret_cast<ValueList*>(client_data)->emplace_back(name, value);
			}
			return CXChildVisit_Continue;
	};
	clang_visitChildren(cursor.m_cursor, ambPtr, &enumValues);
}

void P_Enum::TestShowTypeData(){
	std::cout<< "Enum--> : "<< enumName  << std::endl;
	for(auto value : enumValues){
		std::cout<<"  "<<value.first<< "  "<<value.second  <<std::endl;
	}
	std::cout<< "--<: "<<std::endl;
}

/*
	Function
*/

P_Function::P_Function(Cursor& rawCursor) : TypeBase(rawCursor)
	{
		// std::this_thread::sleep_for(std::chrono::milliseconds(30));
		// std::cout << "function COnstruct"<<std::endl; 

		// funcName = "dsdasd";
		// ReturnType = "d222";
		// // funcName(Cursor::Convert(clang_getCursorSpelling(rawCursor.m_cursor)));
		// // ReturnType(Cursor::GetName(clang_getResultType(clang_getCursorType(rawCursor.m_cursor))));

		// std::cout<<"P_Function Construct> : "<< funcName  << std::endl;
		// auto type = clang_getCursorType(cursor.m_cursor);
		// ReturnType =Cursor::GetName(clang_getResultType(type));
		// std::cout<< "ReturnType"<< ReturnType;
	};


P_Function::P_Function(Cursor& rawCursor, Cursor* parent) : TypeBase(rawCursor, parent)
	{
		// std::this_thread::sleep_for(std::chrono::milliseconds(30));
		// std::cout << "function COnstruct"<<std::endl; 

		// funcName = "dsdasd";
		// ReturnType = "d222";
		// std::cout<<"P_Function Construct> : "<< funcName  << std::endl;
	};

TypeBase::Type P_Function::GetType() const{
	return Type::Function;
}

void P_Function::BuildTypeData(){
	/*
		argument 信息
	*/
	auto type = clang_getCursorType(cursor.m_cursor);

	funcName = Cursor::Convert(clang_getCursorSpelling(cursor.m_cursor));

	int num_args = clang_Cursor_getNumArguments(cursor.m_cursor);
	for (int i = 0; i < num_args; ++i)
	{
		auto arg_cursor = clang_Cursor_getArgument(cursor.m_cursor, i);
		NamedObject arg;
		arg.Name = Cursor::Convert(clang_getCursorSpelling(arg_cursor));
		if (arg.Name.empty())
		{
			arg.Name = "nameless";
		}
		auto arg_type = clang_getArgType(type, i);
		arg.Type = Cursor::GetName(arg_type);
		Arguments.push_back(arg);
	}
	ReturnType =Cursor::GetName(clang_getResultType(type));
	std::cout<<"P_Function ReturnType> : "<< ReturnType  << std::endl;
}

void P_Function::TestShowTypeData(){
	std::cout<< "Function--> : "<< funcName  << std::endl;
	for(auto arg : Arguments){
		std::cout<<"  "<<arg.Name<< "  "<< arg.Type <<std::endl;
	}
	std::cout<< "return : "<< ReturnType  << std::endl;
	std::cout<< "--<: "<<std::endl;
}

/*
	Class
*/


P_Class::P_Class(Cursor& rawCursor) : TypeBase(rawCursor),
	className(Cursor::Convert(clang_getCursorSpelling(rawCursor.m_cursor))) {};

TypeBase::Type P_Class::GetType() const{
	return Type::Class;
}


void P_Class::BuildTypeData(){
	/*
		field | static field
		method| static method
	*/

	std::cout<<"P_Class-BuildTypeData"<<std::endl;
	// import 多个文件这里会报错

	Cursor* parent_cursor = &cursor;
	// todo oxygen
	auto ambPtr = [](CXCursor c_cursor, CXCursor parent, CXClientData client_data){
			auto* c = reinterpret_cast<P_Class*>(client_data);
			std::cout <<"typeid: "<< typeid(c_cursor).name() << std::endl; 
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (clang_getCXXAccessSpecifier(c_cursor) == CX_CXXPublic)
			// add annotation check
			{
				std::cout << "clang_getCursorKind"<<int(clang_getCursorKind(c_cursor))<<std::endl; 
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				switch (clang_getCursorKind(c_cursor))
				{
					case CXCursor_CXXMethod:
						if (clang_CXXMethod_isStatic(c_cursor) != 0)
						{
							// std::cout << "clang_CXXMethod_isStatic"<<std::endl; 
							Cursor cursor(c_cursor);
							P_Function function(cursor);
							function.BuildTypeData();
							c->StaticMethods.push_back(function); // 会复制 copy 可以使用指针管理减少copy todo
						}
						else
						{
							Cursor* function_cursor = new Cursor(c_cursor); // 后面delete
							// // std::cout << "function"<<std::endl; 
							// std::cout<<"GetFullDisplay: "<< function_cursor.GetFullDisplay()<< std::endl;

							// std::this_thread::sleep_for(std::chrono::milliseconds(10));
							// std::cout <<function_cursor.GetSourceFileName()<< std::endl;

							// std::this_thread::sleep_for(std::chrono::milliseconds(30));
							// std::cout << "function2"<<std::endl; 
							P_Function* function = new P_Function(*function_cursor); // 卡在这里了？？？
							// Cursor *p = new Cursor(parent); // todo oxygen 貌似不支持这个？
							// P_Function function{function_cursor, p}; // 卡在这里了？？？
							std::cout << "function3"<<std::endl; 
							std::this_thread::sleep_for(std::chrono::milliseconds(30));
							function->BuildTypeData();
							c->Methods.push_back(*function);
						}
						break;
					case CXCursor_FieldDecl: {
							// Cursor* cursor = new Cursor(c_cursor);
							std::cout << "CXCursor_FieldDecl____1"<<std::endl;
							std::this_thread::sleep_for(std::chrono::milliseconds(1000));

							NamedObject* n_field = new NamedObject();
							// Cursor::ToString(clang_getCursorSpelling(c_cursor), n_field->Name);
							n_field->Name = "dasd";

							std::cout << "CXCursor_FieldDecl____2"<<std::endl;
							std::this_thread::sleep_for(std::chrono::milliseconds(1000));
							// n_field->Name = Cursor::Convert(clang_getCursorSpelling(c_cursor));
							auto i_type = clang_getCursorType(c_cursor); // 这里也卡了？？

							std::cout << "CXCursor_FieldDecl____23"<<std::endl;
							std::this_thread::sleep_for(std::chrono::milliseconds(1000));

							n_field->Type = Cursor::GetName(i_type);

							std::cout << "CXCursor_FieldDecl____3"<<std::endl;
							// n_field->Type = "int";
							c->Fields.push_back(*n_field);
							break;
						}
					case CXCursor_VarDecl:
						{
							Cursor cursor(c_cursor);
							NamedObject* n_field = new NamedObject();
							Cursor::ToString(clang_getCursorSpelling(c_cursor), n_field->Name);
							// Cursor::ToString(clang_getCursorType(c_cursor), n_field->Name);
							// n_field->Name = Cursor::Convert(clang_getCursorSpelling(c_cursor));
							n_field->Type = Cursor::GetName(clang_getCursorType(c_cursor));
							c->StaticFields.push_back(*n_field);

							// NamedObject  s_field;
							// s_field.Name = Cursor::Convert(clang_getCursorSpelling(c_cursor));
							// s_field.Type = Cursor::GetName(clang_getCursorType(c_cursor));
							// c->StaticFields.push_back(s_field);
							break;
						}
					default:
						break;
					}
			}
			return CXChildVisit_Continue;
		};
	// std::cout << typeid(cursor).name() << std::endl;
	// std::cout << typeid(cursor.m_cursor).name() << std::endl;
	std::cout<<"Visit_FIle: "<< file_<< " Full Name: "<<full_name_<<std::endl;
	// clang_getCursorSpelling(cursor.m_cursor);
	// printf("the cursorSpelling is: %s\n", clang_getCString(clang_getCursorSpelling(cursor.m_cursor)));
	// std::cout<<cursor.m_cursor.xdata<<std::endl;
	// std::cout<<int(cursor.m_cursor.kind)<<std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::cout<<"Cursor: "<< cursor.GetFullDisplay() <<std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	
	printf("the cursorSpelling is: %s\n", clang_getCString(clang_getCursorSpelling(cursor.m_cursor)));
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	
	clang_visitChildren(cursor.m_cursor, ambPtr, this); // cursor 是个空？
};


void P_Class::TestShowTypeData(){
	std::cout<< "class--> : "<< className  << std::endl;
	for(auto method : Methods){
		method.TestShowTypeData();
	}
	for(auto method : StaticMethods){
		method.TestShowTypeData();
	}
	for(auto arg : Fields){
		std::cout<<"  "<<arg.Name<< "  "<< arg.Type <<std::endl;
	}
	for(auto arg : StaticFields){
		std::cout<<"  "<<arg.Name<< "  "<< arg.Type <<std::endl;
	}
	std::cout<< "--<: "<<std::endl;
}


