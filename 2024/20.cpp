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
	return PrintNode(-1, -1, Direction::UP);
}

map<PrintNode, int> getNodesWithDistance(Grid* grid, vector<Direction> directions) {
	vector<int> startPosition = grid->find('S');
	vector<int> endPosition = grid->find('E');
	Node start(startPosition);
	Node end(endPosition);
	SolveInput solveInput = {
		grid,
		{'#'},
		false,
		[] (Node &current, Node &next, Direction dir) {
			return current.distance + 1;
		},
		start,
		end,
		{Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT},
	};

	map<int, vector<PrintNode>> paths = solve(solveInput);
	vector<PrintNode> path = paths.begin()->second;
	int bestScore = paths.begin()->first;

	map<PrintNode, int> pathMap;
	map<int,int> groupScores;

	PrintNode currentNode = getPrintNode(start.x, start.y, path);
	grid->setPos(currentNode.x, currentNode.y);
	do{
		currentNode = getPrintNode(grid->getPos()[0], grid->getPos()[1], path);
		pathMap[currentNode] = bestScore--;
	}while(grid->nextInDirection(currentNode.direction) && pathMap.size() < path.size());

	return pathMap;
}



int getDistance(PrintNode a, PrintNode b){
	return abs(a.x - b.x) + abs(a.y - b.y);
}

int solution_1(const string& input, int minSavedTime) {
	Grid* grid = Grid::createFromInput(input);
	vector<Direction> directions = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
	map<PrintNode, int> pathMap = getNodesWithDistance(grid, directions);

	int nbShortCuts = 0;
	for (auto [node, dte] : pathMap)
	{
		for(Direction dir : directions) {
			grid->setPos(node.x, node.y);
			if(grid->getByDirection(dir) == '#'){
				grid->nextInDirection(dir, 2);
				PrintNode nextNode(grid->getPos()[0], grid->getPos()[1], dir);
				if(pathMap.find(nextNode) != pathMap.end()){
					int score = pathMap[nextNode];
					if((dte - score) -2 >= minSavedTime){
						nbShortCuts++;
					}
				}
			}
		}
	}
	
	return nbShortCuts;
}

int solution_2(const string& input, int minSavedTime) {
	Grid* grid = Grid::createFromInput(input);
	vector<Direction> directions = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
	map<PrintNode, int> pathMap = getNodesWithDistance(grid, directions);

	int nbShortCuts = 0;
	for (auto [node, dte] : pathMap)
	{
		for(auto [nextNode, nextNodeScore]: pathMap){
			if(node == nextNode){
				continue;
			}
			if(getDistance(node, nextNode) <= 20) {
				if((dte - nextNodeScore) - getDistance(node, nextNode) >= minSavedTime){
					nbShortCuts++;
				}
			}
		}
	}

	return nbShortCuts;
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);
	bool testMode = argv[2] != nullptr && string(argv[2]) == "test";
	cout << "Part 1: " << solution_1(input, testMode ? 0 : 100) << '\n';
	cout << "Part 2: " << solution_2(input, testMode ? 50 : 100) << '\n';

	return 0;
}