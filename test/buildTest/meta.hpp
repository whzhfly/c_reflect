

#include "reference.hpp"




class MetaType{
public:
	virtual ~MetaType() = default;
	virtual const std::string& GetName() const = 0;
};


/*
	base field type like int|float|bool...
*/
class MetaField: public MetaType{
};


class MetaMethod: public MetaType{
public:

	// template <typename... Ts>
	virtual Object operator()(const Reference& o, std::vector<void*> args) = 0;
	// MetaMethod() = default;

	virtual int GetArgsCount() const = 0;

	virtual Reference Invoke(
			const Reference& o, const std::string& name) const = 0;
};

template <typename T>
class MetaClass: public MetaType{
public:
	virtual int GetFieldCount() const = 0;
	virtual Reference GetField(
			const Reference& o, const std::string& name) const = 0;

	virtual int GetStaticFieldCount() const = 0;
	virtual Reference GetStaticField(const std::string& name) const = 0;

	virtual int GetMethodCount() const = 0;

	virtual MetaMethod* GetMethod() const = 0;

	virtual Object CallFunctionByName() = 0;

	// virtual std::vector<std::unique_ptr<IMethod>> GetMethod(
	// 		const std::string& name) const = 0;

	// virtual int GetStaticMethodCount() const = 0;
	// virtual std::vector<std::unique_ptr<IFunction>> GetStaticMethod(
	// 		const std::string& name) const = 0;

	static const constexpr int test_field = 0; // 子类看不到 todo oxygenC
};


// template <typename T>


template <typename T>
class MetaEnum: public MetaType{
public:
	virtual int GetEnumCount() const = 0;
	// virtual Reference Invoke(
	// 		const Reference& o, const std::string& name) const = 0;
};


// template <typename T> class Enum;
// template <typename T> class Class;
// template <typename T, T t> class Function;
// template <typename T, T t> class Method;

template <typename T, typename... Args>
class Method{};
