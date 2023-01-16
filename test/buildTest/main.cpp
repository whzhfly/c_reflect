#include <iterator>
#include <stdio.h>
#include <iostream>
#include <string>
#include "Monster_SE.hpp"




using namespace std;
// https://blog.csdn.net/huanghxyz/article/details/85280144

// cmake 重复main 定义 问题 CMakeCXXCompilerId.cpp 如下
// stackoverflow forever god
//https://blog.csdn.net/xiaoheqin/article/details/52190002
// https://blog.csdn.net/u013245162/article/details/103894364
// https://stackoverflow.com/questions/41495301/c-compilation-error-multiple-definition-of-main-but-only-1-main-function-in
// file(GLOB_RECURSE. 问题

int main(int argc, char** argv){

	Monster_SE se;

	std::cout<< "class name: "<< se.GetName()<<std::endl;
	std::cout<<"FieldCount "<< se.FieldCount<<std::endl;
	std::cout<<"GetMethodCount "<< se.GetMethodCount()<<std::endl;


	Monster boss{2, 3.5};

	Reference m_speed = se.GetField(boss, "monster_speed");
	Reference m_id = se.GetField(boss, "monster_id");

	std::cout<<"m_speed "<< m_speed.GetT<float>() <<std::endl;
	std::cout<<"m_id "<< m_id.GetT<int>() <<std::endl;
	// std::cout<<"id: "<< se.GetField(r_speed, "speed")<<std::endl;
	// std::cout<<"speed: "<< se.GetField(r_id, "speed")<<std::endl;
	int new_id = 10;
	float new_speed  = 20.3;
	Reference nem_speed = se.SetField(boss, "monster_speed", new_speed);
	Reference nem_id = se.SetField(boss, "monster_id", new_id);


	std::cout<<"m_speed "<< nem_speed.GetT<float>() <<std::endl;
	std::cout<<"m_id "<< nem_id.GetT<int>() <<std::endl;

	auto me = se.GetMethod("monster_run");
	std::cout<<me->GetName()<< std::endl;


	int arg1 = 1;
	int arg2 = 1;

	auto me2 =se.CallFunctionByName<int>(boss, "monster_run", arg1, arg2); // 如果取消 const 就不行 todo oxygenC 可能和隐式转化的临时变量有关

	Reference new_m_id = se.GetField(boss, "monster_id");
	std::cout<<"monster_id "<< new_m_id.GetT<int>() <<std::endl;


	auto method_die = se.GetMethod("monster_die");
	std::cout<<method_die->GetName()<< std::endl;

	float arg3 = 3.5;
	auto me3 =se.CallFunctionByName<float>(boss, "monster_die", arg3); // 如果取消 const 就不行 todo oxygenC 可能和隐式转化的临时变量有关
};
