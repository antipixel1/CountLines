Instrucciones:


Pasar la implementacion de las clases a los cpp creando alguno si falta; los .h deben contener solo declaraciones, como por ejemplo:



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

No importa que no entiendan todo en este momento. La Implementacion seria como :


bool Parser::ProcessFile(std::filesystem::path file)
	{
		using namespace std;
		namespace fs = std::filesystem;

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

