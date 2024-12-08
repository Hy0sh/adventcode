#include <iostream>
#include <map>
#include <numeric>
#include "../Util.h"
#include "../Grid.cpp"

using namespace std;

vector<int> addVectors(vector<int> a, vector<int> b)
{
	return {
		a[0] + b[0],
		a[1] + b[1]
	};
}

vector<int> subVectors(vector<int> a, vector<int> b)
{
	return {
		a[0] - b[0],
		a[1] - b[1]
	};
}

int solution_1(const string& input) {
	Grid* grid = Grid::createFromInput(input);
	map<char, vector<vector<int>>> antennasPositons;
	
	while (grid->next())
	{
		vector<int> pos = grid->getPos();
		char c = grid->get(pos[0], pos[1]);
		if (c != '.')
		{
			if (antennasPositons.find(c) == antennasPositons.end())
			{
				antennasPositons[c] = {};
			}
			antennasPositons[c].push_back(grid->getPos());
		}
	}

	for(auto& [key, value] : antennasPositons)
	{
		for(int i = 0; i < value.size(); i++)
		{
			vector<int> pos = value[i];
			for(int j = 0; j < value.size(); j++)
			{
				if(i == j)
				{
					continue;
				}
				vector<int> pos2 = value[j];
				vector<int> diff = subVectors(pos, pos2);
				vector<int> newPos = addVectors(pos, diff);
				grid->setCharAt(newPos[0], newPos[1], '#');
			}
		}
	}
	

	return grid->countChar('#');
}

int solution_2(const string& input) {
	Grid* grid = Grid::createFromInput(input);
	map<char, vector<vector<int>>> antennasPositons;

	while (grid->next())
	{
		vector<int> pos = grid->getPos();
		char c = grid->get(pos[0], pos[1]);
		if (c != '.')
		{
			if (antennasPositons.find(c) == antennasPositons.end())
			{
				antennasPositons[c] = {};
			}
			antennasPositons[c].push_back(grid->getPos());
		}
	}

	for(auto& [key, value] : antennasPositons)
	{
		for(int i = 0; i < value.size(); i++)
		{
			vector<int> pos = value[i];
			for(int j = 0; j < value.size(); j++)
			{
				if(i == j)
				{
					continue;
				}
				vector<int> pos2 = value[j];
				vector<int> diff = subVectors(pos, pos2);
				int diffGcd = gcd(diff[0], diff[1]);
				vector<int> vecGdc = {diff[0] / diffGcd, diff[1] / diffGcd};

				vector<int> clonePosAdd = addVectors(pos,vecGdc);
				vector<int> clonePosSub = subVectors(pos, vecGdc);
				while (	grid->get(clonePosAdd[0], clonePosAdd[1]) != '\0' || 
						grid->get(clonePosSub[0], clonePosSub[1]) != '\0'
				)
				{
					grid->setCharAt(clonePosAdd[0], clonePosAdd[1], '#');
					clonePosAdd = addVectors(clonePosAdd, vecGdc);

					grid->setCharAt(clonePosSub[0], clonePosSub[1], '#');
					clonePosSub = subVectors(clonePosSub, vecGdc);
				}
				
			}
		}
	}
	

	return grid->countChar('#');
}

int main(int argc, char* argv[]) {
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}