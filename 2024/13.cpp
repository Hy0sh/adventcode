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

		int64_t det = priceX * buttonA.y - priceY * buttonA.x;
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
    string currentField = "buttonA";
    
    for (const string& line : lines) {
        if (line.empty()) {
            games.push_back(game);
            game = Game();
            continue;
        }
        
        if (currentField == "buttonA") {
            currentField = "buttonB";
            game.buttonA = parseCoordinate(line);
        } else if (currentField == "buttonB") {
            currentField = "price";
            game.buttonB = parseCoordinate(line);
        } else if (currentField == "price") {
            game.price = parseCoordinate(line);
            currentField = "buttonA";
        }
    }
    
    games.push_back(game);

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