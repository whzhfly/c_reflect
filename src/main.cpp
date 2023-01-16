#include <stdio.h>
#include <iostream>
#include "cmdargs.hpp"
#include <string>
#include "parse/parse.hpp"

using namespace eTomC;

using namespace std;


vector<string> GetFilesToProcess(CmdArgs& cmd_args, int& argc, char**& argv)
	{
		vector<string> files;
		bool wtf = false;
		try
		{
			--argc;
			++argv;
			files = cmd_args.Consume(argc, argv);
			if (files.empty())
			{
				cerr << "No input files specified." << endl;
				wtf = true;
			}
		}
		catch (const CmdArgs::Exception& error)
		{
			cerr << "Error: " << error.GetError() << endl;
			wtf = true;
		}

		if (wtf)
		{
			// cout << "Reflang tool to generate reflection metadata.\n";
			// cout << "\n";
			// cout << "Usage: reflang [reflang_flags] -- [clang_flags]\n";
			// cout << "Where [reflang_flags] are any of the below, and [clang_flags] "
			// 	"are any flags supported by the libclang version installed\n";
			// cout << "\n";
			cout << "Supported flags:\n";
			cmd_args.PrintHelp();
			exit(-1);
		}

		return files;
	}

int main(int argc, char** argv)
{
	CmdArgs cmd_args;
	cout<<"arg-count:" << argc << std::endl;

	// cout<< argv << std::endl;
	auto list_only = cmd_args.Register<bool>(
			"--list-only",
			"Only list type names, don't generate",
			false);
	// auto filter_include = cmd_args.Register<string>(
	// 		"--include",
	// 		"regex for which types to include in reflection generation",
	// 		".*");
	// auto filter_exclude = cmd_args.Register<string>(
	// 		"--exclude",
	// 		"regex for which types to exclude from reflection generation",
	// 		"std::.*");
	// auto reflang_include = cmd_args.Register<string>(
	// 		"--reflang-include",
	// 		"Complete #include line for reflang for generated code.",
	// 		R"(#include "reflang.hpp")");
	// auto out_hpp = cmd_args.Register<string>(
	// 		"--out-hpp",
	// 		"Output file path to write declarations (header) to. If empty "
	// 		"stdout is used (outputs to console).",
	// 		"");
	// auto out_cpp = cmd_args.Register<string>(
	// 		"--out-cpp",
	// 		"Output file path to write definitions to. If empty, --out-hpp is "
	// 		"used.",
	// 		"");

	vector<string> files = GetFilesToProcess(cmd_args, argc, argv);


	vector<FileParse*> fileCollect;
	// collect file-parse
	// use clang to parse
	
	for (auto file : files){
		FileParse* file_parse = new FileParse(); // 推出这个for会析构么？ 如果是是普通构造的话 unordered_map会复制么
		file_parse->ParseFile(file);
		Generate* gen = new Generate(file);
		file_parse->m_generators.push_back(gen);
		fileCollect.push_back(file_parse); // parse 被copy了 m_schema_modules 应该是复制？？
		file_parse->Serialize();
	}
	
	// Serialize file-parse
	// 这种多个文件就出错
	// 应该是cursor 的 问题 提前析构了 在copy的时候 导致找不到
	// for (auto file_parse: fileCollect){
	// 	file_parse->Serialize(); 
	// }
	return 0;
}
