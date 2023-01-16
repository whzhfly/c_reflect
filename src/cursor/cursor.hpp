#pragma once

#include <string>
#include <vector>
#include "../3rd/clang-c/Index.h"



using namespace std;


class Cursor{

public:
	typedef CXCursorVisitor Visitor;
	typedef std::vector<Cursor> CursorList;
	// cursor for CXCursor get
	Cursor(const CXCursor& rawCursor) : m_cursor(rawCursor) {
		// std::cout<<"raw ca"<<std::endl;
		// std::cout<<rawCursor.xdata<<std::endl;
		// 这里的 m_cursor 会消失 ！！！！！！！！
		// printf("the cursorSpelling is: %s\n", clang_getCString(clang_getCursorSpelling(rawCursor)));
		// 存在没有保存的情况
	};

	// string GetSpelling(); // class name
	string GetFullDisplay(); // namespace::A:xx
	// string GetTypeName(); // just type such as int| bool
	string GetSourceFileName();

	static string GetName(const CXType& type);


	static string Convert(const CXString& s); // convert std move
	static void ToString(const CXString& s, string& str); // convert std move

	bool IsDefine() const;
	// bool IsRecursePublic() const;

	bool ShouldCompile();
	CursorList GetChildCursor();

public:
	CXCursor m_cursor;
	CursorList childCursor;
};
