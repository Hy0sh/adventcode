#include <iostream>
#include "../Util.h"
#include <map>

using namespace std;

int64_t getNbPossibleTowels(string towel, vector<string> colors, map<string, int64_t> *cache) {
	if(cache->find(towel) != cache->end()) {
		return cache->at(towel);
	}

	if(towel.empty()) {
		cache->insert({towel, true});
		return true;
	}
	int64_t nbPossibleColors = 0;
	for(string color : colors) {
		if(towel.rfind(color,0) == 0) {
			nbPossibleColors += getNbPossibleTowels(towel.substr(color.size()), colors, cache);
		}
	}
	cache->insert({towel, nbPossibleColors});
	return nbPossibleColors;
}

int64_t solve(const string& input,int64_t(*solution)(int64_t nbTowels)) {
	vector<string> lines = splitString(input, '\n');
	vector<string> colors;

	for(string color : splitString(lines[0], ',')) {
		color = trim(color, ' ');
		colors.push_back(color);
	}
	int64_t nbTowels = 0;
	map<string, int64_t> cache;
	for(int i = 2; i < lines.size(); i++) {
		string towel = lines[i];
		nbTowels += solution(getNbPossibleTowels(towel, colors, &cache));
	}

	return nbTowels;
}

int64_t solution_1(const string& input) {
	return solve(input, [](int64_t nbTowels) {
		return (int64_t)(nbTowels > 0 ? 1 : 0);
	});
}

int64_t solution_2(const string& input) {
	return solve(input, [](int64_t nbTowels) {
		return nbTowels;
	});
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}