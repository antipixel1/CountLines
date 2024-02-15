#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>
#include <format>
#include <exception>
#include <algorithm>
#include "CountTestCases.h"
#include "Include.h"
#include "CountedClass.h"
using namespace std::literals;

using namespace std;



class Parser  : public CountedClass<Parser>,public CountTestCases<Parser>
{
	friend class ParserTest;
private:
	const bool verbose;
	std::ifstream my_input;

public:

	Parser(bool verbose);

	bool ProcessFile(std::filesystem::path file);
};

