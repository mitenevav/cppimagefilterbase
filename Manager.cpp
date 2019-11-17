#include "Manager.h"
#include <fstream>

void Manager::Parser(char* fileName)
{
	std::ifstream f(fileName);
	if (!f.is_open()) {
		throw "Invalid config file!";
	}

	while (!f.eof()) {
		std::string filterName;
		std::vector<int> borders;

		try {
			f >> filterName;

			int num;
			for (int i = 0; i < 4; i++) {
				f >> num;
				borders.push_back(num);
			}
			workList.push_back(std::pair<std::string, std::vector<int>>(filterName, borders));
		}
		catch (...) {
			break;
		}
	}
	f.close();
}

void Manager::MakeFilters()
{
	for (int i = 0; i < workList.size(); i++) {
		int type;
		type = types.find(workList[i].first)->second;
		switch (type)
		{
		case RED:
			filters.push_back(new RedFilter(workList[i].second[0], workList[i].second[1], workList[i].second[2], workList[i].second[3]));
			break;

		case BLUR:
			filters.push_back(new BlurFilter(workList[i].second[0], workList[i].second[1], workList[i].second[2], workList[i].second[3]));
			break;

		case EDGE:
			filters.push_back(new EdgeFilter(workList[i].second[0], workList[i].second[1], workList[i].second[2], workList[i].second[3]));
			break;

		case THRESHOLD:
			filters.push_back(new ThresholdFilter(workList[i].second[0], workList[i].second[1], workList[i].second[2], workList[i].second[3]));
			break;

		case BLACK_WHITE:
			filters.push_back(new BlWhFilter(workList[i].second[0], workList[i].second[1], workList[i].second[2], workList[i].second[3]));
			break;

		default:
			break;
		}
	}
}

void Manager::Run(char* input, char* output)
{
	png_toolkit studTool;
	studTool.load(input);
	
	for (int i = 0; i < filters.size(); i++) {
		studTool.process_image(filters[i]);
	}
	

	studTool.save(output);
}

Manager::~Manager()
{
	for (int i = 0; i < filters.size(); i++) {
		delete filters[i];
	}
}
