#include <iostream>
#include "../Util.h"
#include "../Grid.cpp"
#include <map>
#include <algorithm>

using namespace std;

struct Pos
{
	int x;
	int y;

	Pos(vector<int> coord)
	{
		x = coord[0];
		y = coord[1];

	}

	string toString()
	{
		return "(" + to_string(x) + ", " + to_string(y) + ")";
	}

	Pos addVector(vector<int> v)
	{
		return Pos({x + v[0], y + v[1]});
	}

	bool operator==(const Pos &p) const
	{
		return x == p.x && y == p.y;
	}
};

struct Side {
	vector<Pos> positions;
	string direction;

	Side(Pos p, string dir)
	{
		positions.push_back(p);
		direction = dir;
	}

	bool isSameSide(Pos perimeterPoint)
	{
		vector<vector<int>> reverseDirections;
		
		if(direction == "up" || direction == "down") {
			reverseDirections = {{0,1},{0,-1}};
		}
		if(direction == "left" || direction == "right") {
			reverseDirections = {{1,0},{-1,0}};
		}

		for(Pos p : positions) {
			for(vector<int> reverseDirection : reverseDirections) {
				Pos pNew = p.addVector(reverseDirection);
				if(pNew == perimeterPoint) {
					return true;
				}
			}
		}
		
		return false;
	}
};

struct Region
{
	vector<Pos> positions;
	char letter;

	map<string, vector<int>> getPerimeterPoint(Pos p)
	{
		map<string, vector<int>> perimeter = {
			{"up", {p.x - 1, p.y}},
			{"down", {p.x + 1, p.y}},
			{"left", {p.x, p.y - 1}},
			{"right", {p.x, p.y + 1}},
		};
		for (Pos b : positions)
		{
			if (perimeter.size() == 0)
			{
				break;
			}
			if (b.x == p.x && b.y == p.y - 1)
			{
				perimeter.erase("left");
			}
			if (b.x == p.x && b.y == p.y + 1)
			{
				perimeter.erase("right");
			}
			if (b.x == p.x - 1 && b.y == p.y)
			{
				perimeter.erase("up");
			}
			if (b.x == p.x + 1 && b.y == p.y)
			{
				perimeter.erase("down");
			}
		}

		return perimeter;
	}

	int getPerimeter()
	{
		int perimeter = 0;
		for (Pos p : positions)
		{
			perimeter += getPerimeterPoint(p).size();
		}
		return perimeter;
	}

	int getNbSides()
	{
		vector<Side> sides;
		for (Pos p : positions)
		{
			map<string, vector<int>> perimeter = getPerimeterPoint(p);
			if (perimeter.size() > 0) // If the point is on the edge
			{
				for (auto [dir, _] : perimeter)
				{
					bool found = false;
					for(Side &s : sides) {
						if(s.direction != dir) {
							continue;
						}
						if(s.isSameSide(p)) {
							s.positions.push_back(p);
							found = true;
						}
					}
					if(!found) {
						sides.push_back(Side(p, dir));
					}
				}
			}
		}
		return sides.size();
	}
};

bool sortPositions(Pos a, Pos b)
{
	if (a.x == b.x)
	{
		return a.y < b.y;
	}
	return a.x < b.x;
}

vector<Region> getRegions(Grid *grid)
{
	vector<Region> regions;

	map<string, bool> assigned;
	vector<Direction> directions = {
		Direction::RIGHT,
		Direction::LEFT,
		Direction::UP,
		Direction::DOWN,
	};

	do
	{
		Pos pInitial = Pos(grid->getPos());
		if (assigned[pInitial.toString()])
		{
			continue;
		}

		Region r;
		r.letter = grid->current();
		r.positions.push_back(pInitial);
		assigned[pInitial.toString()] = true;
		vector<Pos> toCheck = {pInitial};
		
		while (toCheck.size() > 0)
		{
			Pos p = toCheck.back();
			toCheck.pop_back();
			grid->setPos(p.x, p.y);
			for (Direction d : directions)
			{
				if (grid->getByDirection(d) == r.letter)
				{
					grid->nextInDirection(d); // Move to the next letter
					Pos pNew = Pos(grid->getPos());
					if (!assigned[pNew.toString()])
					{
						r.positions.push_back(pNew);
						toCheck.push_back(pNew);
						assigned[pNew.toString()] = true;
					}
					grid->setPos(p.x, p.y);
				}
			}
		}

		regions.push_back(r);
		grid->setPos(pInitial.x, pInitial.y);

	} while (grid->next());

	return regions;
}

int solution_1(const string &input)
{
	Grid *grid = Grid::createFromInput(input);
	vector<Region> regions = getRegions(grid);
	int price = 0;
	for (Region r : regions)
	{
		price += r.getPerimeter() * r.positions.size();
	}
	return price;
}

int solution_2(const string &input)
{
	Grid *grid = Grid::createFromInput(input);
	vector<Region> regions = getRegions(grid);
	int price = 0;
	
	for (Region r : regions)
	{
		sort(r.positions.begin(), r.positions.end(), sortPositions);
		price += r.getNbSides() * r.positions.size();
	}
	return price;
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}