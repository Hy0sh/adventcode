#include <iostream>
#include <map>
#include "../Util.h"

using namespace std;

map<string, vector<string>> cache = {};

void incrementCountTypeSones(map<string, ino64_t> &countTypeSones, const string &stone, ino64_t increment)
{
	if (countTypeSones.find(stone) == countTypeSones.end())
	{
		countTypeSones[stone] = 0;
	}
	countTypeSones[stone] += increment;
}

ino64_t resolve(const string &input, int blink)
{
	vector<string> numbers = splitString(splitString(input, '\n')[0], ' ');
	map<string, ino64_t> countTypeSones = {};
	
	for (string number : numbers)
	{
		incrementCountTypeSones(countTypeSones, number, 1);
	}

	for (int i = 0; i < blink; i++)
	{
		map<string, ino64_t> newCountTypeSones = {};
		for (auto [stone, count] : countTypeSones)
		{
			if (cache.find(stone) != cache.end())
			{
				for (string cacheStone : cache[stone])
				{
					incrementCountTypeSones(newCountTypeSones, cacheStone, count);
				}
				continue;
			}

			if (stone == "0")
			{
				cache[stone] = {"1"};
				incrementCountTypeSones(newCountTypeSones, "1", count);
			}
			else if (stone.size() % 2 == 0)
			{
				cache[stone] = {stone.substr(0, stone.size() / 2), to_string(stoll(stone.substr(stone.size() / 2)))};
				for (string cacheStone : cache[stone])
				{
					incrementCountTypeSones(newCountTypeSones, cacheStone, count);
				}
			}
			else
			{
				cache[stone] = {to_string(stoll(stone) * 2024)};
				incrementCountTypeSones(newCountTypeSones, cache[stone][0], count);
			}

		}
		countTypeSones = newCountTypeSones;
	}

	ino64_t sum = 0;
	for (auto [stone, count] : countTypeSones)
	{
		sum += count;
	}

	return sum;
}

ino64_t solution_1(const string &input)
{
	return resolve(input, 25);
}

ino64_t solution_2(const string &input)
{
	return resolve(input, 75);
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}