
#ifndef E_TOM_C_CMD_ARG
#define E_TOM_C_CMD_ARG

#include <string>
#include <vector>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>


namespace eTomC{


	class CmdArgs{
	public:
		class Exception
		{
		public:
			Exception(const std::string& error);

			std::string GetError() const;

		private:
			std::string error_;
		};

		template <typename U>
		inline static U Convert(const std::string& s);

		template <typename T>
		class Arg{
		public:
			T Get() const {
				return _value;
			}

			void Set(const std::string & value){
				_value = Convert<T>(value);
			}
		private:
			Arg(T default_value): _value(default_value) {};
			T _value;
			friend class CmdArgs;
		};

		// register cmd function
		template <typename T>
		std::shared_ptr<Arg<T>> Register(
				const std::string& cmd,
				const std::string& help,
				T default_value)
		{
			if (cmd.empty() || cmd[0] != '-'){
				throw Exception("Invalid Argument " + cmd + ".");
			} 


			if (_args.find(cmd) != _args.end()){
				throw Exception("Argument of the same cmd already registered.");
			}

			auto arg = std::make_shared<Arg<T>>(Arg<T>(default_value));

			_args[cmd] = {
				help,
				[arg](const std::string& s) { arg->Set(s); }
			};
			return std::move(arg);
		}

		// consume argc input
		std::vector<std::string> Consume(int& argc, char**& argv);

		void PrintHelp() const;

	private:
		struct InternalArg
		{
			std::string help;
			std::function<void(const std::string&)> setter;
		};

		using Args = std::unordered_map<std::string, InternalArg>;
		Args _args;
	};

	// specific template
	template <>
	inline std::string CmdArgs::Convert<std::string>(const std::string& s)
	{
		return s;
	}

	template <>
	inline int CmdArgs::Convert<int>(const std::string& s)
	{
		return std::stoi(s);
	}

	template <>
	inline bool CmdArgs::Convert<bool>(const std::string& s)
	{
		if (s == "true")
		{
			return true;
		}
		else if (s == "false")
		{
			return false;
		}
		else
		{
			throw Exception("Unable to parse bool value " + s + ".");
		}
	}
}

#endif //program once
