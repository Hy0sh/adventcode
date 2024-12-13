#include <iostream>
#include "../Util.h"
#include <regex>

using namespace std;
struct Coordinate {
	int64_t x;
	int64_t y;
};

struct Game {
	Coordinate buttonA;
	Coordinate buttonB;
	Coordinate price;

	int64_t getMinTokens(int64_t offset = 0) {
		int64_t priceX = price.x + offset;
		int64_t priceY = price.y + offset;

        // https://fr.wikipedia.org/wiki/D%C3%A9terminant_(math%C3%A9matiques)
		int64_t det = priceX * buttonA.y - priceY * buttonA.x;
        // https://fr.wikipedia.org/wiki/Syst%C3%A8me_d%27%C3%A9quations_lin%C3%A9aires
		int64_t d = det / (buttonB.x * buttonA.y - buttonB.y * buttonA.x);
		int64_t remaningX = priceX - d * buttonB.x;
		int64_t a = remaningX / buttonA.x;

		bool isPossible = (a * buttonA.y + d * buttonB.y == priceY && remaningX % buttonA.x == 0);
		return isPossible ? 3 * a + d : 0;
	}
};

Coordinate parseCoordinate(const string& line) {
    Coordinate c;
    regex re("(\\d+)");
    smatch match;
    auto it = sregex_iterator(line.begin(), line.end(), re);
    c.x = stoi((*it++).str());
	c.y = stoi((*it).str());
    return c;
}

vector<Game> parseInput(const string& input) {
    vector<Game> games;
    vector<string> lines = splitString(input, '\n');
    Game game;

    for (int i = 0; i < lines.size(); i += 4) {
        game.buttonA = parseCoordinate(lines[i]);
        game.buttonB = parseCoordinate(lines[i + 1]);
        game.price = parseCoordinate(lines[i + 2]);
        games.push_back(game);
    }

    return games;
}

int64_t solve(const string& input, int64_t offset) {
	vector<Game> games = parseInput(input);
	int64_t sum = 0;
	for(Game& game : games) {
		sum += game.getMinTokens(offset);
	}
	return sum;
}

int64_t solution_1(const string& input) {
	return solve(input, 0);
}

int64_t solution_2(const string& input) {
	return solve(input, 10000000000000);
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}