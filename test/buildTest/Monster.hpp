
#include <iostream>


using namespace std;




class Monster{
public:
	int monster_id;
	float monster_speed;


	Monster(int m_id, float m_sp) : monster_id(m_id), monster_speed(m_sp){}; 
	void monster_run(){
		monster_id = 2;
		std::cout<< "SDD"<< std::endl;
	};

	static void monster_die(int type){
		// Monster::obj_type = type;
		std::cout<< type<< std::endl;
	};


	// static int obj_type;
};




