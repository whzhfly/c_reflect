

#ifndef REFERENCE_TYPES_HPP
#define REFERENCE_TYPES_HPP

/**
 * @file types.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-11-10
 * 
 * @copyright Copyright (c) 2022
 * 
 * this file define the basic class when use clang to parse a class that we want,
 * such as we meet a class MyClass
 * use the P_Class to store the message about MyClass's class-member data 
 * 
 * and after we finish record all message we want
 * we use the P_Class to generate the meta class
 */


#include <iostream>
#include <string>
#include <vector>
#include "../cursor/cursor.hpp"


struct NamedObject
{
	std::string Name;
	std::string Type;
};


class TypeBase{
public:
	enum class Type
	{
		Enum,
		Function,
		Class,
	};

public:
	TypeBase(Cursor& cursor);
	TypeBase(Cursor& cursor, Cursor* parent);
	virtual ~TypeBase();

	virtual Type GetType() const = 0;
	virtual void BuildTypeData()= 0;
	virtual void TestShowTypeData()= 0;
	// const std::string& GetFile() const;

public:
	Cursor* parent_cursor; // 方便获取parent todo 文件相关 需要先初始化
	Cursor cursor;
	std::string file_;
	std::string full_name_;
};




class P_Enum: public TypeBase {

public:
	P_Enum(Cursor& cursor);
	using ValueList = std::vector<std::pair<std::string, int>>;

	// P_Enum(Cursor cursor);
	Type GetType() const;
	void BuildTypeData();
	void TestShowTypeData();
	ValueList enumValues;
	std::string enumName;
};

class P_Function: public TypeBase {
public:
	P_Function(Cursor& cursor);
	P_Function(Cursor& cursor, Cursor* parent);
	Type GetType() const override;
	void BuildTypeData();
	void TestShowTypeData();
	static P_Function GetMethodFromCursor(Cursor cursor);

	std::string funcName;
	std::vector<NamedObject> Arguments;
	std::string ReturnType;
};


class P_Class: public TypeBase {

public:
	P_Class(Cursor& cursor);
	using MethodList = std::vector<P_Function>;
	using FieldList = std::vector<NamedObject>;

public:
	// P_Class(std::string file, std::string full_name);
	Type GetType() const override;
	void BuildTypeData();
	void TestShowTypeData();

	std::string className;
	MethodList Methods;
	MethodList StaticMethods;

	FieldList Fields;
	FieldList StaticFields;
};


#endif //REFLANG_TYPES_HPP
