#include <iostream>
#include <queue>
#include <map>
#include "../Util.h"
#include "../Grid.cpp"

using namespace std;

struct Node {
	int x;
	int y;
	int score;

	Node(int x, int y, int score) : x(x), y(y), score(score) {}

	bool operator==(const Node& a) const {
		return x == a.x && y == a.y;
	}

	bool operator<(const Node& a) const {
		return x < a.x || (x == a.x && y < a.y);
	}

	bool operator!=(const Node& a) const {
		return x != a.x || y != a.y;
	}
};

Grid* createGrid(const string& input, int length, int limitBits) {
	Grid *grid = new Grid(length, length);
	vector<string> lines = splitString(input, '\n');
	for (int i = 0; i < limitBits; i++) {
		string line = lines[i];
		vector<string> pos = splitString(line, ',');
		int x = stoi(trim(pos[1], ' '));
		int y = stoi(trim(pos[0], ' '));
		grid->setCharAt(x, y, '#');
	}
	return grid;
}

int solve(const string& input, int length, int limitBits) {
	Grid *grid = createGrid(input, length, limitBits);
	vector<Direction> directions = {
		Direction::RIGHT,
		Direction::LEFT,
		Direction::UP,
		Direction::DOWN,
	};

	Node start(0, 0, 0);

	Node end(length - 1, length - 1, 0);

	map<Node, int> visited;

	priority_queue<Node> q;
	q.push(start);

	while (!q.empty()) {
		Node current = q.top();
		q.pop();
		for(Direction dir : directions){
			grid->setPos(current.x, current.y);
			if(grid->nextInDirection(dir) && grid->current() != '#'){
				Node next(grid->getPos()[0], grid->getPos()[1], current.score + 1);
				if(visited.find(next) != visited.end() && visited[next] <= next.score){
					continue;
				}
				visited[next] = next.score;
				if(next != end){
					q.push(next);
				}
			}
		}	
	}
	
	return visited.find(end) != visited.end() ? visited[end] : -1;
}

int solution_1(const string& input, bool testMode) {
	int length = testMode ? 7 : 71;
	int limitBits = testMode ? 12 : 1024;
	return solve(input, length, limitBits);
}

string solution_2(const string& input, bool testMode) {
	int length = testMode ? 7 : 71;
	int limitBits = testMode ? 12 : 1024;
	vector<string> lines = splitString(input, '\n');
	while(solve(input, length, limitBits) != -1 && limitBits < lines.size()){
		limitBits++;
	}
	return lines[limitBits - 1];
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);
	bool testMode = argv[2] != nullptr && string(argv[2]) == "test";

	cout << "Part 1: " << solution_1(input, testMode) << '\n';
	cout << "Part 2: " << solution_2(input, testMode) << '\n';

	return 0;
}