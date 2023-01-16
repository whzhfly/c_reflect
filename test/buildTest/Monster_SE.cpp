#include "Monster_SE.hpp"


/*
	monster_run
*/

static const std::string monster_run_name = "Monster::monster_run";
const std::string& Monster_Function_Run_Se::GetName() const{
	return monster_run_name;
}

int Monster_Function_Run_Se::GetArgsCount()const {
	return 0;
};



// template <typename... Ts>
Object Monster_Function_Run_Se::operator()(const Reference& o, std::vector<void*> args){
	((o.GetT<Monster>()).*(&Monster::monster_run))(); // this 指针的技巧
	return Object();
};

Reference Monster_Function_Run_Se::Invoke(
	const Reference& o, const std::string& name) const
{
	return Reference(name);
};



/*
	monster_die
*/



// template <typename... Ts>
Object Monster_Static_Function_Die_Se::operator()(const Reference& o, std::vector<void*> args){
	auto arg = *reinterpret_cast<const float*>(args[0]);
	std::cout<< "argarg: "<< arg <<std::endl;
	Monster::monster_die(arg);
	return Object();
};

static const std::string monster_die_name = "Monster::monster_die";
const std::string& Monster_Static_Function_Die_Se::GetName() const{
	return monster_die_name;
}

int Monster_Static_Function_Die_Se::GetArgsCount()const {
	return 0;
};

Reference Monster_Static_Function_Die_Se::Invoke(
			const Reference& o, const std::string& name) const
{
	return Reference(name);
};

/*
	monster
*/

// https://www.cnblogs.com/qicosmos/p/4325949.html 可变模板的参数展开 折叠表达式 ... 类似lambda
// https://greedysky.github.io/2020/12/01/C++17%20%E6%8A%98%E5%8F%A0%E8%A1%A8%E8%BE%BE%E5%BC%8F/


// template <typename T, typename ... Args>
// Object Monster_SE::CallFunctionByName(const Reference& obj, std::string name, Args... args){
// 	auto metaMethod = GetMethod(name);

// 	std::vector<void*> arg_pointers;
// 	(arg_pointers.push_back(reinterpret_cast<void*>(std::addressof(args))), ...);
// 	// arg_pointers.push_back(reinterpret_cast<void*>(std::addressof(args)));
// 	Object ret = metaMethod->operator()(obj, arg_pointers); // 把this 和 xx 放进去
// 	return ret;
// };

Monster_Function_Run_Se* mom1 = new Monster_Function_Run_Se();

Monster_Static_Function_Die_Se* mom2 = new Monster_Static_Function_Die_Se();


std::map<std::string, MetaMethod*> func_new_map = {
	{"monster_run", mom1},
	{"monster_die", mom2},
};


MetaMethod* Monster_SE::GetMethod(std::string name) const{
	auto func_iter = func_new_map.find(name);
	if(func_iter == func_new_map.end())
	{
		return nullptr;
	}
	return func_iter->second;
};


int Monster_SE::GetFieldCount() const{
	return Monster_SE::FieldCount;
}

int Monster_SE::GetMethodCount() const
{
	return Monster_SE::MethodCount;
}

static const std::string Monster_SE_name = "Monster";
const std::string& Monster_SE::GetName() const{
	return Monster_SE_name;
}

Reference Monster_SE::SetField(const Reference& r, const std::string& name, const Reference& val) const{
	if (name == "monster_id")
	{
		Monster & object = r.GetT<Monster>();
		auto & tar_valve = val.GetT<int>();
		object.monster_id = tar_valve;
		return Reference(object.monster_id);
	}
	else
	{
		Monster & object = r.GetT<Monster>();
		auto & tar_valve = val.GetT<float>();
		object.monster_speed = tar_valve;
		return Reference(object.monster_speed);
	}
};

Reference Monster_SE::GetField(const Reference& r, const std::string& name) const{
	if (name == "monster_id")
	{
		Monster & object = r.GetT<Monster>();
		return Reference(object.monster_id);
	}
	else if(name == "monster_speed")
	{
		Monster & object = r.GetT<Monster>();
		return Reference(object.monster_speed);
	}
	// else
	// {
	// 	throw Exception("Invalid Reference passed to GetField().");
	// }
	else {
		Monster & object = r.GetT<Monster>();
		return Reference(object.monster_speed);
	};
}
