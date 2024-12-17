#include <iostream>
#include <math.h>
#include <bits/stdc++.h>
#include "../Util.h"

using namespace std;

struct Program
{
	int64_t A;
	int64_t B;
	int64_t C;
	int pointer;
	vector<int> instructions;

	int64_t getComboValue(int operand)
	{
		if (operand <= 3)
		{
			return operand;
		}
		map<int, int64_t> operand_map = {
			{4, A},
			{5, B},
			{6, C}};
		if (operand_map.find(operand) != operand_map.end())
		{
			return operand_map[operand];
		}

		cout << "Invalid operand: " << operand << '\n';
		return -1;
	}

	tuple<int, vector<int>> executeInstruction(int new_pointer)
	{
		vector<int> outputs;
		int opcode = instructions[new_pointer];
		int operand = instructions[new_pointer + 1];

		switch (opcode)
		{
		case 0: // adv instruction (opcode 0)
			A /= pow(2, getComboValue(operand));
			new_pointer += 2;
			break;
		case 1: // bxl instruction (opcode 1)
			B ^= operand;
			new_pointer += 2;
			break;
		case 2: // bst instruction (opcode 2)
			B = getComboValue(operand) % 8;
			// keep 3 lowest bits
			B &= 0b111;
			new_pointer += 2;
			break;
		case 3: // jnz instruction (opcode 3)
			if (A != 0) new_pointer = operand;
			else new_pointer += 2;
			break;
		case 4: // bxc instruction (opcode 4)
			B ^= C;
			new_pointer += 2;
			break;
		case 5: // out instruction (opcode 5)
			outputs.push_back(getComboValue(operand) % 8);
			new_pointer += 2;
			break;
		case 6: // bdv instruction (opcode 6)
			B = A / pow(2, getComboValue(operand));
			new_pointer += 2;
			break;
		case 7: // cdv instruction (opcode 7)
			C = A / pow(2, getComboValue(operand));
			new_pointer += 2;
			break;
		default:
			cout << "Invalid opcode: " << opcode << '\n';
			break;
		}

		return {new_pointer, outputs};
	}

	vector<int> executeProgram()
	{
		pointer = 0;
		vector<int> outputs;
		while (pointer < instructions.size())
		{
			tuple<int, vector<int>> result = executeInstruction(pointer);
			pointer = get<0>(result);
			vector<int> new_outputs = get<1>(result);
			outputs.insert(outputs.end(), new_outputs.begin(), new_outputs.end());
		}

		return outputs;
	}

	Program(const string &input)
	{
		vector<string> lines = splitString(input, '\n');
		pointer = 0;
		for (const string &line : lines)
		{
			if (line.empty()) continue;
			vector<string> parts = splitString(line, ':');

			if (parts[0] == "Register A") A = stoi(trim(parts[1], ' '));
			else if (parts[0] == "Register B") B = stoi(trim(parts[1], ' '));
			else if (parts[0] == "Register C") C = stoi(trim(parts[1], ' '));
			else
			{
				vector<string> instructionsStr = splitString(parts[1], ',');
				for (const string &inst : instructionsStr)
				{
					if (inst.empty())
					{
						continue;
					}
					instructions.push_back(stoi(trim(inst, ' ')));
				}
			}
		}
	}
};

string solution_1(const string &input)
{
	Program program(input);
	vector<int> outputs = program.executeProgram();
	return accumulate(outputs.begin(), outputs.end(), string(), [](string a, int b)
					  { return a + (a.length() > 0 ? "," : "") + to_string(b); });
}

// crédits to Jérôme for the solution
// https://github.com/jvasseur/advent-of-code/blob/main/2024/src/bin/day17.rs
int64_t solution_2(const string &input)
{
	Program program(input);

	vector<int64_t> possible_results = {0};

	for (int i = program.instructions.size() - 1; i >= 0; i--)
	{
		vector<int64_t> new_possible_results;
		for (int64_t result : possible_results)
		{
			for (int64_t j = 0; j < 8; j++)
			{
				int64_t tentative_result = result | (j << (3 * i));
				Program program(input);
				program.A = tentative_result >> (3 * i);
				vector<int> outputs = program.executeProgram();

				if (outputs[0] == program.instructions[i])
				{
					new_possible_results.push_back(tentative_result);
				}
			}
		}

		possible_results = new_possible_results;
	}

	return possible_results[0];
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}