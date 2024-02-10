


#include <cassert>
#include <fstream>

#include "Parser.h"

#include <filesystem>


class DirectoryExplorer
{
	const bool verbose;
public:

	DirectoryExplorer(bool verbose);
	void Execute(std::string directory_path);
};

