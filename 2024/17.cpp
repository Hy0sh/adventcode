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
	vector<int> instructions;

	int64_t getComboValue(int operand)
	{
		switch (operand)
		{
			case 0:
			case 1:
			case 2:
			case 3:
				return operand;
				break;
			case 4:
				return A;
				break;
			case 5:
				return B;
				break;
			case 6:
				return C;
				break;
			
			default:
				// error case
				cout << "Invalid operand: " << operand << '\n';
				return -1;
				break;
		}
	}

	vector<int> executeProgram()
	{
		int pointer = 0;
		vector<int> outputs;
		while (pointer < instructions.size())
		{
			int opcode = instructions[pointer];
			int operand = instructions[pointer + 1];
			
			switch (opcode)
			{
			case 0: // adv instruction (opcode 0)
				A /= pow(2, getComboValue(operand));
				break;
			case 1: // bxl instruction (opcode 1)
				B ^= operand;
				break;
			case 2: // bst instruction (opcode 2)
				B = getComboValue(operand) % 8;
				//keep 3 lowest bits
				B &= 0b111;
				break;
			case 3: // jnz instruction (opcode 3)
				if (A != 0)
				{
					pointer = operand;
					continue;
				}
				break;
			case 4: // bxc instruction (opcode 4)
				B ^= C;
				break;
			case 5: // out instruction (opcode 5)
				outputs.push_back(getComboValue(operand) % 8);
				break;
			case 6: // bdv instruction (opcode 6)
				B = A / pow(2, getComboValue(operand));
				break;
			case 7: // cdv instruction (opcode 7)
				C = A / pow(2, getComboValue(operand));
				break;
			default:
				cout << "Invalid opcode: " << opcode << '\n';
				break;
			}
			pointer += 2;
		}

		return outputs;
	}
};

Program parseProgram(const string &input)
{
	Program program;
	vector<string> lines = splitString(input, '\n');
	for (const string &line : lines)
	{
		if (line.empty())
		{
			continue;
		}
		vector<string> parts = splitString(line, ':');

		if (parts[0] == "Register A")
		{
			program.A = stoi(trim(parts[1], ' '));
		}
		else if (parts[0] == "Register B")
		{
			program.B = stoi(trim(parts[1], ' '));
		}
		else if (parts[0] == "Register C")
		{
			program.C = stoi(trim(parts[1], ' '));
		}
		else
		{
			vector<string> instructions = splitString(parts[1], ',');
			for (const string &inst : instructions)
			{
				if (inst.empty())
				{
					continue;
				}
				program.instructions.push_back(stoi(trim(inst, ' ')));
			}
		}
	}
	return program;
}

string outputsToString(const vector<int> &outputs)
{
	string output = "";
	for (int i = 0; i < outputs.size(); i++)
	{
		output += to_string(outputs[i]);
		if (i < outputs.size() - 1)
		{
			output += ",";
		}
	}
	return output;
}

string solution_1(const string &input)
{
	Program program = parseProgram(input);
	vector<int> outputs = program.executeProgram();
	return outputsToString(outputs);
}

int64_t solution_2(const string &input)
{
	Program program = parseProgram(input);
	int64_t A = pow(8, program.instructions.size() - 1);
	int64_t limit = pow(8, program.instructions.size());

	while (A < limit)
	{
		cout << "Trying A = " << A << '\n';
		program.A = A;
		vector<int> outputs = program.executeProgram();		
		if (outputs == program.instructions)
		{
			return A;
		}
		A += 1;
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	string input = readInput(argv);

	cout << "Part 1: " << solution_1(input) << '\n';
	cout << "Part 2: " << solution_2(input) << '\n';

	return 0;
}