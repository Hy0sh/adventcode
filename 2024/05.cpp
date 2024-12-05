#include <iostream>
#include <map>
#include <bits/stdc++.h>
#include "../Util.h"

using namespace std;

struct Instructions {
	map <int, vector<int>> rules;
	vector<vector<int>> pagesOrders;
};

Instructions parseInput(const string& input)
{
	Instructions instructions = {};
	vector<string> lines = splitString(input, '\n');
	int pagesOrderIndex = 0;
	for(auto line : lines) {
		if(line.find('|') != string::npos) {
			vector<string> parts = splitString(line, '|');
			int left = stringToInt(parts[0]);
			int right = stringToInt(parts[1]);
			if(instructions.rules.find(left) == instructions.rules.end()) {
				instructions.rules[left] = {};
			}

			instructions.rules[left].push_back(right);
		}else if(line.find(',') != string::npos) {
			vector<string> parts = splitString(line, ',');
			instructions.pagesOrders.push_back({});
			for(auto part : parts) {
				instructions.pagesOrders[pagesOrderIndex].push_back(stringToInt(part));
			}
			pagesOrderIndex++;
		}
	}

	return instructions;
}


int solution_1(const string& input) {
	map <int, vector<int>> rules = {};
	Instructions instructions = parseInput(input);

	int total = 0;

	for(auto orders : instructions.pagesOrders) {
		bool isCorrect = true;
		for(int i = 0; i < orders.size(); i++) {
			int page = orders[i];
			vector<int> rules = instructions.rules[page];
			for(int j = i + 1; j < orders.size(); j++) {
				int nextPage = orders[j];
				if(count(rules.begin(), rules.end(), nextPage) == 0) {
					isCorrect = false;
					break;
				}

				if(!isCorrect) {
					break;
				}
			}
		}

		if(isCorrect) {
			total += orders[orders.size() / 2];
		}
	}
	return total;
}

int solution_2(const string& input) {
	map <int, vector<int>> rules = {};
	Instructions instructions = parseInput(input);

	int total = 0;

	for(auto orders : instructions.pagesOrders) {
		bool isCorrect = true;
		
		for(int i = 0; i < orders.size(); i++) {
			int page = orders[i];
			vector<int> rules = instructions.rules[page];
			for(int j = i + 1; j < orders.size(); j++) {
				int nextPage = orders[j];
				if(count(rules.begin(), rules.end(), nextPage) == 0) {
					isCorrect = false;
					vector<int> nextPagesRules = instructions.rules[nextPage];
					for(int k = 0; k < orders.size(); k++) {
						int prevPage = orders[k];
						if(count(nextPagesRules.begin(), nextPagesRules.end(), prevPage) > 0) {
							int temp = orders[j];
							orders[j] = orders[k];
							orders[k] = temp;
							i = k;
							break;
						}
					}
					break;
				}
			}
		}

		if(!isCorrect) {
			total += orders[orders.size() / 2];
		}
	}
	return total;
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}