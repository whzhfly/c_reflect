
#include "parse.hpp"

// #include "llvm/Support/CommandLine.h"
// using namespace llvm;




// void cursor_print(CXCursor cursor){
// 	CXCursorKind kind=clang_getCursorKind(cursor);
// 	// printf("the cursorSpelling is: %s\n", clang_getCString(clang_getCursorSpelling(cursor)));
// 	// printf("the cursorKindSpelling is:%s\n",clang_getCString(clang_getCursorKindSpelling(kind)));
// }

int collectDeclareCursor(CXCursor cursor, CXCursor parent, CXClientData client_data){
	auto* data = reinterpret_cast<SchemaModule*>(client_data);

	std::unique_ptr<TypeBase> type;
	Cursor* mCursor = new Cursor(cursor); // allocate new
	// Cursor mCursor(cursor); error cause of xx
	switch (clang_getCursorKind(cursor))
		{
			case CXCursor_EnumDecl:
				type = std::make_unique<P_Enum>(P_Enum(*mCursor)); // make unique 类似new
				break;
			case CXCursor_ClassDecl:
			case CXCursor_StructDecl:
			{
				type = std::make_unique<P_Class>(P_Class(*mCursor));
				// P_Class* cls = new P_Class(*mCursor);
				// type = std::make_unique<P_Class>(*cls);
				break;
			}
			case CXCursor_FunctionDecl:
				type = std::make_unique<P_Function>(P_Function(*mCursor));
				break;
			default:
				break;
		}
	if (type == nullptr){
		// 不加这个会出现神器的错误
		// 下面的判断会有问题
		delete mCursor;
		return 0;
	}

	// if (type->shouldCompile()){
	// 	data->types->push_back(std::move(type));
	// }
	// const string& name = type->GetFullName();
	// if (type
	// 		&& !name.empty()
	// 		&& parser::IsRecursivelyPublic(cursor)
	// 		&& !(name.back() == ':')
	// 		&& regex_match(name, data->options->include)
	// 		&& !regex_match(name, data->options->exclude))

	bool flag = type->cursor.ShouldCompile();
	if (flag)
	{
		
		std::cout <<"Add to file Type is " <<type->full_name_<<" and cur-size: "<< data->types->size()<<std::endl;
		data->types->push_back(std::move(type)); // move 之后就删除了 
		// std::cout <<"Add to file Type is " <<type->full_name_<< std::endl; move 之后就删除了  导致这里是type是空指针!!
	}
	return 0;
}

void FileParse::Serialize(){
	for (auto& module : m_schema_modules){
		auto iter_type = module.second.types;
		std::cout<<"FileParse::Serialize:: Index-for-modules: " <<iter_type->size()<< " and module-filename: "<< module.first<<std::endl;
		for (auto it=iter_type->begin(); it!=iter_type->end(); it++){
			(*it)->BuildTypeData();
			(*it)->TestShowTypeData();
			for (auto gen : this->m_generators){
				// 这里得有options
				// gen->TemplateRender(*it); // his means that std::unique_ptr cannot be copied. user reference
			}
		}
		for (auto gen : this->m_generators){
			if (module.second.types->size() > 0){
				gen->TemplateRender(*module.second.types, *module.second.options);
			}
		}
	}
}

void FileParse::ParseFile(std::string file_name){
	CXIndex index = clang_createIndex(0,0);
	const char *args[] = {
		"-I/usr/include",
		"-I."
	};

	// GetModuleFileName(NULL, )
	// std::string s = std::filesystem::current_path();

	std::cout << "FileParse::ParseFile---->>>>" << file_name << std::endl;

	const char * file = file_name.data();
	CXTranslationUnit translationUnit = clang_parseTranslationUnit(index, file, args, 0, NULL, 0, CXTranslationUnit_None);

	if (translationUnit == nullptr)
	{
		std::cout << "Unable to parse translation unit. Quitting." << std::endl;
		exit(-1);
	}

	// CXString CXstr = clang_getTranslationUnitSpelling(translationUnit);
	CXCursor cursor = clang_getTranslationUnitCursor(translationUnit);

	try
	{
		Options op;
		op.include_path = "ref_include->Get()";
		op.out_hpp_path = "out_hpp->Get()";
		op.out_cpp_path = "out_cpp->Get()";

		SchemaModule* moduleData = new SchemaModule(); // 后面保存起来 todo 一次性保存 指针保存
		vector<unique_ptr<TypeBase>>* m = new vector<unique_ptr<TypeBase>>; // 这里不用new 会自动释放... todo 后面研究下
		// 第一次见vector主动释放的。。。。
		moduleData->options = &op;
		moduleData->types = m;

		auto ambPtr = [](CXCursor c, CXCursor parent, CXClientData client_data){
				collectDeclareCursor(c, parent, client_data);
				return CXChildVisit_Recurse;
		};

		clang_visitChildren(cursor, ambPtr, moduleData);

		m_schema_modules[file_name] = (*moduleData);

		// 消除引用
		clang_disposeTranslationUnit(translationUnit); // 或许不能在这个时候消除 得全部析构消除才行
		clang_disposeIndex(index);
	}
	catch(std::exception &e){
		std::cerr << "Exception caught : " << e.what() << std::endl;
	}
}
