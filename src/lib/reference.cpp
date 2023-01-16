#include "reference.hpp"


// static int global_id = 1;

// template <typename T>
// int GetTypeId()
// {
// 	// While it may seem like global_id could be an int rather than
// 	// std::atomic, this is in fact no true. GetTypeId<T1> does not
// 	// sync access with GetTypeId<T2>.
// 	static const int t_id = global_id++;
// 	return t_id;
// }

// template <typename T>
// Reference::Reference(T& t) : data_((void*)&t) // C-style cast avoids issues with possible constness.
// {
// }



// // Reference::Reference(const Reference& o) = default;
// // Reference& Reference::operator=(const Reference& o) = default;


// template <typename T>
// bool Reference::IsT() const
// {
// 	// return GetTypeId<T>() == id_;
// 	return true;
// }

// template <typename T>
// T & Reference::GetT() const
// {
// 	return *static_cast<T*>(data_);
// }



// void NoOp(void*) {}


// Object::Object()
// : id_(1), deleter_(NoOp)
// {
// };

// Object::~Object()
// {
// 	deleter_(data_);
// }

// Object::Object(Object&& o)
// :	deleter_(NoOp)
// {
// 	*this = std::move(o);
// }

// Object& Object::operator=(Object&& o)
// {
// 	// Release existing resource if any.
// 	deleter_(data_);

// 	id_ = o.id_;
// 	data_ = o.data_;
// 	deleter_ = move(o.deleter_);
// 	o.deleter_ = NoOp;
// 	return *this;
// }



// template <typename T>
// bool Object::IsT() const
// {
// 	return GetTypeId<T>() == id_;
// }


// template <typename T>
// const T& Object::GetT() const
// {
// 	// if (GetTypeId<T>() != id_)
// 	// {
// 	// 	throw Exception("GetT() failed: incompatible T.");
// 	// }
// 	return *static_cast<T*>(data_);
// }



// 非模板如果定义在h文件 会出现重复定义问题

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