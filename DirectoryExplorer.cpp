#include "DirectoryExplorer.h"

DirectoryExplorer::DirectoryExplorer(bool verbose): verbose{ verbose }
{}

void DirectoryExplorer::Execute(std::string directory_path)
{
	using namespace std;
	namespace fs = std::filesystem;

	size_t file_count = 0;
	size_t count = CountTestCases<Parser>::test_cases_quantity();
	assert(count == 0);

	Parser parse{ verbose };
	for (auto it = fs::directory_iterator(directory_path); it != fs::directory_iterator{}; ++it)
	{
		if (parse.ProcessFile(*it))
		{
			++file_count;
		}
	}
	count = CountTestCases<Parser>::test_cases_quantity();
	std::cout << std::format("{0} files processed, {1} test cases", file_count,count) << std::endl;
}
