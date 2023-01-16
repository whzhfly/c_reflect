#include "cmdargs.hpp"
#include <iostream>








using namespace eTomC;
using namespace std;


void IncrementArgcArgv(int& argc, char**& argv)
{
	--argc;
	++argv;
}

CmdArgs::Exception::Exception(const string& error)
:	error_(error)
{
}

string CmdArgs::Exception::GetError() const
{
	return error_;
}

vector<string> CmdArgs::Consume(int& argc, char**& argv){
	vector<string> vec;

	while(argc > 0){
		const string arg = *argv;
		// 文件 直接保存
		if (arg[0] != '-'){
			vec.push_back(arg);
			IncrementArgcArgv(argc, argv);
			continue;
		}

		// -- 表示退出 不是 --a 是单独的 --
		if (arg == "--"){
			IncrementArgcArgv(argc, argv);
			// exit
			break;
		}

		// 都是 -a -b --a 这种注册过的
		auto it = _args.find(arg);
		// 注册的限定符
		if (it == _args.end()){
			throw Exception(
				"Unknow argument" + arg + " - did you forget -- ?" 
			);
		}

		if (argc == 1){
			throw Exception("No value provided for " + arg);
		}

		const string value = argv[1];
		it->second.setter(value);

		// Increment twice - 1 for param name and 1 for param value.
		IncrementArgcArgv(argc, argv);
		IncrementArgcArgv(argc, argv);
	}

	for (auto arg : vec){
		std::cout<<arg<<std::endl;
	}

	return vec;

}

void CmdArgs::PrintHelp() const
{
	for (const auto& arg : _args)
	{
		cout << arg.first << ":\t" << arg.second.help << endl;
	}
}
// namespace eTomC{




//
