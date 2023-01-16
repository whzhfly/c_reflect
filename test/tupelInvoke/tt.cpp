


#include <iostream>
#include <cstdarg>
// #include <vadefs.h>

// https://zhuanlan.zhihu.com/p/100878076

void addSum(int count, ...){
	va_list args;
	va_start(args, count);
	int sum =0;


	std::cout<< "Sum = " << sum <<std::endl;
	va_end(args);
}


void sub(int a, int b){
	std::cout<< "Sum = " << "sub" <<std::endl;
}

int main(){
	addSum(3, 4, 5, 6);
	addSum(2,3, 4);

	sub(1, 2);

	// sub(1, ...);
	return 0;
}