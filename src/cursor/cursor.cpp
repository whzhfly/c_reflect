#include "cursor.hpp"
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;


// string Cursor::GetTypeName(){
// 	//TODO: unfortunately, this isn't good enough. It only works as long as the
// 	// type is fully qualified.
// 	auto type = clang_getCursorType(m_cursor);
// 	return Convert(clang_getTypeSpelling(type));
// }



// void Cursor::visitChildren(Visitor visitor, void* data = nullptr){

// }

bool Cursor::ShouldCompile(){
	return true;
}

Cursor::CursorList Cursor::GetChildCursor(){
	CursorList children;

	auto visitor = [](CXCursor cursor, CXCursor parent, CXClientData data)
	{
		auto container = static_cast<std::vector<CXCursor> *>(data);

		container->emplace_back(cursor);

		if (cursor.kind == CXCursor_LastPreprocessing)
			return CXChildVisit_Break;

		return CXChildVisit_Continue; // https://clang.llvm.org/doxygen/group__CINDEX__CURSOR__TRAVERSAL.html Enumeration Type Documentation
	};

	clang_visitChildren(m_cursor, visitor, &children);
	return children;
}

bool Cursor::IsDefine() const{
	return clang_isCursorDefinition(m_cursor);
}

void Cursor::ToString(const CXString& str, string& res){
	auto cstr = clang_getCString(str); // fuck

	if (cstr == nullptr){
		clang_disposeString(str);
		res = "FUCK";
	};
	res = cstr;
	clang_disposeString(str);
	// if (!str_.size()){
	// 	return "FUCK";
	// };
};

string Cursor::Convert(const CXString& str){



	auto cstr = clang_getCString(str); // fuck


	if (cstr == nullptr){
		clang_disposeString(str);
		return "FUCK";
	};
	std::string str_ = cstr;
	clang_disposeString(str);
	// if (!str_.size()){
	// 	return "FUCK";
	// };

	return str_; // 存在 return 0的情况这里出现问题的话
}

string Cursor::GetFullDisplay(){
	std::string name;
	CXCursor cursor = m_cursor;
	while(clang_isDeclaration(clang_getCursorKind(cursor)) != 0)
	{
		std::string cur = Convert(clang_getCursorSpelling(cursor));
		if (name.empty())
		{
			name = cur;
		}
		else
		{
			name = cur + "::" + name;
		}
		cursor = clang_getCursorSemanticParent(cursor);
	}
	return name;
}

string Cursor::GetSourceFileName(){
	// 极其容易出问题
	// todo
	// 最早最外层使用 指定fileName就行

	// auto range = clang_Cursor_getSpellingNameRange(m_cursor, 0, 0);
	// auto start = clang_getRangeStart(range);

	// CXFile   file;
	// unsigned line, column, offset;
	// clang_getFileLocation(start, &file, &line, &column, &offset);

	// class 中的 function等使用这个会出问题
	// 我猜得最外层的才行
	// 具体得看函数

	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	std::cout<<"GetFileName2: " << this->GetFullDisplay()<< std::endl; // 这里都行


	std::cout<<"ADDRESS: "<<static_cast<void *>(&m_cursor)<< endl;

	auto location = clang_getCursorLocation(m_cursor); // 是不是还没有初始化？？？
	CXFile file;

	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	std::cout<<"fileFile: " << std::endl; // 这里都行
	// location-0 卡在这里了
	clang_getSpellingLocation(location, &file, nullptr, nullptr, nullptr); // 存在不是的情况 file是 None? 找不到location

	// location-1
	// auto range = clang_Cursor_getSpellingNameRange(m_cursor, 0, 0);
	// auto start = clang_getRangeStart(range);
	// unsigned line, column, offset;
	// clang_getFileLocation(start, &file, &line, &column, &offset);

	if (!file){
		std::cout<<"GetSourceFileNameError"<< std::endl;
		return "GetSourceFileNameError";
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	std::cout<<"GetFileName3: " << std::endl;

	// auto fileName = clang_getFileName(file); 报错就gle
	auto fileName = clang_File_tryGetRealPathName(file);


	auto res = Convert(fileName);

	return res;
}

string Cursor::GetName(const CXType& type)
{
	//TODO: unfortunately, this isn't good enough. It only works as long as the
	// type is fully qualified.

	std::cout<<"GetName1: " << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(300));

	std::string name;
	ToString(clang_getTypeSpelling(type), name);
	// return Convert(clang_getTypeSpelling(type));

	
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	std::cout<<"GetName2: " << std::endl;

	return name;
}

// self define function



// bool annotation_check(CXCursor cursor){
// 	std::vector<CXCursor> children = cursor_get_children(cursor);
// 	for (auto i : children)
// 		{
// 			if (i.kind == CXCursor_AnnotateAttr)
// 			{
// 				// clang_getCursorType
// 				CXString CXstr = clang_getCursorSpelling(cursor);
// 				CXString CXstr_anno = clang_getCursorSpelling(i);
// 				printf("Annotation Check: %s  %s \n", clang_getCString(CXstr), clang_getCString(CXstr_anno));
// 				return true
// 			}
// 		}
// 	return false
// }
