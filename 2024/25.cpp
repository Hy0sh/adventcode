#include <iostream>
#include "../Util.h"
#include <map>

using namespace std;

struct Game {
	vector<map<int, int>> locks;
	vector<map<int, int>> keys;
	int maxHeigth;
};

void addKeyOrLock(Game& game, const vector<string>& current) {
	bool isKey = true;
	for(char c : current[0]) {
		if(c !='#') {
			isKey = false;
			break;
		}
	}
	map<int, int> produces;
	for(int i = 1; i < current.size(); i++) {
		for(int j = 0; j < current[i].size(); j++) {
			if(current[i][j] == '#') {
				produces[j]++;
			}
		}
	}
	if(isKey) {
		game.keys.push_back(produces);
	} else {
		game.locks.push_back(produces);
	}
}

Game parseInput(const string& input) {
	Game game;
	vector<string> lines = splitString(input, '\n');
	vector<string> current;
	for (const string& line : lines) {
		if(line.empty()) {
			addKeyOrLock(game, current);
			game.maxHeigth = current.size() - 1;
			current.clear();
			continue;
		}
		current.push_back(line);

	}
	addKeyOrLock(game, current);
	return game;
}

int solution_1(const string& input) {
	Game game = parseInput(input);
	int nbLockKeyFit = 0;
	for(auto key : game.keys) {
		for(auto lock : game.locks) {
			bool fit = true;
			for(auto [pin, length] : lock) {
				if((key[pin] + lock[pin]) > game.maxHeigth) {
					fit = false;
					break;
				}
			}
			if(fit) {
				nbLockKeyFit++;
			}
		}
	}

	
	return nbLockKeyFit;
}

int solution_2(const string& input) {
	return 0;
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	
	return 0;
}