#include <iostream>
#include <sstream>

#include "arg_parser.h"

int main(int argc, const char* argv[])
{
	std::cout << std::endl;

	auto args = command_args::parse(argc, argv);
	args.print(std::cout);

	return 0;
}
