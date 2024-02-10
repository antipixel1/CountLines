#pragma once
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
	constexpr  const static  std::array<std::string, 6> valid_extensions{ ".cpp", ".hpp", ".h", ".hxx", ".cxx", ".ixx" };
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