#include <iostream>
#include "../Util.h"
#include <regex>

using namespace std;

struct Robot {
	int x;
	int y;
	vector<int> direction;
};

struct Game {
	vector<Robot> robots;
	int maxX = 101; 
	int maxY = 103;
};

Game parseInput(const string& input) {
	Game game;
	vector<Robot> robots;	
	vector<string> lines = splitString(input, '\n');
	regex re("p=(-?[0-9]+),(-?[0-9]+) v=(-?[0-9]+),(-?[0-9]+)");
	smatch match;
	for (int i = 0; i < lines.size(); i++) {
		Robot robot;
		regex_search(lines[i], match, re);
		robot.x = stoi(match[1].str());
		robot.y = stoi(match[2].str());
		robot.direction.push_back(stoi(match[3].str()));
		robot.direction.push_back(stoi(match[4].str()));
		robots.push_back(robot);
	}

	game.robots = robots;

	return game;
}

Robot moveRobot(const Robot& robot, int rounds, int maxX, int maxY) {
	Robot newRobot = robot;
	// calculate new position of robot
	// new position = old position + direction * rounds % max position + max position (to avoid negative values) % max position (to wrap around)
	newRobot.x = ((robot.x + robot.direction[0] * rounds)%maxX + maxX) % maxX;
	newRobot.y = ((robot.y + robot.direction[1] * rounds)%maxY + maxY) % maxY;
	return newRobot;
}

int solution_1(const string& input) {
	Game game = parseInput(input);
	int rounds = 100;
	int middleX = game.maxX / 2;
	int middleY = game.maxY / 2;
	vector<int> quadrants = {0, 0, 0, 0};
	for(Robot robot : game.robots) {
		robot = moveRobot(robot, rounds, game.maxX, game.maxY);
		if(robot.x != middleX && robot.y != middleY) {
			if(robot.x < middleX && robot.y < middleY) {
				quadrants[0]++;
			} else if(robot.x > middleX && robot.y < middleY) {
				quadrants[1]++;
			} else if(robot.x < middleX && robot.y > middleY) {
				quadrants[2]++;
			} else if(robot.x > middleX && robot.y > middleY) {
				quadrants[3]++;
			}
		}
	}
	return quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
}

vector<vector<bool>> getRobotRepresentation(vector<Robot> robots, int width, int height) {
	vector<vector<bool>> robots_representation = vector<vector<bool>>(height, vector<bool>(width, false));
	for(Robot robot : robots) {
		robots_representation[robot.y][robot.x] = true;
	}
	return robots_representation;
}

bool isEasterEgg(vector<Robot> robots, int width, int height) {
	vector<vector<bool>> robots_representation = getRobotRepresentation(robots, width, height);

	for(int y = 2; y < height  - 2; y++) {
		for(int x = 2; x < width - 2; x++) {
			if(robots_representation[y][x] &&  // top of triangle
				robots_representation[y+1][x] &&  // second row of triangle middle
				robots_representation[y+1][x+1] && // second row of triangle right
				robots_representation[y+1][x-1] && // second row of triangle left
				robots_representation[y+2][x] &&  // bottom of triangle middle
				robots_representation[y+2][x+1] &&  // bottom of triangle right
				robots_representation[y+2][x-1] && // bottom of triangle left
				robots_representation[y+2][x+2] && // bottom of triangle right
				robots_representation[y+2][x-2] && // bottom of triangle left
			) {
				return true;
			}
		}
	}

	return false;

}

void printGrid(vector<vector<bool>> robots_representation) {
	for(int y = 0; y < robots_representation.size(); y++) {
		for(int x = 0; x < robots_representation[0].size(); x++) {
			cout << (robots_representation[y][x] ? '#' : '.');
		}
		cout << '\n';
	}
}

int solution_2(const string& input) {
	Game game = parseInput(input);
	int rounds = 0;
	while (true)
	{
		vector<Robot> robots = {};
		for(Robot robot : game.robots) {
			Robot newRobot = moveRobot(robot, rounds, game.maxX, game.maxY);
			robots.push_back(newRobot);
		}
		if(isEasterEgg(robots, game.maxX, game.maxY)) {
			printGrid(getRobotRepresentation(robots, game.maxX, game.maxY));
			break;
		}
		rounds++;
	}
	
	return rounds;
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);
	
	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}