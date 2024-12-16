#include <iostream>
#include "../Util.h"
#include "../Grid.cpp"
#include <map>

using namespace std;

struct Game {
	Grid* grid;
	vector<char> instructions;
};

struct Box {
	vector<vector<int>> positions;
};

Game parseInput(const string& input) {
	vector<string> lines = splitString(input, '\n');
	vector<vector<char>> gridRows;
	vector<char> instructions;
	bool endOfGrid = false;
	for(auto line : lines) {
		if(line.empty()) {
			endOfGrid = true;
			continue;
		}
		if(endOfGrid) {
			vector<char> newsInstructions = vector<char>(line.begin(), line.end());
			instructions.insert(instructions.end(), newsInstructions.begin(), newsInstructions.end());
		} else {
			gridRows.push_back(vector<char>(line.begin(), line.end()));
		}
	}

	Grid* grid = Grid::createFromVectors(gridRows);
	return {grid, instructions};
}

int solution_1(const string& input) {
	Game game = parseInput(input);
	map<char, Direction> directions = {
		{'<', Direction::LEFT},
		{'>', Direction::RIGHT},
		{'^', Direction::UP},
		{'v', Direction::DOWN}
	};
	vector<int> initialPos = game.grid->find('@');
	game.grid->setPos(initialPos[0], initialPos[1]);

	for(auto instruction : game.instructions) {
		Direction direction = directions[instruction];
		char nextChar = game.grid->getByDirection(direction);
		if(nextChar == '#' || nextChar == '\0') {
			continue;
		}
		if(nextChar == '.') {
			game.grid->setCharAt(game.grid->getPos()[0], game.grid->getPos()[1], '.');
			game.grid->nextInDirection(direction);
			game.grid->setCharAt(game.grid->getPos()[0], game.grid->getPos()[1], '@');

		}else if(nextChar == 'O'){
			vector<vector<int>> positionsToMove = {};
			vector<int> backupPos = game.grid->getPos();
			bool canMove = true;
			while(game.grid->nextInDirection(direction)) {
				if(game.grid->current() == 'O'){
					positionsToMove.push_back(game.grid->getPos());
					continue;
				}
				if(game.grid->current() == '#' || game.grid->current() == '\0') {
					canMove = false;
					break;
				}
				if(game.grid->current() == '.') {
					break;
				}
			}

			if(canMove) {
				// move boxes
				vector<int> lastPos = positionsToMove[positionsToMove.size() - 1];
				game.grid->setPos(lastPos[0], lastPos[1]);
				game.grid->nextInDirection(direction);
				game.grid->setCharAt(game.grid->getPos()[0], game.grid->getPos()[1], 'O');
				// move player
				game.grid->setCharAt(backupPos[0], backupPos[1], '.');
				game.grid->setPos(backupPos[0], backupPos[1]);
				game.grid->nextInDirection(direction);
				game.grid->setCharAt(game.grid->getPos()[0], game.grid->getPos()[1], '@');
			}else{
				game.grid->setPos(backupPos[0], backupPos[1]);
			}
		}
	}

	int sum = 0;
	for(vector<int> pos : game.grid->getCharPositions('O')) {
		sum += 100* pos[0] + pos[1];
	}

	return sum;
}

int solution_2(const string& input) {
	string newInput = "";
	vector<string> lines = splitString(input, '\n');
	bool endOfGrid = false;
	for(auto line : lines) {
		if(line.empty()) {
			newInput += '\n';
			endOfGrid = true;
			continue;
		}else if(!endOfGrid) {
			vector<char> oldRow = vector<char>(line.begin(), line.end());
			vector<char> newRow = vector<char>();
			for(char c : oldRow) {
				if(c == 'O') {
					newRow.push_back('[');
					newRow.push_back(']');
				} else if(c == '@') {
					newRow.push_back(c);
					newRow.push_back('.');
				} else{
					newRow.push_back(c);
					newRow.push_back(c);
				}
			}
			newInput += string(newRow.begin(), newRow.end()) + '\n';
		}else{
			newInput += line + '\n';
		}
	}
	Game game = parseInput(newInput);
	
	map<char, Direction> directions = {
		{'<', Direction::LEFT},
		{'>', Direction::RIGHT},
		{'^', Direction::UP},
		{'v', Direction::DOWN}
	};
	vector<int> initialPos = game.grid->find('@');
	game.grid->setPos(initialPos[0], initialPos[1]);

	for(auto instruction : game.instructions) {
		Direction direction = directions[instruction];
		char nextChar = game.grid->getByDirection(direction);
		if(nextChar == '#' || nextChar == '\0') {
			continue;
		}
		if(nextChar == '.') {
			game.grid->setCharAt(game.grid->getPos()[0], game.grid->getPos()[1], '.');
			game.grid->nextInDirection(direction);
			game.grid->setCharAt(game.grid->getPos()[0], game.grid->getPos()[1], '@');

		}else if(nextChar == '[' || nextChar == ']'){
			vector<Box> boxsToMove = {};
			vector<int> backupPos = game.grid->getPos();
			bool canMove = true;
			bool continueLoop = true;
			while (game.grid->nextInDirection(direction) && continueLoop){
				if(game.grid->current() == '#' || game.grid->current() == '\0') {
					canMove = false;
					break;
				}
				// hit directly a box
				if(game.grid->current() == '[' || game.grid->current() == ']'){
					// define box
					Box b = Box();
					bool push = true;
					switch (direction)
					{
						case Direction::RIGHT:
							if(game.grid->current() == '[') {
								b.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]});
								b.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]+1});
							}else{
								push = false;
							}

							break;
						case Direction::LEFT:
							if (game.grid->current() == ']') {
								b.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]-1});
								b.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]});
							}else{
								push = false;
							}
							break;
						case Direction::UP:
						case Direction::DOWN:
							if(game.grid->current() == '[') {
								b.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]});
								b.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]+1});
							}else{
								b.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]-1});
								b.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]});
							}
							if(boxsToMove.size() > 0) {
								push = false;
							}
							break;
					}
					if(push) {
						boxsToMove.push_back(b);
					}
				}
				else if(game.grid->current() == '.'){
					switch (direction)
					{
						case Direction::RIGHT:
						case Direction::LEFT:
							continueLoop = false;
							break;
						case Direction::UP:
						case Direction::DOWN:
							continueLoop = false;
							vector<Box> stack = {
								boxsToMove[0]
							};
							while (stack.size() > 0 && canMove)
							{
								Box b = stack[0];
								stack.erase(stack.begin());
								for(auto pos : b.positions) {
									game.grid->setPos(pos[0], pos[1]);
									game.grid->nextInDirection(direction);
									if(game.grid->current() == '#'){
										canMove = false;
										break;
									}
									if(game.grid->current() == '[' || game.grid->current() == ']'){
										Box newBox = Box();
										if(game.grid->current() == '[') {
											newBox.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]});
											newBox.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]+1});
										}else{
											newBox.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]-1});
											newBox.positions.push_back({game.grid->getPos()[0], game.grid->getPos()[1]});
										}
										stack.push_back(newBox);
										boxsToMove.push_back(newBox);
									}
								}
							}
							break;
					}
				}

			};

			if(canMove) {
				// push boxes
				for(int i= boxsToMove.size() - 1; i >= 0; i--) {
					Box b = boxsToMove[i];
					game.grid->setCharAt(b.positions[0][0], b.positions[0][1], '.');
					game.grid->setCharAt(b.positions[1][0], b.positions[1][1], '.');
					for(int i=0; i < b.positions.size(); i++) {
						vector<int> pos = b.positions[i];
						game.grid->setPos(pos[0], pos[1]);
						game.grid->nextInDirection(direction);
						game.grid->setCharAt(game.grid->getPos()[0], game.grid->getPos()[1], i == 0 ? '[' : ']');
					}
				}
				game.grid->setPos(backupPos[0], backupPos[1]);
				game.grid->setCharAt(backupPos[0], backupPos[1], '.');
				game.grid->nextInDirection(direction);
				game.grid->setCharAt(game.grid->getPos()[0], game.grid->getPos()[1], '@');
			}else{
				game.grid->setPos(backupPos[0], backupPos[1]);
			}
		}
	}

	int sum = 0;
	for(vector<int> pos : game.grid->getCharPositions('[')) {
		sum += 100* pos[0] + pos[1];
	}

	return sum;
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}