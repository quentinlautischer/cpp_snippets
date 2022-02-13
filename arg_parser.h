#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <stdexcept>
#include <ios>

/* Usage: 

	// program.exe --key value "in/file/path" -o "out/file/path" -h -v
	auto args = command_args::parse(argc, argv);

	bool is_help = args.help;
	bool is_verbose = args.verbose;
	std::ofstream out(args.outfile);
	std::ifstream in(args.infile);

	auto value = args.get("key");

*/

class command_args
{
public:
	bool help;
	bool verbose;
	std::string infile;
	std::string outfile;
	std::unordered_map<std::string, std::string> values;

public:
	static command_args parse(int argc, const char* argv[]);

	bool has(const std::string& key);
	std::string get(const std::string& key);
	bool get_bool(const std::string&key, std::ios_base &(*io_manipulator)(std::ios_base&) = std::boolalpha);
	template<typename ReturnType>
	ReturnType get(const std::string&key, std::function<ReturnType(const std::string&)> converter);

	void print(std::ostream& stream);

private:
	command_args(int argc, const char* argv[]);
	std::string try_one_arg(const std::string& arg);
};

const std::unordered_map<std::string, std::function<void(command_args&)>> no_args 
{
  {"--help", [](command_args& s){s.help = true;}},
  {"-h", [](command_args& s){s.help = true;}},

  {"--verbose", [](command_args& s){s.verbose = true;}},
  {"-v", [](command_args& s){s.verbose = true;}},

  {"--quiet", [](command_args& s){s.verbose = false;}},
};


command_args command_args::parse(int argc, const char* argv[])
{
	return command_args(argc, argv);
}

void command_args::print(std::ostream& stream)
{
	if (!help && !verbose && infile.empty() && outfile.empty() && values.empty())
	{
		stream << "No command-line arguments." << std::endl;
		return;
	}

	stream << "Command-line arguments are:" << std::endl;
	if (help) stream << "--help" << std::endl;
	if (verbose) stream << "--verbose" << std::endl;
	if (!infile.empty()) stream << "infile: " << infile << std::endl;
	if (!outfile.empty()) stream << "outfile: " << outfile << std::endl;

	for(auto& pair : values)
		stream << pair.first << ": " << pair.second << std::endl;
}

bool command_args::has(const std::string& key)
{
	return values.find(key) != values.end();
}

std::string command_args::get(const std::string& key)
{
	if (has(key))
		return values[key];
	
	return std::string();
}

bool command_args::get_bool(const std::string& key, std::ios_base &(*io_manipulator)(std::ios_base&))
{
	if(!has(key))
		throw std::runtime_error {"'" + key + "' does not exist in args."};

	bool b = false;
	std::istringstream(get(key)) >> io_manipulator >> b;
	return b;
}

template<typename ReturnType>
ReturnType command_args::get(const std::string&key, std::function<ReturnType(const std::string&)> converter)
{
	if(!has(key))
		throw std::runtime_error {"'" + key + "' does not exist in args."};

	return converter(get(key));
}

std::string command_args::try_one_arg(const std::string& arg)
{
	if (arg.substr(0, 2) == "--")
		return arg.substr(2);
	else
	if (arg.at(0) == '-')
		return arg.substr(1);
	else
		return std::string();
}

command_args::command_args(int argc, const char* argv[])
:help(false), verbose(false) 
{
	std::string arg;
	for(int i = 1; i < argc; ++i)
	{
		arg = argv[i];

		if(auto no_arg {no_args.find(arg)}; no_arg != no_args.end())
			no_arg->second(*this);
		else
		if (auto key = try_one_arg(arg); !key.empty())
		{
			if (i + 1 >= argc)
				throw std::runtime_error {"one-arg '" + arg + "' does not have a value"};
			
			if (key == "o" || key == "output")
				outfile = argv[++i];
			else
				values[key] = argv[++i];
		}
		else
		{
			infile = arg;
		}
	}
}

