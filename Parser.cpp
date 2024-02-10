#include "Parser.h"

Parser::Parser(bool verbose): verbose{ verbose }
{}

bool Parser::ProcessFile(std::filesystem::path file)
{
	using namespace std;
	namespace fs = std::filesystem;
	auto current = fs::current_path();
	constexpr int value_test = 30;

	const string ext = file.extension().string();

	if (!is_valid_extension(ext))
		return false;
	auto parent = Include::find_or_create(file.string(), IncludeType::Root);
	if (parent->has_been_opened())  // this file has already been opened and analized
	{
		if (verbose)
		{
			std::cout << std::format("***** {} has already been opened before", file.string()) << std::endl;
		}
		return false;
	}

	my_input.open(file.string(), std::ios::in);
	if (!my_input.good())
	{
		string str = format("Could not open {}", file.string());
		throw exception(str.c_str());
	}

	parent->mark_opened();

	string line;
	do
	{
		getline(my_input, line);
		process_line(file, parent,line );
	} while (my_input.good());
	// mark parent as processed to prevent cycles in printing
	parent->mark_processed();
	parent->print();
	my_input.close();

	for (auto child : parent->dependencies)
	{
		fs::path p{ child->name };
		ProcessFile(p);
	}
	return true;
}
