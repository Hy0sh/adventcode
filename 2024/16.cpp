#include <iostream>
#include <queue>
#include <map>
#include <set>
#include "../Util.h"
#include "../Dijkstra.cpp"


using namespace std;

int distanceFunction(Node &current, Node &next, Direction dir)
{
	return current.distance + (dir == current.direction ? 1 : 1001);
}

map<int, vector<PrintNode>> getPaths(const string &input){
	Grid *grid = Grid::createFromInput(input);
	vector<int> startPosition = grid->find('S');
	vector<int> endPosition = grid->find('E');
	vector<Direction> directions = {
		Direction::UP,
		Direction::RIGHT,
		Direction::DOWN,
		Direction::LEFT,
	};

	Node start({startPosition[0], startPosition[1]});
	start.distance = 0;
	start.direction = Direction::RIGHT;

	Node end({endPosition[0], endPosition[1]});

	SolveInput solveInput = {
		grid,
		{'#'},
		false,
		&distanceFunction,
		start,
		end,
		directions
	};

	return solve(solveInput);
}

int64_t solution_1(const string &input)
{
	map<int, vector<PrintNode>> paths = getPaths(input);
	int bestScore = INT_MAX;
	for(auto path : paths){
		bestScore = min(bestScore, path.first);
	}
	return bestScore;
}



int solution_2(const string &input)
{
	
	map<int, vector<PrintNode>> paths = getPaths(input);	
	set<PrintNode> commonTiles;
	int bestScore = INT_MAX;
	for(auto path : paths){
		bestScore = min(bestScore, path.first);
	}
	for(auto node : paths[bestScore]){
		commonTiles.insert(node);
	}

	return commonTiles.size();
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << endl;
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}