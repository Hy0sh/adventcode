#include <iostream>
#include <map>
#include "../Util.h"
#include "../Grid.cpp"

using namespace std;

struct Position
{
	int x;
	int y;
	int value;
};

struct TrailHead
{
	vector<Position> positions;
};

vector<TrailHead> resolve(const string &input)
{
	Grid *grid = Grid::createFromInput(input);
	vector<vector<int>> initialsPos = grid->getCharPositions('0');
	
	vector<TrailHead> stack;
	for (vector<int> pos : initialsPos)
	{
		TrailHead trailHead = TrailHead();
		trailHead.positions.push_back({pos[0], pos[1], 0});
		stack.push_back(trailHead);
	}


	vector<Direction> directions = {Direction::RIGHT, Direction::LEFT, Direction::UP, Direction::DOWN};

	while (stack.size() > 0)
	{
		TrailHead trailhead = stack[0];
		Position lastPos = trailhead.positions[trailhead.positions.size() - 1];
		if(lastPos.value == 9){
			break;
		}
		stack.erase(stack.begin());
		bool found = false;
		for (Direction direction : directions)
		{
			grid->setPos(lastPos.x, lastPos.y);
			grid->nextInDirection(direction);
			int value = grid->getCurrentIntValue();
			if (value == lastPos.value + 1)
			{
				if(!found){
					trailhead.positions.push_back({grid->getX(), grid->getY(), value});
				}else{
					TrailHead newTrailHead = TrailHead();
					vector<Position> positions = trailhead.positions;
					positions.erase(positions.end() - 1);
					newTrailHead.positions = positions;
					newTrailHead.positions.push_back({grid->getX(), grid->getY(), value});
					stack.push_back(newTrailHead);
				}
				found = true;
			}
		}
		if(found){
			stack.push_back(trailhead);
		}
	}

	return stack;
}

string getKey(int x, int y)
{
	return to_string(x) + "," + to_string(y);
}
int solution_1(const string &input)
{
	vector<TrailHead> stack = resolve(input);
	map<string, bool> visited;
	for(TrailHead trailHead : stack){
		Position start = trailHead.positions[0];
		Position end = trailHead.positions[trailHead.positions.size() - 1];
		visited[getKey(start.x, start.y)+getKey(end.x, end.y)] = true;
	}
	
	return visited.size();
}

int solution_2(const string &input)
{
	return resolve(input).size();
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}