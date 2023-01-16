#include <iostream>
#include <vector>
#include <utility>
#include <functional>


using namespace std;





int Add(int& a, int& b){
	int res =  a+ b;
	std::cout << a<< b << std::endl;
	std::cout << res << std::endl;
	return res;
}




// https://blog.csdn.net/qq_43325061/article/details/118415538
// https://blog.csdn.net/tcy23456/article/details/125131811




template<typename o, typename ... Args>
void Invoke(){
	std::cout<<" Invoke: "<< sizeof ...(Args) << std::endl;
	// return (void*)1;
};

// template <size_t N>
// struct build_indices{
// 	using type = typename build_indices<N-1>::type::next;
// };


// template <>
// struct build_indices<0>{
// 	using type = indices<>;
// };

// using BuildIndices = typename buildc

// template<typename re_type, typename value_type, size_t num_args>
// struct unpack_caller{
// private:
// 	template<typename FuncType, size_t ... I>
// 	re_type FAKE_CALL(FuncType& f, std::vector<value_type>& args){
// 		return f(args[I]...)
// 	};

// public:
// 	template <typename FuncType>
// 	re_type operator()(FuncType & f, std::vector<value_type> & args){
// 		return FAKE_CALL(f, args, )
// 	}

// };


int main(){
	
	std::vector<int> args = {1,2,3};

	Invoke<int, float, bool>();
	Invoke<int, float>();
	Invoke<int, float, vector<bool>, vector<int>>();

	// Add(1, 2);

	// void* p_func = Add;
	// using MY_FUNC = std::function<int(int, int)> ;

	std::function<int(int &, int &)> p_func = Add;
	auto f = reinterpret_cast<int(*)(int, int)>(Add);


	auto f2 = reinterpret_cast<int(*)(void*)>(Add);


	int s1=1, s2= 2;
	p_func(s1, s2);
	// f(2, 3);

	std::vector<int> tu{3, 4};

	struct tmp {
		int a;
		int b;
	};
	tmp t{3, 4};
	f2(&tu);
	f2(&t);

	return 0;
};
