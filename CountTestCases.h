#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <array>
#include <algorithm>
#include "Include.h"
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
		if (start != string::npos)
		{
			++test_cases_count;
		}
	}
	static size_t test_cases_quantity() { return test_cases_count; }
	inline static const std::array<std::string, 6> valid_extensions{ ".cpp"s, ".hpp"s, ".h"s, ".hxx"s, ".cxx"s, ".ixx"s };
	static bool is_valid_extension(std::string ext)
	{
		if (ext.empty())	return false;
		auto ret = std::find(valid_extensions.cbegin(), valid_extensions.cend(), ext);
		return ret != valid_extensions.cend();
	}
};