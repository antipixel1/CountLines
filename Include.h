#pragma once
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <format>
#include "printer.h"

enum class IncludeType
{
	Root,
	Child
};


class Include : public std::enable_shared_from_this<Include>
{
	friend class IncludeTests;

	std::vector<std::shared_ptr<Include>> duplicate_dependencies;
	inline static std::vector<std::shared_ptr<Include>> module_seen;
	// factory
	static std::shared_ptr<Include> Include_factory(std::string name);

public:
	std::vector<std::shared_ptr<Include>> dependencies;
	inline static std::vector<std::shared_ptr<Include>> all_roots;
	inline static std::vector<std::shared_ptr<Include>> all_Includes;

	static void print(std::shared_ptr<Include> ancestor, std::shared_ptr<Include> mod, std::shared_ptr<Include> root);

	bool operator==(std::shared_ptr<Include> another);

	std::shared_ptr<Include> getptr();

	// ensure name is a Include in all_Includes
	// this factory method is the only public method to create an Include!!
	static std::shared_ptr<Include> ensure_include(std::string name);
	bool marked_open = false;
public:
	static bool is_root(const std::shared_ptr<Include>& potential_root);

	static bool exists(std::string name);

	static std::shared_ptr<Include> find_or_create(std::string name, IncludeType type);

	void addDependency(std::string name);

	bool has_been_opened();

	void mark_opened();

	void mark_processed();

	void print(int depth = 0);
	const std::string name;
private:

	bool marked_processed = false;

	Include(std::string n);

	// in dependencies
	bool exists_already(std::shared_ptr<Include> candidate);
};

