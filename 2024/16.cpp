#include <iostream>
#include <queue>
#include <map>
#include <set>
#include "../Util.h"
#include "../Grid.cpp"


using namespace std;

struct PrintNode {
	int x;
	int y;
	Direction direction;

	bool operator==(const PrintNode &a) const
	{
		return x == a.x && y == a.y;
	}

	bool operator<(const PrintNode &a) const
	{
		return x < a.x || (x == a.x && y < a.y);
	}
};

struct Node {
	int x;
	int y;
	int distance;
	Direction direction;

	vector<PrintNode> path;

	bool operator>(const Node &a) const
	{
		return distance > a.distance;
	}

	bool operator==(const Node &a) const
	{
		return x == a.x && y == a.y;
	}

	string toString()
	{
		return to_string(x) + "," + to_string(y) + "," + to_string(direction);
	}
};

Node createNode(vector<int> pos)
{
	return {
		.x = pos[0],
		.y = pos[1],
		.distance = INT_MAX
	};
}

map<int, vector<PrintNode>> solve(const string &input){
	Grid *grid = Grid::createFromInput(input);
	vector<int> startPosition = grid->find('S');
	vector<int> endPosition = grid->find('E');

	vector<Direction> directions = {
		Direction::UP,
		Direction::RIGHT,
		Direction::DOWN,
		Direction::LEFT,
	};

	map<string, int> distances;

	do{
		for(Direction dir : directions){
			Node node = createNode(grid->getPos());
			node.direction = dir;
			distances[node.toString()] = INT_MAX;
		}
	}while(grid->next());


	priority_queue<Node,vector<Node>, greater<>> pq;
	Node start = createNode(startPosition);
	start.distance = 0;
	start.direction = Direction::RIGHT;

	pq.push(start);

	int bestScore = INT_MAX;
	map<int, vector<PrintNode>> paths;

	while(!pq.empty()){
		Node current = pq.top();
		pq.pop();

		if(distances[current.toString()] < current.distance){
			continue;
		}

		if(current.x == endPosition[0] && current.y == endPosition[1]){
			bestScore = min(bestScore, current.distance);
			current.path.push_back({current.x, current.y, current.direction});
			for(auto node : current.path){
				paths[current.distance].push_back(node);
			}
		}

		distances[current.toString()] = current.distance;

		for(Direction dir : directions){
			if(dir == grid->getReveseDirection(current.direction)){
				continue;
			}
			grid->setPos(current.x, current.y);
			if(grid->nextInDirection(dir) && grid->current() != '#'){
				Node next = createNode(grid->getPos());
				next.distance = current.distance + (dir == current.direction ? 1 : 1001);
				next.direction = dir;
				next.path = current.path;
				next.path.push_back({current.x, current.y, dir});

				pq.push(next);
			}

		}
	}

	// for(auto node : paths[bestScore]){
	// 	char c = ' ';
	// 	switch (node.direction)
	// 	{
	// 		case Direction::UP:
	// 			c = '^';
	// 			break;
	// 		case Direction::DOWN:
	// 			c = 'v';
	// 			break;
	// 		case Direction::LEFT:
	// 			c = '<';
	// 			break;
	// 		case Direction::RIGHT:
	// 			c = '>';
	// 			break;
	// 	}
	// 	grid->setCharAt(node.x, node.y, c);
	// }
	// grid->print();

	return paths;
}

int64_t solution_1(const string &input)
{
	map<int, vector<PrintNode>> paths = solve(input);
	int bestScore = INT_MAX;
	for(auto path : paths){
		bestScore = min(bestScore, path.first);
	}
	return bestScore;
}

int solution_2(const string &input)
{
	map<int, vector<PrintNode>> paths = solve(input);
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