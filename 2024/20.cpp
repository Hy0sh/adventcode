#include <iostream>
#include "../Util.h"
#include "../Djikstra.cpp"
#include <algorithm>
#include <vector>

using namespace std;

PrintNode getPrintNode(int x, int y, vector<PrintNode> path) {
	for(int i = 0; i < path.size(); i++) {
		PrintNode node = path[i];
		if(node.x == x && node.y == y){
			return node;
		}
	}
	return PrintNode(-1, -1, Direction::UP, INT_MAX);
}

int getDistance(PrintNode a, PrintNode b){
	return abs(a.x - b.x) + abs(a.y - b.y);
}

int solve(const string& input, int maxDistance, int minSavedTime) {
	Grid* grid = Grid::createFromInput(input);
	vector<Direction> directions = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
	vector<int> startPosition = grid->find('S');
	vector<int> endPosition = grid->find('E');
	Node start(startPosition);
	Node end(endPosition);
	SolveInput solveInput = {
		grid,
		{'#'},
		false,
		false,
		[] (Node &current, Node &next, Direction dir) {
			return current.distance + 1;
		},
		start,
		end,
		directions,
	};

	tuple<int, vector<PrintNode>> result = solveDjikstra(solveInput);
	vector<PrintNode> path = get<1>(result);
	int bestScore = get<0>(result);

	map<PrintNode, int> pathMap;
	for(auto node : path){
		pathMap[node] = bestScore -  node.distance; // distance from end
	}

	int nbShortCuts = 0;
	for (auto [node, dte] : pathMap)
	{
		for(auto [nextNode, nextNodeScore]: pathMap){
			if(node == nextNode){
				continue;
			}
			if(getDistance(node, nextNode) <= maxDistance) {
				if((dte - nextNodeScore) - getDistance(node, nextNode) >= minSavedTime){
					nbShortCuts++;
				}
			}
		}
	}
	
	return nbShortCuts;
}

int solution_1(const string& input, int minSavedTime) {
	return solve(input, 2, minSavedTime);
}

int solution_2(const string& input, int minSavedTime) {
	return solve(input, 20, minSavedTime);;
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);
	bool testMode = argv[2] != nullptr && string(argv[2]) == "test";
	cout << "Part 1: " << solution_1(input, testMode ? 0 : 100) << '\n';
	cout << "Part 2: " << solution_2(input, testMode ? 50 : 100) << '\n';

	return 0;
}