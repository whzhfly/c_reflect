

#include "Monster.hpp"
#include "meta.hpp"
#include "reference.hpp"
#include <map>

#define Monster_SE MetaClass<Monster>
#define Monster_Function_Run_Se Function<decltype(&Monster::monster_run), (&Monster::monster_run)>
#define Monster_Static_Function_Die_Se StaticFunction<void(*)(int ), Monster::monster_die>
/*
	特化模板
*/

// using Monster_SE = MetaClass<Monster>; 貌似不行 why ?




template <typename T, T t>
class Function{};

// method 这是带this指针的
template <>
class Monster_Function_Run_Se : public MetaMethod{

	// template <typename... Ts>
	Object operator()(const Reference& o, std::vector<void*> args);

	int GetArgsCount() const;

	const std::string& GetName() const;

	Reference Invoke(
			const Reference& o, const std::string& name) const;
};





template <typename T, T t>
class StaticFunction;

template <>
class Monster_Static_Function_Die_Se: public MetaMethod{
	// template <typename... Ts>
	Object operator()(const Reference& o, std::vector<void*> args);

	int GetArgsCount() const;

	const std::string& GetName() const;

	Reference Invoke(
			const Reference& o, const std::string& name) const;
};

template <>
class MetaClass<Monster>{
public:
	static const constexpr int FieldCount = 2;
	static const constexpr int StaticFieldCount = 0;
	static const constexpr int MethodCount = 2;
	static const constexpr int StaticMethodCount = 0;

	int GetFieldCount() const;

	Reference GetField(
			const Reference& o, const std::string& name) const ;

	Reference SetField(
			const Reference& o, const std::string& name, const Reference& val) const ;

	int GetMethodCount() const;
	// std::vector<std::unique_ptr<IMethod>> GetMethod(
	// 		const std::string& name) const override;

	int GetStaticMethodCount() const;
	// std::vector<std::unique_ptr<IFunction>> GetStaticMethod(
	// 		const std::string& name) const override;

	const std::string& GetName() const;

	MetaMethod* GetMethod(std::string name) const; // 使用指针即可


	// T return Type
	//
	template <typename T, typename ... Args>
	Object CallFunctionByName(const Reference & obj, std::string, Args ... args);

	using function_map =  std::map<std::string, MetaMethod*>;
	// static function_map funcMap;
	static function_map funcMap;
		// {"a", new Monster_Function_Run_Se{};};
	// }
};


// oxygenC
// 放在Cpp中会显示
// undefined reference to `Object MetaClass<Monster>::CallFunctionByName<int, int, int>(
	//Reference const&, std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, int, int)'

template <typename T, typename ... Args>
Object Monster_SE::CallFunctionByName(const Reference& obj, std::string name, Args... args){
	auto metaMethod = GetMethod(name);

	std::vector<void*> arg_pointers;
	(arg_pointers.push_back(reinterpret_cast<void*>(std::addressof(args))), ...);
	// arg_pointers.push_back(reinterpret_cast<void*>(std::addressof(args)));
	Object ret = metaMethod->operator()(obj, arg_pointers); // 把this 和 xx 放进去
	return ret;
};


// // template <typename... Ts>

// Object Monster_Static_Function_Die_Se::operator()(const Reference& o, std::vector<void*> args){
// 	// function_2(*reinterpret_cast<const int*>(args[0]));
// 	Monster::monster_die();
// 	return Object();
// };

// // template <typename... Ts>

// Object Monster_Function_Run_Se::operator()(const Reference& o, std::vector<void*> args){
// 	((o.GetT<Monster>()).*(&Monster::monster_run))(); // this 指针的技巧
// 	return Object();
// };
