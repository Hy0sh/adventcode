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

vector<string> splitString(const string& str, string splitter) {
	vector<string> vec = vector<string>();

	int last = 0;
	int size = str.size();
	int i;
	for (i = 0; i < size; i++) {
		if (str.substr(i, splitter.size()) == splitter) {
			vec.push_back(str.substr(last, i - last));
			last = i + splitter.size();
			i += splitter.size() - 1;
		}
	}
	string leftover = str.substr(last, i - last);
	if (!leftover.empty())
		vec.push_back(str.substr(last, i - last));

	return vec;
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

string trimEnd(const string& str, char c = ' ') {
	size_t first = str.find_first_not_of(c);
	size_t last = str.find_last_not_of(c);
	return str.substr(first, (last - first + 1));
}

string trimStart (const string& str, char c = ' ') {
	size_t first = str.find_first_not_of(c);
	return str.substr(first, str.size() - first);
}

string trim(const string& str, char c = ' ') {
	return trimStart(trimEnd(str, c), c);
}