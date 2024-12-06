#include "Util.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

vector<string> splitString(const string& str, char splitter) {
	vector<string> vec = vector<string>();

	int last = 0;
	int size = str.size();
	int i;
	for (i = 0; i < size; i++) {
		if (str[i] == splitter) {
			vec.push_back(str.substr(last, i - last));
			last = i + 1;
		}
	}
	string leftover = str.substr(last, i - last);
	if (!leftover.empty())
		vec.push_back(str.substr(last, i - last));

	return vec;
}


int stringToInt(string str) {
	int num = 0;
	int multiplier = 1;
	for (int i = str.size() - 1; i >= 0; i--) {
		int digit = str[i] - '0';
		num += digit * multiplier;
		multiplier *= 10;
	}
	return num;
}

string readInput(char* argv[]) {
	if (argv[1] == nullptr) {
		cerr << "Error: Please provide the path to the inputs folder" << '\n';
		exit(EXIT_FAILURE);
	}

	ifstream inputFile = ifstream((stringstream() << argv[1]).str());
    if (!inputFile.is_open()) {
		cerr << "Error: input file not found." << '\n';
		exit(EXIT_FAILURE);
    }

	string fileContent = string((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());

	while (fileContent.back() == '\n')
		fileContent.erase(fileContent.size() - 1);
	fileContent.push_back('\n');

	inputFile.close();
	return fileContent;
}

vector<size_t> findAll(const string &str, const string &sub)
{
	vector<size_t> positions; // holds all the positions that sub occurs within str

	size_t pos = str.find(sub, 0);
	while (pos != string::npos)
	{
		positions.push_back(pos);
		pos = str.find(sub, pos + 1);
	}

	return positions;
}