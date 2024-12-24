#include <iostream>
#include "../Util.h"
#include "24.h"
#include <map>
#include <algorithm>
#include <cmath>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;


struct Game
{
	map<string, int> values;
	map<string, Operation> operations;

	int visit(string wire)
	{
		if (values.find(wire) != values.end())
			return values[wire];

		Operation op = operations[wire];
		int x = visit(op.x);
		int y = visit(op.y);


		if (op.op == "AND")
			values[wire] = x & y;
		else if (op.op == "OR")
			values[wire] = x | y;
		else if (op.op == "XOR")
			values[wire] = x ^ y;

		return values[wire];
	}

	vector<string> getWires(string letter)
	{
		vector<string> wires;
		for (auto &[key, value] : operations)
		{
			if (key.find(letter) == 0)
				wires.push_back(key);
		}

		for (auto &[key, value] : values)
		{
			if (key.find(letter) == 0 && find(wires.begin(), wires.end(), key) == wires.end())
				wires.push_back(key);
		}

		return wires;
	}

	int64_t getResult(string letter)
	{;
		string binaryStr;
		for (auto &[key, value] : values)
		{
			if (key.find(letter) == 0)
			{
				binaryStr += to_string(value);
			}
		}

		reverse(binaryStr.begin(), binaryStr.end());

		return int64_t(stoll(binaryStr, nullptr, 2));
	}
};

Game parseInput(const string &input)
{
	Game game;
	vector<string> lines = splitString(input, '\n');
	bool setValues = true;
	for (const string &line : lines)
	{
		if (line.empty())
		{
			setValues = false;
			continue;
		}

		if (setValues)
		{
			vector<string> parts = splitString(line, ':');
			game.values[trim(parts[0], ' ')] = stoi(trim(parts[1], ' '));
		}
		else
		{
			Operation op;
			vector<string> parts = splitString(line, ' ');
			string x = trim(parts[0], ' ');
			string y = trim(parts[2], ' ');
			// we assure that x is always the smallest value and y the biggest
			op.x = min(x, y);
			op.y = max(x, y);
			op.op = trim(parts[1], ' ');
			game.operations[trim(parts[4], ' ')] = op;
		}
	}
	return game;
}

int64_t solution_1(const string &input)
{
	Game game = parseInput(input);
	int64_t result = 0;
	for (const string &wire : game.getWires("z"))
		game.visit(wire);

	return game.getResult("z");
}

string makeWireName(string letter, int number)
{
	return letter +(number < 10 ? "0" : "")+ to_string(number);
}

bool verifyIntermediateXOR(string wire, int number, map<string, Operation> operations)
{
	if(operations.find(wire) == operations.end())
		return false;
	
	Operation op = operations[wire];
	if(op.op != "XOR")
		return false;

	return min(op.x, op.y) == makeWireName("x", number) && max(op.x, op.y) == makeWireName("y", number);
}

bool verifyRecarry(string wire, int number, map<string, Operation> operations)
{
	if(operations.find(wire) == operations.end())
		return false;
	
	Operation op = operations[wire];
	if(op.op != "AND")
		return false;

	return (verifyIntermediateXOR(op.x, number, operations) && verifyCarryBit(op.y, number, operations)) ||
		   (verifyIntermediateXOR(op.y, number, operations) && verifyCarryBit(op.x, number, operations));
}

bool verifyDirectCarry(string wire, int number, map<string, Operation> operations)
{
	if(operations.find(wire) == operations.end())
		return false;
	
	Operation op = operations[wire];
	if(op.op != "AND")
		return false;

	return makeWireName("x", number) == min(op.x, op.y) && makeWireName("y", number) == max(op.x, op.y);
}

bool verifyCarryBit(string wire, int number, map<string, Operation> operations)
{
	if(operations.find(wire) == operations.end())
		return false;
	
	Operation op = operations[wire];
	if(number == 1)
	{
		if(op.op != "AND")
			return false;
		return makeWireName("x", 0) == min(op.x, op.y) && makeWireName("y", 0) == max(op.x, op.y);
	}

	if(op.op != "OR")
		return false;

	return (verifyDirectCarry(op.x, number - 1, operations) && verifyRecarry(op.y, number - 1, operations)) ||
		   (verifyDirectCarry(op.y, number - 1, operations) && verifyRecarry(op.x, number - 1, operations));
}

// recursive function to verify the z wire
bool verifyZ(string wire, int number, map<string, Operation> operations)
{
	if(operations.find(wire) == operations.end())
		return false;
	
	Operation op = operations[wire];
	if(op.op != "XOR")
		return false;


	if(number == 0)
		return "x00" == min(op.x, op.y) && "y00" == max(op.x, op.y);
	

	return (verifyIntermediateXOR(op.x, number, operations) && verifyCarryBit(op.y, number, operations)) ||
		   (verifyIntermediateXOR(op.y, number, operations) && verifyCarryBit(op.x, number, operations));
	
}

// Verify wire number is correct
bool verify(int number, map<string, Operation> operations)
{
	return verifyZ(makeWireName("z", number), number, operations);
}

// Calculate how far the system progresses
int progress(map<string, Operation> operations)
{
	int number = 0;
	while(true)
	{
		if(!verify(number, operations))
			return number;
		number++;
	}

	return -1;
}

string solution_2(const string &input)
{
	Game game = parseInput(input);
	map<string, Operation> operations = game.operations;
	
	vector<string> swaps;

	// we are looking for 4 swaps
	for(int i = 0; i < 4; i++)
	{
		int basline = progress(operations);
		cout << basline << '\n';
		for(auto &[x, _] : operations)
		{
			bool found = false;
			for(auto &[y, _] : operations)
			{
				if(x == y)
					continue;

				//  swap x and y operations
				Operation temp = operations[x];
				operations[x] = operations[y];
				operations[y] = temp;
				if( progress(operations) > basline){
					// we found a swap
					swaps.push_back(x);
					swaps.push_back(y);
					found = true;
					break;
				}
				// restore
				temp = operations[x];
				operations[x] = operations[y];
				operations[y] = temp;
			}

			if(found)
				break;
		}
	}

	sort(swaps.begin(), swaps.end());

	return accumulate(swaps.begin(), swaps.end(), string(), [](string a, string b)
					  { return a + (a.length() > 0 ? "," : "") + b; });
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}