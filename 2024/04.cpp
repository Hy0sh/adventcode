#include <iostream>
#include "../Util.h"
#include "../Grid.cpp"

using namespace std;

int solution_1(const string &input)
{
	int total = 0;
	Grid *grid = Grid::createFromInput(input);
	vector<Direction> aviablesDirections = Grid::getDirections();

	do
	{
		if (grid->current() != 'X')
		{
			continue;
		}
		for (auto direction : aviablesDirections)
		{
			string word = grid->getStr(direction, 4);
			if (word == "XMAS")
			{
				total++;
			}
		}
	} while (grid->next());

	return total;
}

int solution_2(const string &input)
{
	int total = 0;
	Grid *grid = Grid::createFromInput(input);

	do
	{
		char c = grid->current();
		if (c != 'A')
		{
			continue;
		}

		vector<int> pos = grid->getPos();
		string upLeft = {
			grid->getByDirection(Direction::UP_LEFT),
			grid->getByDirection(Direction::DOWN_RIGHT)};
		string upRight = {
			grid->getByDirection(Direction::UP_RIGHT),
			grid->getByDirection(Direction::DOWN_LEFT)};

		if ((upLeft == "MS" || upLeft == "SM") && (upRight == "MS" || upRight == "SM"))
		{
			total++;
		}
	} while (grid->next());

	return total;
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);
	
	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}