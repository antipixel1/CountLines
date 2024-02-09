#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <format>
#include <exception>
#include <array>
#include <algorithm>

#include "Include.h"

using namespace std::literals;

using namespace std;

template<typename T>
class CountTestCases
{
	inline static size_t test_cases_count = 0;
public:
	CountTestCases() {}
	void process_line(std::filesystem::path file, std::shared_ptr<Include> parent, std::string line)
	{
		auto comment_start = line.find("//");
		auto start = line.find("TEST_CASE");
		if (comment_start != string::npos)
		{
			if (comment_start < start)
			{
				return;
			}
		}
		if(start != string::npos )
		{
			++test_cases_count;
		}
	}
	static size_t test_cases_quantity() { return test_cases_count; }
	inline static const std::array<std::string,6> valid_extensions {".cpp"s, ".hpp"s, ".h"s, ".hxx"s, ".cxx"s, ".ixx"s};
	static bool is_valid_extension(std::string ext)
	{
		if (ext.empty())	return false;
		auto ret = std::find(valid_extensions.cbegin(), valid_extensions.cend(), ext);
		return ret != valid_extensions.cend();
	}
};

template<typename T>
class ExtractIncludeFileName
{

public:
	std::pair<bool, std::string> find_include_name(const std::string& line, size_t offset_to_include)
	{
		auto rest = line.substr(offset_to_include + 9);
		// #include "gggg.h"
		// #include <hhh>
		// skip whitespace
		bool is_system_file = false;
		int i = 0;
		for (i = 0; i < rest.size(); ++i)
		{
			char caracter = rest[i];
			if (caracter == '\"')
				continue;
			if (caracter == '<')
			{
				is_system_file = true;
				continue;
			}
			if (caracter != ' ' && caracter != '\t')
				break;
		}
		rest = rest.substr(i);
		// "gggg.h" or <hhhh>

		auto end = rest.find_first_of("\" >");
		auto include_name = rest.substr(0, end);
		return std::make_pair(is_system_file, include_name);
	}
	constexpr  const static  std::array<std::string, 6> valid_extensions {".cpp", ".hpp", ".h", ".hxx", ".cxx", ".ixx"};
	static bool is_valid_extension(std::string ext)
	{
		if (ext.empty())	return false;
		auto ret = std::find(valid_extensions.cbegin(), valid_extensions.cend(), ext);
		return ret != valid_extensions.cend();
	}
	void process_line(std::filesystem::path file, std::shared_ptr<Include> parent, std::string line)
	{
		auto comment_start = line.find("//");
		auto start = line.find("#include ");
		if (comment_start != string::npos)
		{
			if (comment_start < start || start == string::npos)
			{
				return;
			}
		}
		if (start != string::npos)
		{
			concat_parent_include(file, parent, line, start);
		}
	}
	void concat_parent_include(std::filesystem::path file, std::shared_ptr<Include> parent, std::string line, std::string::size_type start)
	{
		auto [sys_file, include_name] = this->find_include_name(line, start);
		auto pp = file.parent_path().string();
		auto root = pp + "\\";
		auto concat = root + include_name;

		std::filesystem::path complete(concat);
		auto n = complete.lexically_normal().string();



		if (!sys_file)
		{
			parent->addDependency(n);
		}
	}
};


template<typename T>
class CountedClass
{
	int count = 0;
protected:
	CountedClass() { ++count; }
	~CountedClass() { --count; }

};

class Parser  : public CountedClass<Parser>,public CountTestCases<Parser>
{
	friend class ParserTest;
private:
	const bool verbose;
	std::ifstream my_input;

public:

	Parser(bool verbose) : verbose{ verbose }
	{}

	bool ProcessFile(std::filesystem::path file)
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
};

