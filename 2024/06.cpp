#include <iostream>
#include <map>
#include "../Util.h"
#include "../Grid.cpp"
#include <climits>

using namespace std;

Direction rotate90Right(Direction direction)
{
	switch (direction)
	{
	case Direction::RIGHT:
		return Direction::DOWN;
	case Direction::LEFT:
		return Direction::UP;
	case Direction::UP:
		return Direction::RIGHT;
	case Direction::DOWN:
		return Direction::LEFT;
	}

	return direction;
}

int solution_1(const string &input)
{
	Grid *grid = Grid::createFromInput(input);
	grid->setDebug(true);

	Direction currentDirection = Direction::UP;

	vector<int> pos = grid->find('^');
	grid->setPos(pos[0], pos[1]);
	grid->setCharAt(pos[0], pos[1], grid->getDebugChar());

	while (true)
	{
		pos = grid->findNext('#', currentDirection);
		if (pos[0] == INT_MIN)
		{
			break;
		}
		grid->backwardDirection(currentDirection); // a step back to be front of the wall
		currentDirection = rotate90Right(currentDirection);
	}

	return grid->countChar(grid->getDebugChar());
}

string getLoopKey(const vector<int> &pos, Direction direction)
{
	return to_string(pos[0]) + "-" + to_string(pos[1]) + "," + to_string((int)direction);
}

void setLastObstacle(Grid *grid, vector<int> &lastObstacle, bool resetOldValue = true)
{

	if(resetOldValue){
		grid->setCharAt(lastObstacle[0], lastObstacle[1], '.');
	}
	lastObstacle[1]++;
	if(grid->get(lastObstacle[0], lastObstacle[1]) == '\0') {
		lastObstacle[1] = 0;
		lastObstacle[0]++;
	}
	
	if(grid->get(lastObstacle[0], lastObstacle[1]) == '#')
	{
		return setLastObstacle(grid, lastObstacle, false);
	}	

	if(grid->get(lastObstacle[0], lastObstacle[1]) != '\0') {
		grid->setCharAt(lastObstacle[0], lastObstacle[1], '#');
	}
}

int solution_2(const string &input)
{
	Grid *grid = Grid::createFromInput(input);

	Direction currentDirection = Direction::UP;

	vector<int> pos = grid->find('^');
	vector<int> initialPos = pos;
	grid->setPos(pos[0], pos[1]);
	grid->setCharAt(pos[0], pos[1], grid->getDebugChar());

	map<string, bool> loopDetector;

	int totalLoops = 0;
	vector<int> lastObstacle = {0, 0};

	while (grid->get(lastObstacle[0], lastObstacle[1]) != '\0')
	{
		pos = grid->findNext('#', currentDirection);
		string loopKey = getLoopKey(pos, currentDirection);
		if (loopDetector[loopKey] || pos[0] == INT_MIN)
		{
			if(loopDetector[loopKey]){
				totalLoops++; 
			}
			loopDetector.clear();
			setLastObstacle(grid, lastObstacle);
			grid->setPos(initialPos[0], initialPos[1]);
			currentDirection = Direction::UP;
		}else{
			grid->backwardDirection(currentDirection); // a step back to be front of the wall
			loopDetector[loopKey] = true;
			currentDirection = rotate90Right(currentDirection);
		}
	}

	return totalLoops;
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}