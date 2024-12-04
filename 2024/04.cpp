#include <iostream>
#include "../Util.h"
#include "../Grid.cpp"

using namespace std;

int solution_1(const string& input) {
	int total = 0;
	Grid* grid = Grid::createFromInput(input);
	vector<string> aviablesDirections = Grid::getAviablesDirections();

	for(int x = 0; x < grid->getRowsCount(); x++) {
		vector<char> row = grid->getRow(x);
		for(int y = 0; y < row.size(); y++) {
			for(auto direction : aviablesDirections) {
				string word = grid->getStr(x, y, direction, 4);
				if(word == "XMAS") {
					total++;
				}
			}
		}
	}

	return total;
}

int solution_2(const string& input) {
	int total = 0;
	Grid* grid = Grid::createFromInput(input);


	for(int x = 0; x < grid->getRowsCount(); x++) {
		vector<char> row = grid->getRow(x);
		for(int y = 0; y < row.size(); y++) {
			char c = grid->get(x, y);
			if(c != 'A') {
				continue;
			}
			string upLeft = {
				grid->get(x + 1, y + 1),
				grid->get(x - 1, y - 1)
			};

			string upRight = {
				grid->get(x - 1, y + 1),
				grid->get(x + 1, y - 1)
			};

			if((upLeft == "MS" || upLeft == "SM") && (upRight == "MS" || upRight == "SM")) {
				total++;
			}
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