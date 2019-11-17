#pragma once
#include "Filters.h"
#include <vector>
#include <utility>
#include <map>
#include <string>
#include "png_toolkit.h"

enum {
	RED, EDGE, BLUR, THRESHOLD, BLACK_WHITE
};
class Manager {
private:
	std::vector<std::pair<std::string, std::vector<int>>> workList;
	std::vector<Filter*> filters;
	std::map<std::string, int> types = { {"Red", RED}, {"Edge", EDGE}, {"Blur", BLUR}, {"Threshold", THRESHOLD} , {"BlWh", BLACK_WHITE} };
public:
	void Parser(char* fileName);
	void MakeFilters();
	void Run(char* input, char* output);
	~Manager();
};