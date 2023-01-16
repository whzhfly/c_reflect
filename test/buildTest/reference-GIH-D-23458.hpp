

#ifndef REFERENCE_TYPES_HPP
#define REFERENCE_TYPES_HPP

#include <atomic>
#include <functional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>


// 模板类 函数必须在h文件中实现 Reference Object 函数在 reference.cpp 中会报错
// https://blog.csdn.net/dfq12345/article/details/78212002
// oxygenC-learn


// multi define 
// https://blog.csdn.net/bookscape/article/details/84487810
/*
	指向所有类型的PTR
	万能指针
	py_object
*/

// std::atomic<int> global_id;
// static int global_id = 0;

// static int GetTypeId()
// {
// 	// While it may seem like global_id could be an int rather than
// 	// std::atomic, this is in fact no true. GetTypeId<T1> does not
// 	// sync access with GetTypeId<T2>.
// 	static const int t_id = global_id++;
// 	return t_id;
// }

class Reference final{

public:
	template<typename T>
	Reference(T& t);

	Reference(const Reference& o) = default;
	Reference & operator=(const Reference & o) = default;

	template<typename T>
	bool IsT() const;

	template<typename T>
	T & GetT() const;


	void* data_ = nullptr;
};


class Object final
{
public:
	// Constructs an Object with stored type of void.
	Object();

	// Constructs an Object with a copy of T.
	template <typename T>
	explicit Object(T&& t);

	Object(Object&& o);
	Object& operator=(Object&& o);
	Object(const Object& o) = delete;
	Object& operator=(const Object& o) = delete;

	~Object();

	template <typename T>
	bool IsT() const;

	template <typename T>
	const T& GetT() const;

	bool IsVoid() const;

private:
	int id_;
	void* data_ = nullptr;
	std::function<void(void*)> deleter_;
};



static int global_id = 1;

template <typename T>
int GetTypeId()
{
	// While it may seem like global_id could be an int rather than
	// std::atomic, this is in fact no true. GetTypeId<T1> does not
	// sync access with GetTypeId<T2>.
	static const int t_id = global_id++;
	return t_id;
}

template <typename T>
Reference::Reference(T& t) : data_((void*)&t) // C-style cast avoids issues with possible constness.
{
}



// Reference::Reference(const Reference& o) = default;
// Reference& Reference::operator=(const Reference& o) = default;


template <typename T>
bool Reference::IsT() const
{
	// return GetTypeId<T>() == id_;
	return true;
}

template <typename T>
T & Reference::GetT() const
{
	return *static_cast<T*>(data_);
}



void NoOp(void*) {}


Object::Object()
: id_(GetTypeId<void>()), deleter_(NoOp)
{
};

Object::~Object()
{
	deleter_(data_);
}

Object::Object(Object&& o)
:	deleter_(NoOp)
{
	*this = std::move(o);
}

Object& Object::operator=(Object&& o)
{
	// Release existing resource if any.
	deleter_(data_);

	id_ = o.id_;
	data_ = o.data_;
	deleter_ = move(o.deleter_);
	o.deleter_ = NoOp;
	return *this;
}



template <typename T>
bool Object::IsT() const
{
	return GetTypeId<T>() == id_;
}


template <typename T>
const T& Object::GetT() const
{
	// if (GetTypeId<T>() != id_)
	// {
	// 	throw Exception("GetT() failed: incompatible T.");
	// }
	return *static_cast<T*>(data_);
}



#endif //REFLANG_TYPES_HPP
